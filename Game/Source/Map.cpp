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
					fPoint vec = MapToWorld(x, y);
					for (int i = 0; i < data.tilesets.Count(); i++)
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
fPoint Map::MapToWorld(float x, float y) const
{
	fPoint ret;

	ret.x = x * data.tileWidth;
	ret.y = y * data.tileHeight;

	// L05: TODO 1: Add isometric map to world coordinates

	return ret;
}

// L05: TODO 2: Add orthographic world to map coordinates
fPoint Map::WorldToMap(float x, float y) const
{
	fPoint ret(0, 0);
	ret.x = x / data.tileWidth;
	ret.y = y / data.tileHeight;
	// L05: TODO 3: Add the case for isometric maps to WorldToMap

	return ret;
}

TileSet* Map::GetTilesetFromTileId(int id) const
{
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

void Map::ResetPath(iPoint start)
{
	frontier.Clear();
	visited.Clear();
	breadCrumbs.Clear();

	frontier.Push(start, 0);
	visited.Add(start);
	breadCrumbs.Add(start);

	memset(costSoFar, 0, sizeof(uint) * COST_MAP_SIZE * COST_MAP_SIZE);
}

void Map::DrawPath()
{
	iPoint pointV;
	iPoint pointF;
	iPoint pointPath;

	// Draw visited
	ListItem<iPoint>* itemVisited = visited.start;
	PQueueItem<iPoint>* itemFrontier = frontier.start;


	while (itemVisited)
	{
		pointV = itemVisited->data;

		TileSet* tileset = GetTilesetFromTileId(422);

		SDL_Rect rec = tileset->GetTileRect(422);
		fPoint pos = MapToWorld(pointV.x, pointV.y);

		app->render->DrawTexture(tileset->texture, pos.x, pos.y, &rec);
		itemVisited = itemVisited->next;

	}
	while (itemFrontier)
	{
		TileSet* tileset = GetTilesetFromTileId(401);

		SDL_Rect rec = tileset->GetTileRect(401);

		pointF = itemFrontier->data;
		tileset = GetTilesetFromTileId(401);
		fPoint pos = MapToWorld(pointF.x, pointF.y);
		app->render->DrawTexture(tileset->texture, pos.x, pos.y, &rec);
		itemFrontier = itemFrontier->next;
	}
	int pathSize = path.Count();
	for (size_t i = 0; i < pathSize; i++)
	{
		TileSet* tileset = GetTilesetFromTileId(401);

		SDL_Rect rec = tileset->GetTileRect(401);

		pointPath = { path.At(i)->x,path.At(i)->y };
		tileset = GetTilesetFromTileId(401);
		fPoint pos = MapToWorld(pointPath.x, pointPath.y);
		app->render->DrawTexture(tileset->texture, pos.x, pos.y, &rec);

	}
}

int Map::MovementCost(int x, int y) const
{
	int ret = -1;

	if ((x >= 0) && (x < data.width) && (y >= 0) && (y < data.height))
	{
		// Coje el layer de las colisiones que en nuestro caso es el tercero
		int id = data.layers.start->next->next->data->Get(x, y);

		if (id != 0)
		{
			int fisrtGid = GetTilesetFromTileId(id)->firstgid;

			if (id == fisrtGid) ret = 1;
			else if (id == fisrtGid + 1) ret = 0;
			else if (id == fisrtGid + 2) ret = 3;
		}
		else ret = 1;
	}

	return ret;
}


void Map::PropagateAStar(int heuristic)
{
	// Consider the different heuristics
	iPoint curr;
	curr = frontier.GetLast()->data;
	if (frontier.Pop(curr) && curr != tileDestiny)
	{
		if (true)
		{
			iPoint neighbors[4];
			neighbors[0].Create(curr.x + 1, curr.y + 0);
			neighbors[1].Create(curr.x + 0, curr.y + 1);
			neighbors[2].Create(curr.x - 1, curr.y + 0);
			neighbors[3].Create(curr.x + 0, curr.y - 1);

			for (uint i = 0; i < 4; ++i)
			{
				if (MovementCost(neighbors[i].x, neighbors[i].y) > 0)
				{
					if (visited.Find(neighbors[i]) == -1)
					{
						frontier.Push(neighbors[i], CalculateDistanceToDestiny(neighbors[i]) + CalculateDistanceToStart(neighbors[i]));
						visited.Add(neighbors[i]);
						costSoFar[i][0] = MovementCost(neighbors[i].x, neighbors[i].y);
						breadCrumbs.Add(curr);
					}
				}
			}
		}

	}
	else
	{
		breadCrumbs.Add(curr);
		ComputePathAStar(tileDestiny.x, tileDestiny.y);
		app->map->ResetPath(app->map->tileDestiny);
	}
}

void Map::ComputePathAStar(int x, int y)
{
	path.Clear();
	iPoint goal = { x, y };
	int size = breadCrumbs.Count() - 1;
	path.PushBack(goal);

	ListItem<iPoint>* iterator = visited.end;
	ListItem<iPoint>* tmp = breadCrumbs.At(size);

	for (iterator; iterator; iterator = iterator->prev)
	{
		size--;
		if (iterator->data == tmp->data)
		{
			path.PushBack(iterator->data);
			tmp = breadCrumbs.At(size);
		}
	}
}

int Map::CalculateDistanceToDestiny(iPoint node)
{
	iPoint distance = tileDestiny - node;
	return distance.x + distance.y;
}

int Map::CalculateDistanceToStart(iPoint node)
{
	iPoint distance;
	distance = node - visited.start->data;
	return distance.x + distance.y;

}
// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };

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

	ListItem<TileSet*>* item;
	item = data.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.Clear();

	ListItem<MapLayer*>* item2;
	item2 = data.layers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.Clear();

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
		ret = LoadMap();
	}

	pugi::xml_node tileset;
	for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true) ret = LoadTilesetDetails(tileset, set);

		if (ret == true) ret = LoadTilesetImage(tileset, set);

		data.tilesets.Add(set);
	}

	pugi::xml_node layer;
	for (layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if (ret == true)
			data.layers.Add(lay);

		pugi::xml_node propertiesNode;
		for (propertiesNode = layer.child("properties"); propertiesNode && ret; propertiesNode = propertiesNode.next_sibling("properties"))
		{
			Properties* property = new Properties();

			ret = LoadProperties(propertiesNode, *property);

			lay->properties = *property;
		}
	}

	if (ret == true)
	{
		LOG("Successfully parsed Successfully parsed map XML file: %s", filename);
		LOG("width: %d  height: %d", data.width, data.height);

		for (int i = 0; i < data.tilesets.Count(); i++)
		{
			LOG("Tileset %d ----", i + 1);
			LOG("name: %s firstgid: %i", data.tilesets.At(i)->data->name.GetString(), data.tilesets.At(i)->data->firstgid);
			LOG("tile width: %d tile height: %d", data.tilesets.At(i)->data->texWidth, data.tilesets.At(i)->data->texHeight);
			LOG("spacing: %d margin: %d", data.tilesets.At(i)->data->spacing, data.tilesets.At(i)->data->margin);
		}
	}

	mapLoaded = ret;

	return ret;
}

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
		data.width = map.attribute("width").as_int(0);
		data.height = map.attribute("height").as_int(0);
		data.tileWidth = map.attribute("tilewidth").as_int(0);
		data.tileHeight = map.attribute("tileheight").as_int(0);
		data.type = MAPTYPE_ORTHOGONAL;
	}

	return ret;
}

bool Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;

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
		set->texture = app->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
	}

	return ret;
}

bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

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
		properties.list.Add(Prop);
	}

	return ret;
}

int Properties::GetProperty(const char* value, int defaultValue) const
{
	for (int i = 0; i < list.Count(); i++)
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
				fPoint pos = MapToWorld(x, y);
				SDL_Rect n = { pos.x + 1, pos.y, data.tileWidth - 2, data.tileHeight-15 };
				SDL_Rect n2 = { pos.x-1, pos.y+7, 2, 6 };
				SDL_Rect n3 = { pos.x + 15, pos.y + 7, 2, 6 };
				SDL_Rect n4 = { pos.x+6, pos.y+10, 6, 7 };
				if (u != 0)
				{
					if (layer->properties.GetProperty("Collisions", 1) == 1)
					{
							app->collisions->AddCollider(n, Collider::Type::FLOOR, this);
							app->collisions->AddCollider(n2, Collider::Type::LEFT_WALL, this);
							app->collisions->AddCollider(n3, Collider::Type::RIGHT_WALL, this);
							app->collisions->AddCollider(n4, Collider::Type::ROOF, this);
					}
				}

			}
		}
		L = L->next;
	}
}

bool Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;
	ListItem<MapLayer*>* item;
	item = data.layers.start;

	for (item = data.layers.start; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if (layer->properties.GetProperty("Nodraw", 0) == 0)
			continue;

		uchar* map = new uchar[layer->width * layer->height];
		memset(map, 1, layer->width * layer->height);

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int i = (y * layer->width) + x;

				int tileId = layer->Get(x, y);
				TileSet* tileset = (tileId > 0) ? GetTilesetFromTileId(tileId) : NULL;

				if (tileset != NULL)
				{
					map[i] = (tileId - tileset->firstgid) > 0 ? 0 : 1;
				}
			}
		}

		*buffer = map;
		width = data.width;
		height = data.height;
		ret = true;

		break;
	}

	return ret;
}