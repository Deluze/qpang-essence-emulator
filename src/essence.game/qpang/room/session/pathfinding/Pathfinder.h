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
	void free();

	void updateMapInfo(const MapInfo& mapInfo);

	float getCellWidth();
	float getCellHeight();

	float getCellX(float x);
	float getCellZ(float z);

	// Calculates the time needed for one move with specified speed.
	// This assumes speed means units / second, and that pos1 and pos2 differ one move.
	// NOTE: Returns time in seconds.
	float calculateMoveTime(float speed, const PathfinderCell& pos1, const PathfinderCell& pos2);

	// These 2 methods are for conversion of cells to nodes
	void getCoords(void* node, int* x, int* z);
	void* getNode(int x, int z);

	bool canPassThrough(int x, int z);

	bool solve(const PathfinderCell& start, const PathfinderCell& end, std::vector<PathfinderCell>& path);

	// These 3 methods need to be implemented from the micropather Graph
	float LeastCostEstimate(void* nodeStart, void* nodeEnd) override;
	void AdjacentCost(void* node, MP_VECTOR<micropather::StateCost>* neighbors) override;
	void PrintStateInfo(void* node) override;
private:
	micropather::MicroPather* m_microPather = nullptr;

	MapInfo m_mapInfo = {};

	int m_numCellsX = 0;
	int m_numCellsZ = 0;

	std::weak_ptr<RoomSession> m_roomSession;
};