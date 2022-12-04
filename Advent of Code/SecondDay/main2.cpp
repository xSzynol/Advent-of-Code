#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>
#include <string>
#include <exception>
#include <algorithm>
#include <map>

void openFile(std::string_view filePath, std::ifstream& file, std::vector<std::string>& fileContainer)
{ // Not std::string_view*, because I know it will be not necessary for three chars.
	file = std::ifstream(filePath.data());
	if (!file.is_open())
	{
		throw std::runtime_error("File not found");
	}

	auto currentLine	= std::string();
	while (std::getline(file, currentLine))
		fileContainer.push_back(currentLine);
		
	file.close();
}

struct Verdict
{
	public:
	enum class Score : uint16_t
	{
		Lose = 0,
		Draw = 3,
		Win = 6,
	};

	std::map <char, Score> verConversion = { {'X', Score::Lose}, {'Y', Score::Draw}, {'Z', Score::Win} };
	std::map <Score, uint16_t> scoreToIdx = { {Score::Win, 0}, {Score::Lose, 1} };
	std::vector <uint16_t> win = { 2, 3, 1 };
	std::vector <uint16_t> lose = { 3, 1, 2 };
	std::array <std::vector <uint16_t>, 2> wlArray =  {win, lose};

	Verdict() = default;
};

auto calculateRound(std::string const& round, size_t idx) -> std::pair <uint16_t, uint16_t>
{
	auto moveMap		= std::map <char, uint16_t> { {'A', 1}, {'B', 2}, {'C', 3} };
	auto verdict		= std::make_unique <Verdict> ();
	auto score			= std::pair <uint16_t, uint16_t> { 0, uint16_t(verdict->verConversion[round[2]])}; // Adding knew-result of the clash into the second data of pair.

	using VS = Verdict::Score;
	if (verdict->verConversion[round[2]] == VS::Draw)
		std::get <0>(score) = moveMap[round[0]]; // If we are having draw, let's add equal result to our score-pair.
	else
		std::get <0>(score)	= verdict->wlArray[ verdict->scoreToIdx[ verdict->verConversion [ round[2] ] ] ][ moveMap[ round[0] ] - 1 ];
		// Let's convert 'XYZ' into enumerators (Win, Lose etc), and then enums to indexes of win/loss array, then check one of the vectors for appropriate move.

	auto returnVerdict = [&verdict, &round]() -> std::string_view
	{
		auto vString		= std::array <std::string_view, 3> {"lose", "draw", "win"};
		auto verdictMapIdx	= std::map <VS, uint16_t>{ {VS::Lose, 0}, {VS::Draw, 1}, {VS::Win, 2} };
		return vString[verdictMapIdx[verdict->verConversion[round[2]]]];
	};

	std::cout << "Round " << idx << " {" << moveMap[round[0]] << ", " << returnVerdict() << "} so it has " << score.first + score.second << "points. \n";
	return score;
}

auto matchRound(std::vector <std::string>& fileContainer) -> size_t
{
	auto scoreSum	= size_t(0);
	auto scorePair	= std::pair <uint16_t, uint16_t>();
	auto idx		= size_t(0);

	for (auto const& round : fileContainer)
	{
		scorePair	= calculateRound(round, ++idx);
		scoreSum	+= size_t(scorePair.first + scorePair.second);
		std::cout << "Score: " << scorePair.first + scorePair.second << " Max Score: " << scoreSum << "\n";
	}

	return scoreSum;
}

auto main() -> int
{
	auto constexpr filePath	= std::string_view("C:\\Users\\Testo\\source\\repos\\AoC\\Advent Of Code\\Advent Of Code\\SecondDay\\Puzzle-Day2.txt");
	auto file				= std::ifstream();
	auto fileContainer		= std::vector <std::string> ();

	try
	{
		openFile(filePath, file, fileContainer);
	} catch (std::exception& e)
	{
		std::cerr << e.what() << '\n';
		std::cin.get();
		return 1;
	}

	std::cout << "Counting score..." << "\n";
	auto score = matchRound(fileContainer);
	std::cout << "Total score: " << score << '\n';
	std::cin.get();
	return 0;
}