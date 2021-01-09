#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "PQueue.h"
#include "Point.h"
#include "DynArray.h"

#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "Scene4.h"


#include "PugiXml\src\pugixml.hpp"

#define COST_MAP_SIZE	100

// L03: DONE 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
struct TileSet
{
	SString	name;
	int	firstgid;
	int margin;
	int	spacing;
	int	tileWidth;
	int	tileHeight;

	SDL_Texture* texture;
	int	texWidth;
	int	texHeight;
	int	numTilesWidth;
	int	numTilesHeight;
	int	offsetX;
	int	offsetY;

	// L04: DONE 7: Create a method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
	SDL_Rect GetTileRect(int id) const;
};

// L03: DONE 1: We create an enum for map type, just for convenience,
// NOTE: Platformer game will be of type ORTHOGONAL
enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
struct Properties
{
	struct Property
	{
		SString name;
		int value;
	};

	~Properties()
	{
		//...
	}

	// L06: TODO 7: Method to ask for the value of a custom property

	int GetProperty(const char* name, int default_value = 0) const;

	List<Property*> list;
	Property property;
};

// L04: DONE 1: Create a struct for the map layer
struct MapLayer
{
	SString	name;
	int width;
	int height;
	uint* data;

	Properties properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	// L04: DONE 6: Short function to get the value of x,y
	inline uint Get(int x, int y) const
	{

		return data[(y*width)+x];
	}
};

// L03: DONE 1: Create a struct needed to hold the information to Map node
struct MapData
{
	int width;
	int	height;
	int	tileWidth;
	int	tileHeight;
	SDL_Color backgroundColor;
	MapTypes type;
	List<TileSet*> tilesets;

	// L04: DONE 2: Add a list/array of layers to the map
	List<MapLayer*> layers;
};

class Map : public Module
{
public:

	Map();

	// Destructor
	virtual ~Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	bool Load(const char* path);

	fPoint MapToWorld(float x, float y) const;
	fPoint WorldToMap(float x, float y) const;

	void ShowCollider() { drawColliders = !drawColliders; }

	void LoadColliders();

	void ResetPath(iPoint start);
	void DrawPath();

	int MovementCost(int x, int y) const;

	void ComputePathAStar(int x, int y);
	void PropagateAStar(int heuristic);

	int CalculateDistanceToDestiny(iPoint node);
	int CalculateDistanceToStart(iPoint node);

	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

private:

	// L03: Methods to load all required map data
	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadProperties(pugi::xml_node& node, Properties& properties);
	TileSet* GetTilesetFromTileId(int id) const;

public:
	MapData data;
	iPoint tileDestiny;

private:

	pugi::xml_document mapFile;
	SString folder;
	bool mapLoaded;
	bool drawColliders = false;

	PQueue<iPoint> frontier;
	List<iPoint> visited;

	List<iPoint> breadCrumbs;
	uint costSoFar[COST_MAP_SIZE][COST_MAP_SIZE];
	DynArray<iPoint> path;

	iPoint goalAStar;
	bool finishAStar = false;

	SDL_Texture* tileX = nullptr;
};

#endif // __MAP_H__