#include "tasks.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <rxcpp/rx.hpp>
#include <string>
#include <thread>
#include <vector>

namespace
{
using namespace std::chrono_literals;

struct ServiceCall
{
	std::string serviceName;
	int delayMs;
	std::string threadName;
};

std::vector<ServiceCall> getServices()
{
	return {
		{"UserService", 800, "io-1"},
		{"OrderService", 1200, "io-2"},
		{"RecommendationService", 600, "io-3"}
	};
}

std::string callService(const ServiceCall& service)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(service.delayMs));

	return "[" + service.threadName + "] (+) " + service.serviceName
		+ " відповів за " + std::to_string(service.delayMs) + " мс";
}

long long elapsedMs(std::chrono::steady_clock::time_point start)
{
	const auto end = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}
}

void runTask5_2()
{
	const auto services = getServices();

	std::cout << "Частина A — послідовно через concatMap\n";
	auto sequentialStart = std::chrono::steady_clock::now();

	rxcpp::observable<>::iterate(services)
		.concat_map([](const ServiceCall& service) {
			return rxcpp::observable<>::just(service)
				.map(callService);
		})
		.as_blocking()
		.subscribe([](const std::string& response) {
			std::cout << response << '\n';
		});

	std::cout << "Загальний час (послідовно): ~" << elapsedMs(sequentialStart) << " мс\n";

	std::cout << "\nЧастина B — паралельно через flatMap + Schedulers.io()\n";
	auto parallelStart = std::chrono::steady_clock::now();

	rxcpp::observable<>::iterate(services)
		.flat_map([](const ServiceCall& service) {
			return rxcpp::observable<>::just(service)
				.subscribe_on(rxcpp::synchronize_new_thread())
				.map(callService);
		})
		.as_blocking()
		.subscribe([](const std::string& response) {
			std::cout << response << '\n';
		});

	std::cout << "Загальний час (паралельно): ~" << elapsedMs(parallelStart) << " мс\n";
}
