#include "App.h"
#include "PathFinding.h"

#include "Defs.h"
#include "Log.h"

PathFinding::PathFinding() : Module(), map(NULL), lastPath(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name.Create("pathfinding");
}
// Destructor
PathFinding::~PathFinding()
{
	RELEASE_ARRAY(map);
}
// Called before quitting
bool PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	lastPath.Clear();
	RELEASE_ARRAY(map);

	return true;
}

void PathFinding::ResetPath(fPoint start)
{
	frontier.Clear();
	visited.Clear();
	breadCrumbs.Clear();

	frontier.Push(start, 0);
	visited.Add(start);
	breadCrumbs.Add(start);
}

void PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width * height];
	memcpy(map, data, width * height);
}

DynArray<fPoint>* PathFinding::GetLastPath()
{
	return &lastPath;
}
PQueue<fPoint>* PathFinding::GetFrontier()
{
	return &frontier;
}
List<fPoint>* PathFinding::GetVisited()
{
	return &visited;
}
List<fPoint>* PathFinding::GetbreadCrumbs()
{
	return &breadCrumbs;
}

bool PathFinding::CheckBoundaries(const fPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}

bool PathFinding::IsWalkable(const fPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

uchar PathFinding::GetTileAt(const fPoint& pos) const
{
	if (CheckBoundaries(pos))
		return map[((int)pos.y * width) + (int)pos.x];

	return INVALID_WALK_CODE;
}

void PathFinding::PropagateAStar(const fPoint& destination)
{
	fPoint curr;
	int newCost[4];
	if (frontier.Pop(curr))
	{
		fPoint neighbors[4];
		neighbors[0].Create(curr.x + 0, curr.y + 1);
		neighbors[1].Create(curr.x - 1, curr.y + 0);
		neighbors[2].Create(curr.x + 0, curr.y - 1);
		neighbors[3].Create(curr.x + 1, curr.y + 0);

		int j = 0;
		bool init = false;
		for (uint i = 0; i < 4; ++i)
		{
			if (GetTileAt(neighbors[i]) > 0)
			{
				if (visited.Find(neighbors[i]) == -1)
				{
					//visited.Add(neighbors[i]);
					newCost[i] = neighbors[i].DistanceManhattan(visited.start->data) + neighbors[i].DistanceManhattan(destination);
					if (newCost[i] >= 0 && init == false)j = i, init = true;
				}
			}
		}

		for (int i = 0; i < 3; i++)
		{
			if (visited.Find(neighbors[i]) == -1)
			{
				if (GetTileAt(neighbors[i]) > 0 && newCost[i + 1] >= 0 && newCost[j] >= newCost[i + 1])j = i + 1;
				//if (!(MovementCost(neighbors[i].x, neighbors[i].y) > 0) && i == 0)j++;
			}
		}
		for (int i = 0; i < 4; i++)
		{
			if (visited.Find(neighbors[i]) == -1 && GetTileAt(neighbors[i]) > 0)
			{
				visited.Add(neighbors[i]);
				breadCrumbs.Add(curr);
				if (newCost[j] == newCost[i])
				{
					frontier.Push(neighbors[i], newCost[i]);
				}
			}
		}
	}
}

void PathFinding::ComputePathAStar(const fPoint& origin, const fPoint& destination)
{
	//ResetPath(origin);
	int count = 0;
	int x = 0;
	while (destinationIsFind == false)
	{
		x++;
 		PropagateAStar(destination);
		for (count; count < visited.Count(); count++)
		{
			if ((int)visited.At(count)->data.x == (int)destination.x && (int)visited.At(count)->data.y == (int)destination.y)
			{
				destinationIsFind = true;
				break;
			}
		}
		if (x > 1000)
		{
			destinationIsFind = true;
			break;
		}
	}

	destinationIsFind = false;
	lastPath.Clear();

	lastPath.PushBack(destination);
	lastPath.PushBack(breadCrumbs.At(count)->data);
	for (int i = visited.Count() - 1; i > 0; i--)
	{
		if (visited.At(i)->data == breadCrumbs.At(count)->data)
		{
			lastPath.PushBack(breadCrumbs.At(i)->data);
			count = i;
		}
	}
	lastPath.Flip();
}