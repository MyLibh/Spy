#include "Espionage.hpp"

#include <cstdlib>   // std::rand, std::srand
#include <thread>    // std::this_thread::sleep_for
#include <chrono>    // std::chrono::seconds
#include <Windows.h> // Beep
#include <algorithm> // std::for_each

void IntelligenceAgency::listen(RWAVE wave)
{
	while (/* !wave.empty() */ true)
	{
		std::cout << "[" << __FUNCTION__ << "] get '" << morse2char(wave.front()) << "'\n";
		
		std::for_each(wave.front().cbegin(), wave.front().cend(), [] (char c) { if (c == '.') Beep(500, 300); else if (c == '-') Beep(500, 750); else std::cout << "Undefined symbol: " << c << std::endl; });
	
		Sleep(500);

		wave.pop();
	}
	Beep(500, 2000);

	std::cout << "[" << __FUNCTION__ << "] finished working\n";
}

void Spy::generateInfo(RWAVE wave, size_t freq /* = FREQ */)
{
	std::srand(static_cast<unsigned int>(time(NULL)));
	auto time = std::rand() % 5000;
	while (time--)
	{
		char c = std::rand() % 26 + 'A';

		std::cout << "[" << __FUNCTION__ << "] pushing '" << c << "'\n";

		wave.push(std::move(char2morse(c)));

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	std::cout << "[" << __FUNCTION__ << "] finished working\n";
}

inline std::string char2morse(char c)
{
	return ALPHABET.left.find(c)->second;
}

inline char morse2char(const std::string &str)
{
	return ALPHABET.right.find(str)->second;
}