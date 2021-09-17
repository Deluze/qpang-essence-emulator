#pragma once

#include <memory>

struct Weapon;
class Room;
class RoomSession;
class RoomSessionPlayer;

class GameMode
{
public:

	enum Mode
	{
		DM = 1,
		TDM = 2,
		PTE = 3,
		VIP = 4,
		PRACTICE = 5,
		PREY = 8,
		PVE = 9,
	};

	virtual bool isTeamMode();
	virtual bool isMissionMode();
	virtual bool isPublicEnemyMode();

	virtual void tick(std::shared_ptr<RoomSession> roomSession);

	virtual void onApply(std::shared_ptr<Room> room);
	virtual void onStart(std::shared_ptr<RoomSession> roomSession);
	virtual void onPlayerSync(std::shared_ptr<RoomSessionPlayer> session);
	virtual void onPlayerKill(std::shared_ptr<RoomSessionPlayer> killer, std::shared_ptr<RoomSessionPlayer> target, const Weapon& weapon, uint8_t hitLocation);
};