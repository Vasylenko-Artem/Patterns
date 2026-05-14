#include "tasks.h"

#include <iostream>
#include <rxcpp/rx.hpp>
#include <stdexcept>
#include <string>

namespace
{
rxcpp::observable<std::string> createCurrencyService()
{
	return rxcpp::observable<>::create<std::string>([](rxcpp::subscriber<std::string> subscriber) {
		subscriber.on_next("USD -> UAH: 41.50");
		subscriber.on_next("EUR -> UAH: 44.20");
		subscriber.on_error(std::make_exception_ptr(std::runtime_error("Сервіс тимчасово недоступний")));
		subscriber.on_next("GBP -> UAH: 52.10");
		subscriber.on_completed();
	}).as_dynamic();
}
}

void runTask7_1()
{
	std::cout << "Сценарій A — onErrorReturn\n";

	createCurrencyService()
		.on_error_resume_next([](std::exception_ptr) {
			return rxcpp::observable<>::just(std::string("Використовується кешований курс: USD -> UAH: 41.00"));
		})
		.as_blocking()
		.subscribe([](const std::string& rate) {
			std::cout << rate << '\n';
		});

	std::cout << "\nСценарій B — onErrorResumeNext\n";

	createCurrencyService()
		.on_error_resume_next([](std::exception_ptr) {
			return rxcpp::observable<>::from(
				std::string("JPY -> UAH: 0.27"),
				std::string("PLN -> UAH: 10.30"));
		})
		.as_blocking()
		.subscribe([](const std::string& rate) {
			std::cout << rate << '\n';
		});
}
