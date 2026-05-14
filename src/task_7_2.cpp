#include "tasks.h"

#include <atomic>
#include <chrono>
#include <exception>
#include <iostream>
#include <rxcpp/rx.hpp>
#include <stdexcept>
#include <string>
#include <thread>

namespace
{
using namespace std::chrono_literals;

rxcpp::observable<std::string> unstableApiCall(std::atomic<int>& attemptCount)
{
	return rxcpp::observable<>::create<std::string>(
		[&attemptCount](rxcpp::subscriber<std::string> subscriber) {
			const int attempt = ++attemptCount;
			std::cout << "[ПОВТОР] Спроба #" << attempt << '\n';

			if (attempt < 4) {
				subscriber.on_error(std::make_exception_ptr(std::runtime_error("Connection timeout")));
				return;
			}

			subscriber.on_next("(+) Відповідь API: {status: 'ok', data: [...]}");
			subscriber.on_completed();
		}).as_dynamic();
}

rxcpp::observable<std::string> retryWithExponentialBackoff(std::atomic<int>& attemptCount)
{
	return rxcpp::observable<>::create<std::string>(
		[&attemptCount](rxcpp::subscriber<std::string> subscriber) {
			for (int attempt = 1; attempt <= 4; ++attempt) {
				bool completed = false;
				std::exception_ptr lastError;

				unstableApiCall(attemptCount)
					.as_blocking()
					.subscribe(
						[&subscriber](const std::string& response) {
							subscriber.on_next(response);
						},
						[&lastError](std::exception_ptr error) {
							lastError = error;
						},
						[&completed] {
							completed = true;
						});

				if (completed) {
					subscriber.on_completed();
					return;
				}

				if (attempt == 4) {
					subscriber.on_error(lastError);
					return;
				}

				const int waitSeconds = 1 << (attempt - 1);
				std::cout << "Очікуємо " << waitSeconds << " сек перед повтором...\n";
				std::this_thread::sleep_for(std::chrono::seconds(waitSeconds));
			}
		}).as_dynamic();
}
}

void runTask7_2()
{
	std::atomic<int> attemptCount = 0;

	retryWithExponentialBackoff(attemptCount)
		.as_blocking()
		.subscribe(
			[](const std::string& response) {
				std::cout << response << '\n';
			},
			[](std::exception_ptr error) {
				try {
					if (error) {
						std::rethrow_exception(error);
					}
				} catch (const std::exception& exception) {
					std::cout << "(-) Помилка після 4 спроб: " << exception.what() << '\n';
				}
			});
}
