#include "tasks.h"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>
#include <rxcpp/rx.hpp>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

namespace
{
using namespace std::chrono_literals;

std::vector<std::string> getEvents()
{
	return {
		"LOGIN:user1",
		"CLICK:btn_buy",
		"VIEW:product_42",
		"LOGIN:user2",
		"LOGOUT:user1",
		"CLICK:btn_cart",
		"VIEW:product_7",
		"LOGIN:user3",
		"CLICK:btn_pay",
		"LOGOUT:user2",
		"LOGIN:user4",
		"VIEW:product_1"
	};
}

std::string joinBatch(const std::vector<std::string>& batch)
{
	std::ostringstream output;
	output << '[';

	for (std::size_t index = 0; index < batch.size(); ++index) {
		if (index > 0) {
			output << ", ";
		}

		output << batch[index];
	}

	output << ']';
	return output.str();
}

struct DropBackpressureReport
{
	int processed = 0;
	int dropped = 0;
};

DropBackpressureReport simulateDropBackpressure()
{
	constexpr int totalItems = 1000;
	constexpr std::size_t queueCapacity = 128;

	std::deque<int> queue;
	std::mutex mutex;
	std::condition_variable itemAvailable;
	bool producerDone = false;
	DropBackpressureReport report;

	std::thread consumer([&] {
		while (true) {
			int item = 0;

			{
				std::unique_lock<std::mutex> lock(mutex);
				itemAvailable.wait(lock, [&] {
					return producerDone || !queue.empty();
				});

				if (queue.empty() && producerDone) {
					break;
				}

				item = queue.front();
				queue.pop_front();
			}

			(void)item;
			std::this_thread::sleep_for(10ms);
			++report.processed;
		}
	});

	for (int item = 1; item <= totalItems; ++item) {
		{
			std::lock_guard<std::mutex> lock(mutex);

			if (queue.size() >= queueCapacity) {
				++report.dropped;
				continue;
			}

			queue.push_back(item);
		}

		itemAvailable.notify_one();
	}

	{
		std::lock_guard<std::mutex> lock(mutex);
		producerDone = true;
	}

	itemAvailable.notify_one();
	consumer.join();

	return report;
}
}

void runTask6_2()
{
	const auto events = getEvents();
	int savedEventsCount = 0;
	int batchNumber = 1;

	std::cout << "Частина A — buffer(5)\n";

	rxcpp::observable<>::iterate(events)
		.buffer(5)
		.as_blocking()
		.subscribe([&](const std::vector<std::string>& batch) {
			savedEventsCount += static_cast<int>(batch.size());
			std::cout << "[DB] Batch INSERT #" << batchNumber++ << ": " << joinBatch(batch) << '\n';
		});

	std::cout << "(+) Збережено подій: " << savedEventsCount << '\n';

	std::cout << "\nЧастина B — Flowable + BackpressureStrategy.DROP\n";

	const auto report = simulateDropBackpressure();
	std::cout << "[ЗВІТ] Оброблено: ~" << report.processed << '\n';
	std::cout << "[ЗВІТ] Відкинуто: ~" << report.dropped << '\n';
	std::cout << "(!) Стратегія DROP: частину елементів втрачено\n";
}
