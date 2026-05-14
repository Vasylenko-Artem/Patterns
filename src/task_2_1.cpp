#include "tasks.h"

#include <iostream>
#include <rxcpp/rx.hpp>
#include <string>
#include <vector>

void runTask2_1()
{
	const std::vector<std::string> atmSteps = {
		"Вставте картку",
		"Введіть PIN-код",
		"Оберіть суму: 500 грн",
		"Видача готівки...",
		"Дякуємо! Заберіть картку"
	};

	std::cout << "[БАНКОМАТ] Сесію розпочато\n";

	rxcpp::observable<>::iterate(atmSteps)
		.as_blocking()
		.subscribe(
			[](const std::string& step) {
				std::cout << ">> " << step << '\n';
			},
			[](std::exception_ptr error) {
				try {
					if (error) {
						std::rethrow_exception(error);
					}
				} catch (const std::exception& exception) {
					std::cout << "[БАНКОМАТ] Помилка: " << exception.what() << '\n';
				}
			},
			[] {
				std::cout << "[БАНКОМАТ] Сесію завершено\n";
			});
}
