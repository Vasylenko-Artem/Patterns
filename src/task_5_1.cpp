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

std::string downloadImage(const std::string& image)
{
	std::cout << "[io-1] [ЗАВАНТ] Завантаження: " << image << '\n';
	std::this_thread::sleep_for(1s);
	return image;
}

std::string compressImage(const std::string& image)
{
	std::cout << "[computation-1] [СТИСК] Стиснення: " << image << '\n';
	std::this_thread::sleep_for(500ms);
	return image;
}

void displayImage(const std::string& image)
{
	std::cout << "[main] [ФОТО] Відображення: " << image << '\n';
}
}

void runTask5_1()
{
	const std::vector<std::string> images = {
		"photo_1.jpg",
		"photo_2.jpg",
		"photo_3.jpg"
	};

	const auto ioScheduler = rxcpp::synchronize_new_thread();
	const auto computationScheduler = rxcpp::observe_on_event_loop();
	const auto mainScheduler = rxcpp::identity_current_thread();

	rxcpp::observable<>::iterate(images)
		.subscribe_on(ioScheduler)
		.map(downloadImage)
		.observe_on(computationScheduler)
		.map(compressImage)
		.observe_on(mainScheduler)
		.as_blocking()
		.subscribe(displayImage);
}
