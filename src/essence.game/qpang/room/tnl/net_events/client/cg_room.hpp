#ifndef CG_ROOM_HPP
#define CG_ROOM_HPP

#include "GameNetEvent.h"

#include "core/Emulator.h"
#include "core/config/ConfigManager.h"

#include "qpang/room/tnl/net_events/server/gc_room.hpp"

class CGRoom : public GameNetEvent
{
	typedef NetEvent Parent;
public:

	CGRoom() : GameNetEvent{ CG_ROOM, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirClientToServer }
	{
		titleBuffer = new ByteBuffer();
	};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream)
	{
		bstream->read(&playerId);
		bstream->read(&cmd);
		bstream->read(&value);
		bstream->read(&mode);
		bstream->read(&memberCount);
		bstream->read(&goal);
		char buf[256];
		bstream->readString(buf);
		password = buf;
		bstream->read(titleBuffer);
		bstream->read(&timeAmount);
		bstream->read(&isRounds);
		bstream->read(&unk_11);
		bstream->read(&unk_12);
		bstream->read(&unk_13);
		bstream->read(&unk_14);
		bstream->read(&_160);
		bstream->read(&_161);
		bstream->read(&_162);
		bstream->read(&_163);
		bstream->read(&unk_19);
		bstream->read(&unk_20);
		bstream->read(&unk_21);
		bstream->read(&unk_22);
	};

	void handle(GameConnection* conn, Player::Ptr player)
	{
		std::u16string title = byteBufferToString(titleBuffer, 30);

		delete titleBuffer;

		if (cmd == Command::CREATE_ROOM || cmd == Command::CREATE_EVENT_ROOM)
		{
			if (cmd == Command::CREATE_EVENT_ROOM && player->getRank() != 3)
				return conn->disconnect("Cannot create event room");

			const auto isValidMode =
				mode == GameMode::Mode::DM ||
				mode == GameMode::Mode::TDM ||
				mode == GameMode::Mode::PTE ||
				mode == GameMode::Mode::VIP ||
				mode == GameMode::Mode::PREY;
			
			if (!isValidMode || map > 12)
			{
				conn->disconnect("Invalid gamemode");

				return player->broadcast(u"Sorry, but this game mode has not been implemented yet");
			}

			if (Game::instance()->getRoomManager()->list().size() >= CONFIG_MANAGER->getInt("GAME_MAX_ROOMS"))
				return conn->disconnect("Failed to create room");

			auto room = Game::instance()->getRoomManager()->create(title, static_cast<uint8_t>(map), mode);

			room->setEventRoom(cmd == Command::CREATE_EVENT_ROOM);
			room->addPlayer(conn);
		}

		else if (cmd == Command::JOIN_ROOM)
		{
			auto room = Game::instance()->getRoomManager()->get(roomId);

			if (room == nullptr)
				return conn->disconnect("Couldn't find room ID");

			if (room->getPlayerCount() >= room->getMaxPlayers())
				return conn->disconnect("Room is full");

			auto pass = room->getPassword();

			if (!pass.empty() && player->getRank() < 3)
				if (pass != password)
					return;

			room->addPlayer(conn);
		}

		else
		{
			auto roomPlayer = player->getRoomPlayer();

			if (roomPlayer == nullptr)
				return;

			auto room = roomPlayer->getRoom();

			if (room->getMasterId() != player->getId())
				return;

			if (room->isPlaying())
				return;

			switch (cmd)
			{
			case Command::MAP_ROOM:
			{
				room->setMap(value);
			}
			break;
			case Command::MODE_ROOM:
			{
				const auto isValidMode =
					mode == GameMode::Mode::DM ||
					mode == GameMode::Mode::TDM ||
					mode == GameMode::Mode::PTE ||
					mode == GameMode::Mode::VIP ||
					mode == GameMode::Mode::PREY;

				if (!isValidMode)
				{
					conn->postNetEvent(new GCRoom(player->getId(), Command::MODE_ROOM, room));

					return player->broadcast(u"Sorry, but this game mode has not been implemented yet");
				}

				room->setMode(mode);
			}
			break;
			case Command::SET_POINTS:
			{
				room->setScorePoints(value);
				room->setIsPointsGame(true);
			}
			break;
			case Command::SET_TIME:
			{
				room->setScoreTime(value);
				room->setIsPointsGame(false);
			}
			break;
			case Command::PLAYERS_ROOM:
			{
				if (value == 4 || value == 8 || value == 12 || value == 16)
					room->setMaxPlayers(value);
			}
			break;
			case Command::PASS_ROOM:
			{
				room->setPassword(password);
			}
			break;
			case Command::LEVEL_ROOM:
			{
				room->setLevelLimited(value);
			}
			break;
			case Command::TOGGLE_MELEE:
			{
				room->setMeleeOnly(value);
			}
			break;
			case Command::TOGGLE_SKILL:
			{
				// room->setSkillsEnabled(!room->isSkillsEnabled());
				// disabled
			}
			break;
			case Command::TEAM_ROOM:
			{
				room->setTeamSorting(value);
			}
			break;
			default:
				break;
			}
		}
	}

	void process(EventConnection* ps)
	{
		post<CGRoom>(ps);
	}


	enum Command : uint32_t
	{
		MAP_ROOM = 1,
		PLAYERS_ROOM = 2,
		MODE_ROOM = 3,
		SET_POINTS = 4,
		PASS_ROOM = 5,
		CREATE_ROOM = 6,
		JOIN_ROOM = 7,
		SET_TIME = 20,
		LEVEL_ROOM = 23,
		TEAM_ROOM = 25,
		TOGGLE_SKILL = 26,
		CREATE_EVENT_ROOM = 27,
		TOGGLE_MELEE = 30,
	};

	U32 playerId = 0; //92
	U32 cmd = Command::CREATE_ROOM; //96

	union {
		U32 value = 0; //100
		U32 map;
	};

	union {
		U8 mode = 0; //109
		U8 roomId;
	};

	union {
		U8 memberCount = 0; //110
		U8 roomId2;
	};

	U8 goal = 0; //111
	std::string password; //104

	union
	{
		U8 timeAmount = 0;
		U8 pointsAmount;
	};

	U8 isRounds = 0; //155
	U32 unk_11; //156

	U32 unk_12; //164

	U16 unk_13; //168
	U32 unk_14; //188

	U8 _160; //160
	U8 _161; //161
	U8 _162; //162
	U8 _163; //163

	U32 unk_19; //184
	U32 unk_20; //180
	U32 unk_21; //172
	U32 unk_22; //176

	ByteBuffer* titleBuffer;

	TNL_DECLARE_CLASS(CGRoom);
};
#endif