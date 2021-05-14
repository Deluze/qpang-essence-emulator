#include "Authenticator.h"

#include "core/Emulator.h"

uint32_t main()
{	
	Emulator::instance()->initialize();

	Authenticator auth{};

	Emulator::instance()->run();

	return 0;
}