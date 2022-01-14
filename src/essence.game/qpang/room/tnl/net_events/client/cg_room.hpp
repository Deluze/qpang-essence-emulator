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

	CGRoom() : GameNetEvent{ CG_ROOM, GuaranteedOrdered, DirClientToServer }
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

		//std::cout
		//	<< "CGRoom::unpack >> PlayerId: " << playerId << ", Cmd: " << (int)cmd << ", Value: " << (int)value << ", Mode: " << (int)mode
		//	<< ", MemberCount: " << (int)memberCount << ", Goal: " << (int)goal << ", TimeAmount: " << (int)timeAmount << ", IsRounds: " << (int)isRounds << std::endl;

		//std::cout << "CGRoom::unpack >> Unk11: " << (int)unk_11 << ", Unk12: " << (int)unk_12 << ", Unk13: " << (int)unk_13 << ", Unk14: " << (int)unk_14
		//	<< ", Unk19: " << (int)unk_19 << ", Unk20: " << (int)unk_20 << ", Unk21: " << (int)unk_21 << ", Unk22: " << (int)unk_22 << std::endl;

		//std::cout << "CGRoom::unpack >> _160: " << (int)_160 << ", _161: " << (int)_161 << ", _162: " << (int)_162 << ", _163: " << (int)_163 << std::endl;
	};

	void handle(GameConnection* conn, const Player::Ptr player)
	{
		const std::u16string title = byteBufferToString(titleBuffer, 30);

		delete titleBuffer;

		bool usesTweakTool = (unk_19 == 1337 && unk_20 == 1337 && unk_21 == 1337 && unk_22 == 1337);
		if (cmd == CREATE_ROOM || cmd == CREATE_EVENT_ROOM)
		{
			if (cmd == CREATE_EVENT_ROOM && player->getRank() != 3)
			{
				return conn->disconnect("Cannot create event room");
			}

			const auto isValidMode =
				mode == GameMode::Mode::DM ||
				mode == GameMode::Mode::TDM ||
				mode == GameMode::Mode::PTE ||
				mode == GameMode::Mode::VIP ||
				mode == GameMode::Mode::PREY ||
				mode == GameMode::Mode::PVE;

			if (mode == GameMode::Mode::PVE)
			{
				if (player->getName().compare(u"hinnie") != 0 && player->getName().compare(u"Jarrett") != 0)
				{
					player->broadcast(u"Sorry, but you can't create a pve room.");
					return;
				}

				// FIXME: We should also disconnect the connection, otherwise the second time the player sends this packet
				// it will fail. It will work after that though.
				// However, if we disconnect the connection, the client wont see the broadcast message.
				// Possible, but bad? solution is to disconnect, Sleep for roughly a sec, then call the broadcast and return.
				if (!usesTweakTool)
				{
					player->broadcast(u"Sorry, but you have to use the tweak tool in order to create a PVE room. You can enable it in the launcher.");
					return;
				}
			}

			if (!isValidMode)
			{
				conn->disconnect("Invalid gamemode");
				player->broadcast(u"Sorry, but this game mode has not been implemented yet");
				return;
			}

			if (Game::instance()->getRoomManager()->list().size() >= CONFIG_MANAGER->getInt("GAME_MAX_ROOMS"))
			{
				conn->disconnect("Failed to create room");
				return;
			}

			const auto room = Game::instance()->getRoomManager()->create(title, static_cast<uint8_t>(map), mode);

			room->setEventRoom(cmd == CREATE_EVENT_ROOM);
			room->addPlayer(conn);
		}
		else if (cmd == JOIN_ROOM)
		{
			const auto room = Game::instance()->getRoomManager()->get(roomId);

			if (room == nullptr)
			{
				conn->disconnect("Couldn't find room ID");
				return;
			}

			if (room->getMode() == GameMode::Mode::PVE)
			{
				if (!room->isPublicPveRoom())
				{
					if (player->getName().compare(u"hinnie") != 0 && player->getName().compare(u"Jarrett") != 0)
					{
						player->broadcast(u"Sorry, but you can't join this pve room.");
						return;
					}
				}

				// FIXME: We should also disconnect the connection, otherwise the second time the player sends this packet
				// it will fail. It will work after that though.
				// However, if we disconnect the connection, the client wont see the broadcast message.
				// Possible, but bad? solution is to disconnect, Sleep for roughly a sec, then call the broadcast and return.
				if (!usesTweakTool)
				{
					player->broadcast(u"Sorry, but you have to use the tweak tool in order to join a PVE room. You can enable it in the launcher.");
					return;
				}
			}

			if (room->getPlayerCount() >= room->getMaxPlayers())
			{
				return conn->disconnect("Room is full");
			}

			const auto roomPassword = room->getPassword();

			const auto playerRank = player->getRank();
			const auto playerCanBypassPasswordCheck = ((playerRank == 3) || (playerRank == 4));

			// If the room has no password OR the player is a GM or Helper, they can bypass the password check.
			if (roomPassword.empty() || playerCanBypassPasswordCheck)
			{
				room->addPlayer(conn);

				return;
			}

			// Compare the given password with the room password.
			if (password == roomPassword)
			{
				room->addPlayer(conn);
			}
		}
		else
		{
			const auto roomPlayer = player->getRoomPlayer();

			if (roomPlayer == nullptr)
			{
				return;
			}

			const auto room = roomPlayer->getRoom();

			if (room->getMasterId() != player->getId())
			{
				return;
			}

			if (room->isPlaying())
			{
				return;
			}

			switch (cmd)
			{
			case MAP_ROOM:
			{
				const auto mapId = static_cast<uint8_t>(value);

				// Note: So, switching difficulty in a PvE room triggers a change in the mapId
				// and thus if we do allow switching difficulties we need to take that into account. 
				if (room->getMode() == GameMode::Mode::PVE)
				{
					return;
				}

				// FIXME: We probably need to add a gamemode check to ensure a player can set a certain mapId.

				room->setMap(mapId);
			}
			break;
			case MODE_ROOM:
			{
				const auto isValidMode =
					mode == GameMode::Mode::DM ||
					mode == GameMode::Mode::TDM ||
					mode == GameMode::Mode::PTE ||
					mode == GameMode::Mode::VIP ||
					mode == GameMode::Mode::PREY ||
					mode == GameMode::Mode::PVE;

				if (!isValidMode)
				{
					conn->postNetEvent(new GCRoom(player->getId(), MODE_ROOM, room));

					player->broadcast(u"Sorry, but this game mode has not been implemented yet");

					break;
				}

				room->setMode(mode);

				if (room->getMode() == GameMode::PREY)
				{
					room->setSkillsEnabled(false);

					break;
				}
				else
				{
					room->setSkillsEnabled(true);
				}
			}
			break;
			case SET_POINTS:
			{
				if (value == 10 || value == 15 || value == 20 || value == 25 || value == 30 || value == 35 || value == 40)
				{
					room->setScorePoints(value);
					room->setIsPointsGame(true);
				}
			}
			break;
			case SET_TIME:
			{
				if (value == 5 || value == 10 || value == 15 || value == 20)
				{
					room->setScoreTime(value);
					room->setIsPointsGame(false);
				}
			}
			break;
			case PLAYERS_ROOM:
			{
				if (value == 4 || value == 8 || value == 12 || value == 16)
				{
					room->setMaxPlayers(value);
				}
			}
			break;
			case PASS_ROOM:
			{
				room->setPassword(password);
			}
			break;
			case LEVEL_ROOM:
			{
				room->setLevelLimited(value);
			}
			break;
			case TOGGLE_MELEE:
			{
				room->setMeleeOnly(value);
				room->setSkillsEnabled(false);
			}
			break;
			case TOGGLE_SKILL:
			{
				if (room->getMode() == GameMode::PREY)
				{
					room->setSkillsEnabled(false);

					break;
				}

				room->setSkillsEnabled(!room->isSkillsEnabled());

				break;
			}
			break;
			case TEAM_ROOM:
			{
				room->setTeamSorting(value);
			}
			break;
			default:
				std::cout << "CGRoom::handle >> Unknown cmd: " << cmd << std::endl;
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
	U32 cmd = CREATE_ROOM; //96

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