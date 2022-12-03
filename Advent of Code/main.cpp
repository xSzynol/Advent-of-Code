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

class elf
{
	std::vector <uint32_t> puzzles;

	protected:
	uint32_t calories = 0;

	public:
	size_t idx = 0;
	uint32_t getCalories()
	{ return calories; }
	
	elf() = default;
	elf(size_t promptedIdx, std::vector <uint32_t> ctorPuzzles) : idx(promptedIdx), puzzles(ctorPuzzles)
	{
		auto sum = [](auto vec, auto& calories) -> size_t
		{
			for (auto const& calory : vec)
				calories += calory;

			return calories;
		};

		std::cout << "Elf no " << 
			this->idx << " has " << 
			this->puzzles.size() << " amount of puzzle and " << 
			sum(puzzles, calories) << " amount of calories. \n";
	}
};

namespace bestElves
{
	static inline elf bestElf;
}

auto findAnotherElf(std::ifstream& file, size_t elfIdx) -> std::optional <elf> 
{
	auto currentLine	= std::string();
	auto currIdx		= size_t(0);
	auto puzzles		= std::vector <uint32_t> ();
	auto atDestination	= bool(false);

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
				puzzles.push_back(uint32_t(stoi(currentLine)));
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
{
	auto saveElf = [&currentElf]() -> elf { return currentElf; };

	if (currentElf.getCalories() > bestElves::bestElf.getCalories())
	{
		auto pair = saveElf();
		bestElves::bestElf = pair;
	}
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
	std::cout << "Zliczanie ktory elf ma najwieksza ilosc kcali... \n";
	auto file = std::ifstream();

	try
	{
		file.open("C:\\Users\\Testo\\source\\repos\\AoC\\Advent Of Code\\Advent Of Code\\Puzzle-Day1.txt", std::ios::in);

		if (!file.is_open())
		{
			throw std::runtime_error("File not found");
		}
	} catch (std::exception& e)
	{
		std::cerr << e.what() << '\n';
		std::cin.get();
		std::terminate();
	}

	checkElves(file);
	std::cout << "Maks kalorii: " << bestElves::bestElf.getCalories();
	std::cout << "\nPrzy id elfa: " << bestElves::bestElf.idx;
	std::cin.get();
	file.close();
}
