#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Collisions.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>

Map::Map() : Module(), mapLoaded(false)
{
	name.Create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.Create(config.child("folder").child_value());

	return ret;
}

// Draw the map (all requried layers)
void Map::Draw()
{
	if (mapLoaded == false) return;

	// L04: DONE 5: Prepare the loop to draw all tilesets + DrawTexture()
	MapLayer* layer = data.layers.start->data;

	// L06: TODO 4: Make sure we draw all the layers and not just the first one

	iPoint point;

	for (int y = 0; y < data.height; ++y)
	{
		for (int x = 0; x < data.width; ++x)
		{
			for (ListItem<MapLayer*>* layer = data.layers.start; layer; layer = layer->next)
			{
				int tileId = layer->data->Get(x, y);
				if (tileId > 0)
				{
					// L04: TODO 9: Complete the draw function       
					iPoint vec = MapToWorld(x, y);
					for (int i = 0; i < data.tilesets.count(); i++)
					{
						if (data.layers.At(i)->data->properties.GetProperty("Nodraw", 0) == 0 || DrawColliders)
							app->render->DrawTexture(data.tilesets.At(i)->data->texture, vec.x, vec.y, &data.tilesets.At(i)->data->GetTileRect(tileId));
							//app->render->DrawTexture(GetTilesetFromTileId(tileId)->texture, vec.x, vec.y, &GetTilesetFromTileId(tileId)->GetTileRect(tileId));
					}
				}
			}
		}
	}
}

// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * data.tileWidth;
	ret.y = y * data.tileHeight;

	// L05: TODO 1: Add isometric map to world coordinates

	return ret;
}

// L05: TODO 2: Add orthographic world to map coordinates
iPoint Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	// L05: TODO 3: Add the case for isometric maps to WorldToMap

	return ret;
}

TileSet* Map::GetTilesetFromTileId(int id) const
{
	/*ListItem<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	for (set; item->next != nullptr; item = item->next, set = item->data)
	{
		if (id >= set->firstgid && id < set->firstgid + (set->numTilesWidth * set->numTilesHeight)) return set;
	}

	return set;*/
	ListItem<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	while (item->data != nullptr)
	{
		if (item->next == nullptr)
		{
			set = item->data;
			break;
		}
		if ((item->data->firstgid < id) && item->next->data->firstgid > id)
		{
			set = item->data;
			break;
		}
		item = item->next;
	}

	return set;
}


// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };

	// L04: DONE 7: Get relative Tile rectangle
	int relativeId = id - firstgid;
	rect.w = tileWidth;
	rect.h = tileHeight;
	rect.x = margin + ((rect.w + spacing) * (relativeId % numTilesWidth));
	rect.y = margin + ((rect.h + spacing) * (relativeId / numTilesWidth));

	return rect;
}

// Called before quitting
bool Map::CleanUp()
{
	LOG("Unloading map");

	// L03: DONE 2: Make sure you clean up any memory allocated from tilesets/map
	// Remove all tilesets
	ListItem<TileSet*>* item;
	item = data.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// L04: DONE 2: clean up all layer data
	// Remove all layers
	ListItem<MapLayer*>* item2;
	item2 = data.layers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.clear();

	// Clean up the pugui tree
	mapFile.reset();

	return true;
}

// Load new map
bool Map::Load(const char* filename)
{
	bool ret = true;
	SString tmp("%s%s", folder.GetString(), filename);

	pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
		ret = false;
	}

	// Load general info
	if (ret == true)
	{
		// L03: DONE 3: Create and call a private function to load and fill all your map data
		ret = LoadMap();
	}

	// L03: DONE 4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!
	pugi::xml_node tileset;
	for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true) ret = LoadTilesetDetails(tileset, set);

		if (ret == true) ret = LoadTilesetImage(tileset, set);

		data.tilesets.add(set);
	}

	// L04: DONE 4: Iterate all layers and load each of them
	// Load layer info
	pugi::xml_node layer;
	for (layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if (ret == true)
			data.layers.add(lay);

		pugi::xml_node propertiesNode;
		for (propertiesNode = layer.child("properties"); propertiesNode && ret; propertiesNode = propertiesNode.next_sibling("properties"))
		{
			Properties* property = new Properties();

			ret = LoadProperties(propertiesNode, *property);

			//data.layers.At(0)->data->properties.list.add();
			lay->properties = *property;
		}
	}

	if (ret == true)
	{
		// L03: TODO 5: LOG all the data loaded iterate all tilesets and LOG everything
		LOG("Successfully parsed Successfully parsed map XML file: %s", filename);
		LOG("width: %d  height: %d", data.width, data.height);

		for (int i = 0; i < data.tilesets.count(); i++)
		{
			LOG("Tileset %d ----", i + 1);
			LOG("name: %s firstgid: %i", data.tilesets.At(i)->data->name.GetString(), data.tilesets.At(i)->data->firstgid);
			LOG("tile width: %d tile height: %d", data.tilesets.At(i)->data->texWidth, data.tilesets.At(i)->data->texHeight);
			LOG("spacing: %d margin: %d", data.tilesets.At(i)->data->spacing, data.tilesets.At(i)->data->margin);
		}


		// L04: TODO 4: LOG the info for each loaded layer
	}

	mapLoaded = ret;

	return ret;
}

// L03: TODO: Load map general properties
bool Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = mapFile.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		// L03: TODO: Load map general properties
		data.width = map.attribute("width").as_int(0);
		data.height = map.attribute("height").as_int(0);
		data.tileWidth = map.attribute("tilewidth").as_int(0);
		data.tileHeight = map.attribute("tileheight").as_int(0);
		data.type = MAPTYPE_ORTHOGONAL;
	}

	return ret;
}

// L03: TODO: Load Tileset attributes
bool Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;

	// L03: TODO: Load Tileset attributes
	set->firstgid = tileset_node.attribute("firstgid").as_int(0);
	set->name = tileset_node.attribute("name").as_string();
	set->margin = tileset_node.attribute("margin").as_int(0);
	set->spacing = tileset_node.attribute("spacing").as_int(0);
	set->tileHeight = tileset_node.attribute("tileheight").as_int(0);
	set->tileWidth = tileset_node.attribute("tilewidth").as_int(0);
	set->numTilesWidth = tileset_node.attribute("columns").as_int(0);
	set->numTilesHeight = tileset_node.attribute("tilecount").as_int(0) / set->numTilesWidth;
	return ret;
}

// L03: TODO: Load Tileset image
bool Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");


	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		// L03: TODO: Load Tileset image
		set->texture = app->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
	}

	return ret;
}

// L04: TODO 3: Create the definition for a function that loads a single layer
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	// L04: TODO 3: Load a single layer
	layer->name = node.attribute("name").as_string("");
	layer->width = node.attribute("width").as_int(0);
	layer->height = node.attribute("height").as_int(0);
	int size = layer->height * layer->width;
	layer->data = new uint[size];

	pugi::xml_node tile = node.child("data").child("tile");
	for (int i = 0; i < size; i++)
	{
		layer->data[i] = tile.attribute("gid").as_int(0);
		tile = tile.next_sibling("tile");
	}

	return ret;
}

bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = true;


	pugi::xml_node property_ = node.child("property");

	Properties::Property* Prop = new Properties::Property();

	for (property_; property_ && ret; property_ = property_.next_sibling("property"))
	{
		Prop->name = property_.attribute("name").as_string("");
		Prop->value = property_.attribute("value").as_int(0);
		properties.list.add(Prop);
	}

	return ret;
}

int Properties::GetProperty(const char* value, int defaultValue) const
{
	for (int i = 0; i < list.count(); i++)
	{
		if (strcmp(list.At(i)->data->name.GetString(), value) == 0)
		{
			if (list.At(i)->data->value != defaultValue) return list.At(i)->data->value;
			else return defaultValue;
		}
	}

	return defaultValue;
}

void Map::LoadColliders()
{
	if (mapLoaded == false) return;


	ListItem<MapLayer*>* L = data.layers.start;
	ListItem<TileSet*>* T = data.tilesets.start;
	TileSet* tileSet = data.tilesets.start->data;
	uint num = 401;
	while (L != nullptr)
	{
		MapLayer* layer = L->data;
		if (layer->properties.GetProperty("Collisions", 1) == 0)
		{

			L = L->next;
			continue;


		}

		for (int y = 0; y < layer->height; y++)
		{
			for (int x = 0; x < layer->width; x++)
			{


				int u = layer->Get(x, y);
				iPoint pos = MapToWorld(x, y);
				SDL_Rect n = { pos.x, pos.y, data.tileWidth, data.tileHeight };
				if (u != 0)
				{
					if (layer->properties.GetProperty("Collisions", 1) == 1)
					{
							app->collisions->AddCollider(n, Collider::Type::FLOOR, this);
					}
					/*if (layer->properties.GetProperty("Collisions", 1) == 2)
					{
						app->collisions->AddCollider(n, Collider::Type::DEATH, this);
					}*/
				}

			}
		}
		L = L->next;
	}
}