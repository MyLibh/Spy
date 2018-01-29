//#define GUARD_LVL 3

#include <thread> // std::thread

#include "Espionage.hpp"

using namespace NQueue;

int main(int argc, char *argv[])
{
	std::ios_base::sync_with_stdio(false);

	WAVE wave;
	std::thread intelAgency(&IntelAgency::listen, std::ref(wave));
	std::thread spy(&Spy::generateInfo, std::ref(wave), FREQ);
	

	spy.detach();
	intelAgency.join();
	wave.dump();

	system("pause");
	return 0;
}