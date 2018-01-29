#include "Espionage.hpp"

#include <cstdlib>   // std::rand, std::srand
#include <thread>    // std::this_thread::sleep_for
#include <chrono>    // std::chrono::seconds
#include <Windows.h> // Beep
#include <algorithm> // std::for_each
#include <fstream>   // std::fstream

void IntelligenceAgency::listen(RWAVE wave)
{
	while (char c = morse2char(wave.front()))
	{
		std::cout << "[" << __FUNCTION__ << "] get '" << c << "'\n";
		
		std::for_each(wave.front().cbegin(), wave.front().cend(), [] (char c) { if (c == '.') Beep(500, 300); else if (c == '-') Beep(500, 750); else std::cout << "Undefined symbol: " << c << std::endl; });
	
		Sleep(500);

		wave.pop();

		if (c == '\0') break;
	}
	Beep(500, 2000);

	std::cout << "[" << __FUNCTION__ << "] finished working\n";
}

void Spy::generateInfo(RWAVE wave, size_t freq /* = FREQ */)
{
	std::srand(static_cast<unsigned int>(time(NULL)));

	auto time = std::rand() % 50;
	while (time--)
	{
		char c = std::rand() % 26 + 'A';

		std::cout << "[" << __FUNCTION__ << "] push '" << c << "'\n";

		wave.push(std::move(char2morse(c)));

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	wave.push(std::move(char2morse('\0')));

	std::cout << "[" << __FUNCTION__ << "] finished working\n";
}

void Spy::sendInfo(RWAVE wave)
{
	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		char c = char();
		std::cin >> c;

		std::cout << "[" << __FUNCTION__ << "] push '" << c << "'\n";

		wave.push(char2morse(c));
	}

	std::cout << "[" << __FUNCTION__ << "] finished working\n";
}

void Spy::sendFrom(RWAVE wave, const std::experimental::filesystem::path &crPath)
{
	std::ifstream input(crPath.native());
	if (!input.is_open())
	{
		std::string error("Cannot open file: ");
		error += *crPath.c_str();
		NDebugger::Error(error);

		return;
	}

	while (!input.eof())
	{
		char c = char();
		input >> c;

		std::cout << "[" << __FUNCTION__ << "] push '" << c << "'\n";

		wave.push(char2morse(c));
	}
	input.close();

	std::cout << "[" << __FUNCTION__ << "] finished working\n";
}

inline std::string char2morse(char c)
{
	return ALPHABET.left.find(c)->second; // Check for exist
}

inline char morse2char(const std::string &str)
{
	return ALPHABET.right.find(str)->second; // Check for exist
}