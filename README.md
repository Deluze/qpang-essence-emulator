# QPang Essence Emulator

This is the first ever QPang server emulator that is built around the 2012/03/14 revision of the client (20120314), but should also support newer versions.

The emulator has been used by "Project Essence" or "QPang.io" and has been used for over 7 months while handling over hundreds of players concurrently.

Join the community for support/questions/chatting: [Discord](https://discord.gg/2AMAZEt).

By: [Dennis](https://github.com/kuroppoi) & [Deluze](https://github.com/Deluze) ♥.

## Features

The goal of this emulator is to emulate all of QPang's original functionality as how it was intended.

It conists out of:

- **Event relays**: Matches won't be p2p anymore, and every event is now relayed between players on the server-side, yay!
- **User Authentication** (with bcrypt hashed passwords, yes, this server doesn't use plain text passwords. :^))
- **Fully working inventory/character customization** (besides name changers & quick revive)
- **Friends**
- **Chat, Whispers & In-Game Announcements**
- **Bans & Mutes**
- **Gamerooms**
  - **TDM, DM, Essence & VIP game modes**
  - **Melee only**
  - **Power-ups**
  - **Team balancing**
  - **Ensures "quality" matches**, prevents players from joining late game and losing items
  - **Most achievements are implemented** (We don't know what the requirements of some achievements are)
  - **Points & Time based matches**
  - **Cash & Coin drops**
  - **Weapon effects**
- **Custom in-game commands**
- **Configurable crane machine**
- **Configurable shop**
- **In-game leaderboards**
- **Automatic square generation so they don't get overcrowded** (50 players max, configurable)

Missing features:

- PvE gamemode
- Memos
- Game invites
- Trading
- Registration in-game
- Skill cards
- Card boosting/enhancing (purposefully unimplemented, design choice)
- Daily login bonus
- In-game quests

## Installation

### Prerequisites:
You should have the following pieces of software installed:

- [CMake](https://cmake.org/download/) (add to PATH)
- [vcpkg](https://github.com/microsoft/vcpkg) (with user wide integration & add to PATH)
- [MySQL C++ Connector](https://dev.mysql.com/doc/connector-cpp/8.0/en/connector-cpp-installation-binary.html#connector-cpp-installation-binary-windows) (in default path)
- [MySQL server](https://dev.mysql.com/downloads/mysql/)

### Installation steps:
Assuming you have all the prerequisities listed above.

```bash
git clone https://github.com/Deluze/qpang-essence-emulator
cd qpang-essence-emulator

vcpkg install spdlog
vcpkg install boost

mkdir build
cd build

# if < VS 2019
cmake ../
# if > VS 2019
cmake ../ -A Win32
```

After running these steps a visual studio solution should be generated in the build directory and you can open up the project. Open the solution and set the start up projects to `EssenceAuth` and `EssenceGame`.

You can now start your build, the first build should take a while, give it some time. 

If you start the server for the first time it will throw an error that the configuration file is missing and that it created one for you.

### Creating the database
Create a database and execute a query with the contents of `qpang.sql` file in the `database` with your favorite MySQL client.

### Configuring the server

You can change all of these variables in `server.conf` in the root of the binaries. These variables will be loaded in on start up, so if you change any of them you will have to reboot the server.

```bash
HOST # IP address of host of the server

SQUARE_PREFIX # Name of generated square example: Square 1
SQUARE_CAPACITY # Max player count of a square

GAME_REVISION # Client revision of game, needs to match
GAME_WHITELIST # Users needs to be whitelisted in the database
GAME_PORT # port that is used to host the gamerooms server on
GAME_ENABLED # if 0 -> nobody can create games, if 1 -> everyone can create games
GAME_MAX_ROOMS # maximum amount of rooms

DB_HOST # IP address of database server
DB_PORT # port of database server
DB_NAME # name of database server
DB_USER # user of database server
DB_PASS # password of database server
```

### Registering users
This is something you will have to do yourself, create a script, a back-end, form or anything that will insert the correct data into the database (check `users`, `players`, `player_equipement`, `player_stats` tables in the database).

On executing the database script a default user 'admin' was created. You can login with the following credentials: 

User => `admin` Password => `admin`
