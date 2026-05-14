#include "tasks.h"

#include <chrono>
#include <iostream>
#include <rxcpp/rx.hpp>
#include <string>
#include <thread>
#include <vector>

namespace
{
using namespace std::chrono_literals;

struct Keystroke
{
	std::string text;
	int delayAfterMs;
};

std::vector<Keystroke> getKeystrokes()
{
	return {
		{"К", 50},
		{"Ки", 80},
		{"Киї", 120},
		{"Київ", 400},
		{"Київ ", 60},
		{"Київ К", 60},
		{"Київ Ки", 350}
	};
}
}

void runTask6_1()
{
	const auto keystrokes = getKeystrokes();

	auto source = rxcpp::observable<>::create<std::string>(
		[keystrokes](rxcpp::subscriber<std::string> subscriber) {
			for (const auto& keystroke : keystrokes) {
				subscriber.on_next(keystroke.text);
				std::this_thread::sleep_for(std::chrono::milliseconds(keystroke.delayAfterMs));
			}

			subscriber.on_completed();
		});

	source
		.subscribe_on(rxcpp::synchronize_new_thread())
		.debounce(300ms, rxcpp::observe_on_new_thread())
		.as_blocking()
		.subscribe([](const std::string& query) {
			std::cout << "[ПОШУК] Запит до API: \"" << query << "\"\n";
		});
}
