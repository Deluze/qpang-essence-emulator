#include "pathfinder.h"

#include "RoomSession.h"

enum eCellType : uint8_t
{
	MOVEABLE,
	WALL = 1,
	TAKEN_BY_NPC = 2,
	NO_DIAGONAL_MOVES = 3
};

Pathfinder::Pathfinder()
{
	// Use 60 * 60, as all the grids we're going to be using are 60x60.
	// Increase if they're bigger, however, according to official documentation
	// if the grid consists of more than a few thousand nodes, set it to 1/4 of the amount of nodes.
	m_microPather = new micropather::MicroPather(this, 60 * 60, 8, true);
}

Pathfinder::~Pathfinder()
{
	free();
}

void Pathfinder::initialize(const std::shared_ptr<RoomSession>& roomSession)
{
	m_roomSession = roomSession;
}

void Pathfinder::free()
{
	delete m_microPather;
}

void Pathfinder::updateMapInfo(const MapInfo& mapInfo)
{
	m_mapInfo = mapInfo;
	m_backupMapInfo = m_mapInfo;

	m_numCellsX = m_mapInfo.m_mapGrid[0].size() - 1;
	m_numCellsZ = m_mapInfo.m_mapGrid.size() - 1;

	// This tells micropather that the underlying graph has changed (a.k.a, this class)
	// It will also remove all cache so that it can now cache the new grid
	m_microPather->Reset();
}

float Pathfinder::getCellWidth()
{
	return m_mapInfo.m_cellWidth;
}

float Pathfinder::getCellHeight()
{
	return m_mapInfo.m_cellHeight;
}

int Pathfinder::getCellX(float x)
{
	return ((x - m_mapInfo.m_mapOffX) / m_mapInfo.m_cellWidth);
}

int Pathfinder::getCellZ(float z)
{
	return (((z - m_mapInfo.m_mapOffZ) / m_mapInfo.m_cellHeight) * -1.f);
}

void Pathfinder::cellToCoords(const PathfinderCell& cell, float& x, float& z)
{
	const float cellX = cell.x;
	const float cellZ = cell.z;

	x = cellX * m_mapInfo.m_cellWidth + m_mapInfo.m_mapOffX;
	z = (cellZ * -1.f) * m_mapInfo.m_cellHeight + m_mapInfo.m_mapOffZ;
}

bool Pathfinder::isCellTaken(const PathfinderCell& cell)
{
	if (cell.x >= 0 && cell.x < m_numCellsX
		&& cell.z >= 0 && cell.z < m_numCellsZ)
	{
		return m_mapInfo.m_mapGrid[cell.x][cell.z] == eCellType::TAKEN_BY_NPC;
	}

	return true;
}

void Pathfinder::setCellTaken(const PathfinderCell& cell, bool taken)
{
	if (cell.x >= 0 && cell.x < m_numCellsX
		&& cell.z >= 0 && cell.z < m_numCellsZ)
	{
		// Set the cell to taken, or to what it was if it's not take anymore by using the
		// backup map info.
		m_mapInfo.m_mapGrid[cell.x][cell.z] = taken ? eCellType::TAKEN_BY_NPC : 
			m_backupMapInfo.m_mapGrid[cell.x][cell.z];

		m_microPather->Reset();
	}
}

float Pathfinder::calculateMoveTime(float speed, const PathfinderCell& pos1, const PathfinderCell& pos2)
{
	bool xChange = pos2.x != pos1.x;
	bool zChange = pos2.z != pos1.z;

	float moveTime = 0.f;
	if (xChange && zChange)
		moveTime = ((m_mapInfo.m_cellWidth + m_mapInfo.m_cellHeight) * 0.5f) / speed;
	else if (xChange)
		moveTime = m_mapInfo.m_cellWidth / speed;
	else if (zChange)
		moveTime = m_mapInfo.m_cellHeight / speed;

	return moveTime;
}

void Pathfinder::getCoords(void* node, int* x, int* z)
{
	intptr_t index = (intptr_t)node;
	*z = index / m_numCellsX;
	*x = index - *z * m_numCellsX;
}

void* Pathfinder::getNode(int x, int z)
{
	return (void*)(z * m_numCellsX + x);
}

bool Pathfinder::lineOfSightBetween(const PathfinderCell& pos1, const PathfinderCell& pos2)
{
	// We're going to be using Bresenham's line algorithm to get all cells that lie
	// on the line segment between pos1 and pos2.
	// Then, we're going to loop all the cells on the line and see if any of them have a wall.
	// If they do, there's no line of sight between pos1 and pos2.

	int x, z;

	int dx = pos2.x - pos1.x;
	int dy = pos2.z - pos1.z;

	int dx1 = fabs(dx);
	int dy1 = fabs(dy);

	int px = 2 * dy1 - dx1;
	int py = 2 * dx1 - dy1;

	if (dy1 <= dx1)
	{
		int xe = 0;
		if (dx >= 0)
		{
			x = pos1.x;
			z = pos1.z;
			xe = pos2.x;
		}
		else
		{
			x = pos2.x;
			z = pos2.z;
			xe = pos1.x;
		}

		// The last cell may be in a "wall"
		// and still be considered visible
		if (x != pos2.x && z != pos2.z)
		{
			if (!canPassThrough(x, z, true))
				return false;
		}

		for (int i = 0; x < xe; i++)
		{
			x = x + 1;
			if (px < 0)
				px = px + 2 * dy1;
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
					z = z + 1;
				else
					z = z - 1;

				px = px + 2 * (dy1 - dx1);
			}

			// The last cell may be in a "wall"
			// and still be considered visible
			if (x != pos2.x && z != pos2.z)
			{
				if (!canPassThrough(x, z, true))
					return false;
			}
		}
	}
	else
	{
		int ze = 0;
		if (dy >= 0)
		{
			x = pos1.x;
			z = pos1.z;
			ze = pos2.z;
		}
		else
		{
			x = pos2.x;
			z = pos2.z;
			ze = pos1.z;
		}

		// The last cell may be in a "wall"
		// and still be considered visible
		if (x != pos2.x && z != pos2.z)
		{
			if (!canPassThrough(x, z, true))
				return false;
		}

		for (int i = 0; z < ze; i++)
		{
			z = z + 1;

			if (py <= 0)
				py = py + 2 * dx1;
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
					x = x + 1;
				else
					x = x - 1;

				py = py + 2 * (dx1 - dy1);
			}

			// The last cell may be in a "wall"
			// and still be considered visible
			if (x != pos2.x && z != pos2.z)
			{
				if (!canPassThrough(x, z, true))
					return false;
			}
		}
	}

	return true;
}

bool Pathfinder::canPassThrough(int x, int z, bool ignoreTaken)
{
	if (x >= 0 && x < m_numCellsX
		&& z >= 0 && z < m_numCellsZ)
	{
		if (m_mapInfo.m_mapGrid[x][z] == eCellType::MOVEABLE || (ignoreTaken && m_mapInfo.m_mapGrid[x][z] == eCellType::TAKEN_BY_NPC))
			return true;
	}

	return false;
}

bool Pathfinder::solve(const PathfinderCell& start, const PathfinderCell& end, std::vector<PathfinderCell>& path)
{
	if (start.x == end.x && start.z == end.z)
		return false;

	if (start.x >= m_numCellsX || start.x < 0 || end.x >= m_numCellsX && end.x < 0)
		return false;

	if (start.z >= m_numCellsZ || start.z < 0 || end.z >= m_numCellsZ && end.z < 0)
		return false;

	path.clear();

	float totalCost = 0;
	micropather::MPVector<void*> micropatherPath;

	int result = m_microPather->Solve(getNode(start.x, start.z), getNode(end.x, end.z), &micropatherPath, &totalCost);
	if (result == micropather::MicroPather::SOLVED)
	{
		for (size_t i = 0; i < micropatherPath.size(); ++i)
		{
			int x, z;
			getCoords(micropatherPath[i], &x, &z);

			path.emplace_back(PathfinderCell{ x, z });
		}

		return true;
	}

	return false;
}

float Pathfinder::LeastCostEstimate(void* nodeStart, void* nodeEnd)
{
	int xStart, zStart, xEnd, zEnd;
	getCoords(nodeStart, &xStart, &zStart);
	getCoords(nodeEnd, &xEnd, &zEnd);

	int deltaX = xStart - xEnd;
	int deltaZ = zStart - zEnd;

	return (float)sqrt((double)(deltaX * deltaX) + (double)(deltaZ * deltaZ));
}

void Pathfinder::AdjacentCost(void* node, MP_VECTOR<micropather::StateCost>* neighbors)
{
	const int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	const int dz[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	const float cost[8] = { 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f };

	int x, z;
	getCoords(node, &x, &z);

	auto cellType = m_mapInfo.m_mapGrid[x][z];
	for (int i = 0; i < 8; ++i)
	{
		// If the cell we're on doesn't allow diagonal moves, and i is uneven
		// continue out of here.
		// If you take a look at dx, dz and cost, you can see that every uneven index
		// is a diagonal move.
		bool isDiagonalMove = (i % 2 != 0);
		if (cellType == eCellType::NO_DIAGONAL_MOVES && isDiagonalMove)
			continue;

		const int nx = x + dx[i];
		const int nz = z + dz[i];

		if (canPassThrough(nx, nz))
		{
			micropather::StateCost nodeCost = { getNode(nx, nz), cost[i] };
			neighbors->push_back(nodeCost);
		}
	}
}

void Pathfinder::PrintStateInfo(void* node)
{
	int x, z;
	getCoords(node, &x, &z);
	printf("(%d,%d)", x, z);
}