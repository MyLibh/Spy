#pragma once

#include <boost\config.hpp>

#include <boost\assign\list_of.hpp>       // boost::assign::list_of
#include <boost\assign\list_inserter.hpp>

#include <boost\bimap\bimap.hpp>       // boost::bimaps::bimap
#include <boost\bimap\multiset_of.hpp> // boost::bimaps::multiset_of
#include <boost\bimap\list_of.hpp>     // boost::bimaps::list_of

#include <experimental\filesystem> // std::filesystem

#include "Queue.hpp"

typedef boost::bimaps::bimap<char, std::string>  morse_t;
typedef morse_t::value_type                       letter;

typedef NQueue::Queue<std::string, 10>   WAVE;
typedef WAVE                           &RWAVE;

constexpr size_t FREQ = 1;

const morse_t ALPHABET = boost::assign::list_of<letter>
#pragma region Letters

('A', ".-")
('B', "-...")
('C', "-.-.")
('D', "-..")
('E', ".")
('F', "..-.")
('G', "--.")
('H', "....")
('I', "..")
('J', ".---")
('K', "-.-")
('L', ".-..")
('M', "--")
('N', "-.")
('O', "---")
('P', ".--.")
('Q', "--.-")
('R', ".-.")
('S', "...")
('T', "-")
('U', "..-")
('V', "...-")
('W', ".--")
('X', "-..-")
('Y', "-.--")
('Z', "--..")

#pragma endregion

#pragma region Digits

('0', "-----")
('1', ".----")
('2', "..---")
('3', "...--")
('4', "....-")
('5', ".....")
('6', "-....")
('7', "--...")
('8', "---..")
('9', "----.")

#pragma endregion

#pragma region Special

('.', ".-.-.-")
(',', "--..--")
('!', "-.-.--")
('?', "..--..")
('\'', ".----.")
('"', ".-..-.")
(';', "-.-.-.")
(':', "---...")
('+', ".-.-.")
('=', "-...-")
('_', "..--.-")
('/', "-..-.")
('(', "-.--.")
(')', "-.--.-")
('&', ".-...")
('$', "...-..-")
('@', ".--.-.")
(' ', " ")
('\0', "..-.-")
('\\', "........")

#pragma endregion

;

class IntelligenceAgency
{
public:
	static void listen(RWAVE);
};

typedef IntelligenceAgency IntelAgency;

class Spy
{
public:
	static void generateInfo(RWAVE, size_t = FREQ);
	static void sendInfo(RWAVE);
	static void sendFrom(RWAVE, const std::experimental::filesystem::path&);
};

inline std::string char2morse(char);
inline char        morse2char(const std::string&);