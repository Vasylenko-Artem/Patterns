#include "tasks.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <ranges>
#include <rxcpp/rx.hpp>
#include <string>
#include <utility>
#include <vector>

namespace
{
	const std::vector<std::string> cities = {
	    "Київ",
	    "Харків",
	    "Одеса",
	    "Дніпро",
	    "Кривий Ріг",
	    "Миколаїв",
	    "Херсон",
	    "Черкаси",
	    "Суми",
	    "Хмельницький",
	    "Запоріжжя",
	    "Кропивницький",
	    "Чернівці",
	    "Каховка"};

	bool startsWithK(const std::string &city)
	{
		return city.rfind("К", 0) == 0;
	}

	void replaceAll(std::string &text, const std::string &from, const std::string &to)
	{
		std::size_t position = 0;

		while ((position = text.find(from, position)) != std::string::npos)
		{
			text.replace(position, from.length(), to);
			position += to.length();
		}
	}

	std::string toUpperUkrainian(std::string text)
	{
		for (char &character : text)
		{
			const auto value = static_cast<unsigned char>(character);
			if (value < 128)
			{
				character = static_cast<char>(std::toupper(value));
			}
		}

		const std::vector<std::pair<std::string, std::string>> replacements = {
		    {"а", "А"}, {"б", "Б"}, {"в", "В"}, {"г", "Г"}, {"ґ", "Ґ"}, {"д", "Д"}, {"е", "Е"}, {"є", "Є"}, {"ж", "Ж"}, {"з", "З"}, {"и", "И"}, {"і", "І"}, {"ї", "Ї"}, {"й", "Й"}, {"к", "К"}, {"л", "Л"}, {"м", "М"}, {"н", "Н"}, {"о", "О"}, {"п", "П"}, {"р", "Р"}, {"с", "С"}, {"т", "Т"}, {"у", "У"}, {"ф", "Ф"}, {"х", "Х"}, {"ц", "Ц"}, {"ч", "Ч"}, {"ш", "Ш"}, {"щ", "Щ"}, {"ь", "Ь"}, {"ю", "Ю"}, {"я", "Я"}};

		for (const auto &[lowercase, uppercase] : replacements)
		{
			replaceAll(text, lowercase, uppercase);
		}

		return text;
	}

	std::vector<std::string> imperativeApproach()
	{
		std::vector<std::string> result;

		for (const auto &city : cities)
		{
			if (startsWithK(city))
			{
				result.push_back(toUpperUkrainian(city));
			}
		}

		std::ranges::sort(result);
		return result;
	}

	std::vector<std::string> functionalApproach()
	{
		auto processedCities = cities | std::views::filter(startsWithK) | std::views::transform(toUpperUkrainian);

		std::vector<std::string> result(processedCities.begin(), processedCities.end());
		std::ranges::sort(result);

		return result;
	}

	std::vector<std::string> reactiveApproach()
	{
		std::vector<std::string> result;

		rxcpp::observable<>::iterate(cities)
		    .filter(startsWithK)
		    .map(toUpperUkrainian)
		    .as_blocking()
		    .subscribe([&result](const std::string &city)
			       { result.push_back(city); });

		std::ranges::sort(result);
		return result;
	}

	void printResult(const std::string &title, const std::vector<std::string> &result)
	{
		std::cout << title << ":\n";

		for (const auto &city : result)
		{
			std::cout << city << '\n';
		}
	}
}

void runTask1_2()
{
	printResult("Імперативний підхід", imperativeApproach());
	std::cout << '\n';

	printResult("Функціональний підхід", functionalApproach());
	std::cout << '\n';

	printResult("Реактивний підхід", reactiveApproach());
}
