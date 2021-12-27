#include "pathfinder.h"

#include "RoomSession.h"

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
	x = cell.x * m_mapInfo.m_cellWidth + m_mapInfo.m_mapOffX;
	z = (cell.z * -1.f) * m_mapInfo.m_cellHeight + m_mapInfo.m_mapOffZ;
}

bool Pathfinder::isCellTaken(const PathfinderCell& cell)
{
	return m_mapInfo.m_mapGrid[cell.x][cell.z] == 1;
}

void Pathfinder::setCellTaken(const PathfinderCell& cell, bool taken)
{
	m_mapInfo.m_mapGrid[cell.x][cell.z] = taken;
	m_microPather->Reset();
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

bool Pathfinder::canPassThrough(int x, int z)
{
	if (x >= 0 && x < m_numCellsX
		&& z >= 0 && z < m_numCellsZ)
	{
		if (m_mapInfo.m_mapGrid[x][z] == 0)
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

			path.emplace_back(PathfinderCell { x, z });
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

	for (int i = 0; i < 8; ++i)
	{
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