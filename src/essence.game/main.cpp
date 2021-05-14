#include <cstdint>

#include "qpang/room/tnl/net_events/EventIncludes.h"

#include "core/Emulator.h"
#include "qpang/Game.h"

uint32_t main()
{
	Emulator::instance()->initialize();

	Game::instance()->initialize();

	Game::instance()->run();

	Emulator::instance()->run();
}