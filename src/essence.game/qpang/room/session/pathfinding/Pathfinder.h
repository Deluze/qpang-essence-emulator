#pragma once

#include "micropather.h"

#include <vector>
#include <memory>

struct MapInfo
{
	float m_mapOffX = 0.f;
	float m_mapOffZ = 0.f;
	float m_cellWidth = 0.f;
	float m_cellHeight = 0.f;

	std::vector<std::vector<uint8_t>> m_mapGrid = {};
};

struct PathfinderCell
{
	PathfinderCell() { x = 0; z = 0; }
	PathfinderCell(int x, int z) { this->x = x; this->z = z; }

	~PathfinderCell() {}

	bool operator==(const PathfinderCell& other) const
	{
		return this->x == other.x && this->z == other.z;
	}

	bool operator!=(const PathfinderCell& other) const
	{
		return !(*this == other);
	}

	int x = 0;
	int z = 0;
};

class RoomSession;
class Pathfinder : public micropather::Graph
{
public:
	Pathfinder();
	~Pathfinder();

	void initialize(const std::shared_ptr<RoomSession>& roomSession);

	void updateMapInfo(const MapInfo& mapInfo);

	float getCellWidth();
	float getCellHeight();

	int getCellX(float x);
	int getCellZ(float z);

	void cellToCoords(const PathfinderCell& cell, float& x, float& z);

	bool isCellTaken(const PathfinderCell& cell);
	void setCellTaken(const PathfinderCell& cell, bool taken);

	// Calculates the time needed for one move with specified speed.
	// This assumes speed means units / second, and that pos1 and pos2 differ one move.
	// NOTE: Returns time in seconds.
	float calculateMoveTime(float speed, const PathfinderCell& pos1, const PathfinderCell& pos2);

	// These 2 methods are for conversion of cells to nodes
	void getCoords(void* node, int* x, int* z);
	void* getNode(int x, int z);

	bool lineOfSightBetween(const PathfinderCell& pos1, const PathfinderCell& pos2);

	bool canPassThrough(int x, int z, bool ignoreTaken = false);

	bool solve(const PathfinderCell& start, const PathfinderCell& end, std::vector<PathfinderCell>& path);

	// These 3 methods need to be implemented from the micropather Graph
	float LeastCostEstimate(void* nodeStart, void* nodeEnd) override;
	void AdjacentCost(void* node, MP_VECTOR<micropather::StateCost>* neighbors) override;
	void PrintStateInfo(void* node) override;
private:
	std::unique_ptr<micropather::MicroPather> m_microPather = nullptr;

	MapInfo m_mapInfo = {};
	MapInfo m_backupMapInfo = {};

	int m_numCellsX = 0;
	int m_numCellsZ = 0;

	std::weak_ptr<RoomSession> m_roomSession;
};