#include "tasks.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <rxcpp/rx.hpp>
#include <string>
#include <thread>
#include <vector>

namespace
{
const std::vector<std::string> matchResults = {
	"Динамо 2:1 Шахтар",
	"Шахтар 3:0 Металіст",
	"Дніпро-1 1:1 Зоря",
	"Карпати 0:2 Полісся",
	"Чорноморець 2:2 Ворскла"
};

void printMatch(const std::string& subscriberName, const std::string& match)
{
	std::cout << subscriberName << " отримав: " << match << '\n';
}
}

void runTask2_2()
{
	std::cout << "Частина A — Холодний Observable\n";

	const auto coldSeasonResults = rxcpp::observable<>::iterate(matchResults);

	std::cout << "Підписник 1 підписався:\n";
	coldSeasonResults
		.as_blocking()
		.subscribe([](const std::string& match) {
			printMatch("Підписник 1", match);
		});

	std::cout << '\n';
	std::cout << "Підписник 2 підписався:\n";
	coldSeasonResults
		.as_blocking()
		.subscribe([](const std::string& match) {
			printMatch("Підписник 2", match);
		});

	std::cout << "\nЧастина B — Гарячий Observable\n";

	const auto currentMatchIndex = std::make_shared<std::size_t>(0);
	const auto liveSource = rxcpp::observable<>::interval(
			std::chrono::milliseconds(700),
			rxcpp::observe_on_new_thread())
		.take(static_cast<int>(matchResults.size()))
		.map([currentMatchIndex](long) {
			const auto index = (*currentMatchIndex)++;
			return matchResults.at(index);
		});

	auto liveResults = liveSource.publish();

	liveResults.subscribe([](const std::string& match) {
		printMatch("Підписник 1", match);
	});

	auto connection = liveResults.connect();

	std::thread lateSubscriber([&liveResults] {
		std::this_thread::sleep_for(std::chrono::seconds(2));
		std::cout << "Підписник 2 підключився із затримкою 2 секунди\n";

		liveResults.subscribe([](const std::string& match) {
			printMatch("Підписник 2", match);
		});
	});

	liveResults.as_blocking().subscribe();
	lateSubscriber.join();
	connection.unsubscribe();
}
