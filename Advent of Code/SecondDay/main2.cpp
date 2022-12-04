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

auto calculateRound(std::string const& round, size_t idx) -> std::pair <uint16_t, uint16_t>
{
	//1 + 1 |= 3
	//1 + 2 += 6
	//1 + 3 /= 0

	//2 + 1 /= 0
	//2 + 2 |= 3
	//2 + 3 += 6

	//3 + 1 += 6
	//3 + 2 /= 0
	//3 + 3 |= 3

	auto moveMap	= std::map <char, uint16_t> { {'A', 1}, {'B', 2}, {'C', 3}, {'X', 1}, {'Y', 2}, {'Z', 3} };
	auto win		= std::vector <uint16_t> { 2, 3, 1 };
	auto lose		= std::vector <uint16_t> { 3, 1, 2 };
	auto score		= std::pair <uint16_t, uint16_t>{moveMap[round[2]], 0};

	enum class Score : uint16_t
	{
		Lose = 0,
		Draw = 3,
		Win	 = 6,
	};

	if (moveMap[round[0]] == moveMap[round[2]])
		std::get <1>(score) = (uint16_t)Score::Draw;
	else
	{
		if (win[moveMap[round[2]] - 1] == moveMap[round[0]])
			std::get <1>(score) = uint16_t(Score::Lose);
		else if(lose[moveMap[round[2]]-1] == moveMap[round[0]])
			std::get <1>(score) = uint16_t(Score::Win);
	}

	auto returnVerdict = [&score]() -> std::string_view
	{
		std::array <std::string_view, 3> verdict {"lose", "draw", "win"};
		if (score.second == 0)
			return verdict[0];
		else if (score.second == 3)
			return verdict[1];
		else
			return verdict[2];
	};

	std::cout << "Round " << idx << " {" << moveMap[round[0]] << ", " << moveMap[round[2]] << "} so it's " << returnVerdict() << "\n";
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
		std::cout << "Score: " << scorePair.first + scorePair.second << "\n";
		std::cin.get();
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