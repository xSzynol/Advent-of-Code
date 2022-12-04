//I know it could be faster - all the file copied to stringstream and then seek 
//through it not using getline() func, but I'm too lazy!

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <array>
#include <algorithm>
#include <string>
#include <exception>
#include <thread>
#include <chrono>
#include <optional>
#include <numeric>
#include <algorithm>

class elf
{
	std::vector <uint32_t> puzzles;

	protected:
	uint32_t calories = 0;

	public:
	size_t idx = 0;
	uint32_t getCalories() const
	{ return calories; }
	
	elf() = default;
	elf(size_t promptedIdx, std::vector <uint32_t> ctorPuzzles) : idx(promptedIdx), puzzles(std::move(ctorPuzzles))
	{
		/*auto sum = [](auto vec, auto& calories) -> size_t // Depreciated, due to the std::accumulate.
		{
			for (auto const& calory : vec)
				calories += calory;

			return calories;
		};*/

		calories = std::accumulate(puzzles.begin(), puzzles.end(), 0);

		std::cout << "Elf no " << 
			this->idx << " has " << 
			this->puzzles.size() << " amount of puzzle and " << 
			calories << " amount of calories. \n";
	}
};

namespace bestElves
{
	std::vector <size_t> bestElf;
}

auto findAnotherElf(std::ifstream& file, size_t elfIdx) -> std::optional <elf> 
{
	auto currentLine	= std::string();
	auto currIdx		= size_t(0);
	auto puzzles		= std::vector <uint32_t> ();
	auto atDestination	= false;

	while (std::getline(file, currentLine)) // Iterating through entire file is slow, but I'm too lazy for that.
	{
		if (currentLine == "" && atDestination == false)
		{
			if (elfIdx > currIdx)
			{
				currIdx++;
				continue;
			}
			else if (elfIdx <= currIdx)
			{
				atDestination = true;
				continue;
			}
		}
		else if (atDestination == true)
		{
			if (currentLine != "")
				puzzles.push_back(uint32_t(std::stoi(currentLine)));
			else
				break;
		}
	}

	if (puzzles.size() == 0)
		throw std::logic_error("EOF");
	else
		return elf{ currIdx, puzzles };
}

void updateCalories(elf& currentElf) noexcept
{ // I could save all the elves and then sort them.
	#define bE bestElves::bestElf
	if (bE.size() < 3)
	{
		bE.push_back(currentElf.getCalories());
		return;
	}
	else if (bE.size() == 3)
		std::sort(bE.begin(), bE.end(), [](size_t i, size_t j) {return i > j; });

	if (currentElf.getCalories() > bE[0]) // Somehow slow and non-professional.
	{
		bE[2] = bE[1];
		bE[1] = bE[0];
		bE[0] = currentElf.getCalories();
	}

	if (currentElf.getCalories() > bE[1] && currentElf.getCalories() < bE[0])
	{
		bE[2] = bE[1];
		bE[1] = currentElf.getCalories();
	}

	if (currentElf.getCalories() > bE[2] && currentElf.getCalories() < bE[1])
		bE[2] = currentElf.getCalories();
	/*
	if (currentElf.getCalories() > bE[0])
	{
		bE.push_back(currentElf.getCalories());
		std::sort(bE.begin(), bE.end(), [](size_t i, size_t j) {return i > j; });
		bE.pop_back();
	}*/

	#undef bE
}

auto checkElves(std::ifstream& file)
{
	auto idx		= size_t(0);
	auto currElf	= std::optional <elf> {};

	while (true)
	{
		try
		{
			currElf = findAnotherElf(file, idx);
			file.seekg(std::ios_base::beg);
			updateCalories(currElf.value());
			idx++;
		} catch (std::exception& e)
		{
			std::cerr << e.what() << '\n';
			break;
		}
	}
}

auto main() -> int
{
	std::cout << "Counting which elf carrying meal that has the most calories... \n";
	auto file		= std::ifstream();
	auto filePath	= std::string_view("C:\\Users\\Testo\\source\\repos\\AoC\\Advent Of Code\\Advent Of Code\\FirstDay\\Puzzle-Day1.txt");

	try
	{
		file.open(filePath.data(), std::ios::in);

		if (!file.is_open())
		{
			throw std::runtime_error("File not found");
		}
	} catch (std::exception& e)
	{
		std::cerr << e.what() << '\n';
		std::cin.get();
		return 1;
	}

	checkElves(file);
	auto maxCalories = [](auto vec) -> size_t 
	{ 
		auto max = size_t(0); 
		for (auto _elf : vec) 
			max += _elf;
		
		return max;
	};

	std::cout << "First three elfes with most calories: " << maxCalories(bestElves::bestElf);
	std::cin.get();
	file.close();
	return 0;
}
