#include "tasks.h"

#include <exception>
#include <iostream>
#include <rxcpp/rx.hpp>
#include <stdexcept>
#include <string>

namespace
{
using SingleString = rxcpp::observable<std::string>;

SingleString getUserById(int id)
{
	if (id > 0) {
		return rxcpp::observable<>::just("Користувач #" + std::to_string(id) + ": Іван Франко");
	}

	return rxcpp::observable<>::error<std::string>(
		std::make_exception_ptr(std::invalid_argument("ID не може бути від'ємним або нульовим")));
}

void printError(const std::string& prefix, std::exception_ptr error)
{
	try {
		if (error) {
			std::rethrow_exception(error);
		}
	} catch (const std::exception& exception) {
		std::cout << prefix << exception.what() << '\n';
	}
}
}

void runTask4_1()
{
	getUserById(42)
		.as_blocking()
		.subscribe(
			[](const std::string& user) {
				std::cout << "(+) Знайдено: " << user << '\n';
			},
			[](std::exception_ptr error) {
				printError("(+) Помилка: ", error);
			});

	getUserById(-1)
		.as_blocking()
		.subscribe(
			[](const std::string& user) {
				std::cout << "(-) Знайдено: " << user << '\n';
			},
			[](std::exception_ptr error) {
				printError("(-) Помилка: ", error);
			});
}
