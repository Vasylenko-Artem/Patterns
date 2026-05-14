#include "tasks.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <rxcpp/rx.hpp>
#include <string>
#include <vector>

namespace
{
using namespace std::chrono_literals;

struct FoodOrder
{
	std::string orderId;
	std::vector<std::string> items;
};

struct IndexedFoodItem
{
	std::size_t orderIndex;
	std::size_t itemIndex;
	std::string name;
};

std::vector<FoodOrder> getFoodOrders()
{
	return {
		{"ZAM-01", {"Піца Маргарита", "Кола 0.5л"}},
		{"ZAM-02", {"Борщ", "Вареники", "Компот"}},
		{"ZAM-03", {"Суші-сет 20шт", "Місо-суп"}}
	};
}

auto indexedOrderItems(const std::vector<FoodOrder>& orders, std::size_t orderIndex)
{
	std::vector<IndexedFoodItem> items;
	const auto& order = orders.at(orderIndex);

	for (std::size_t itemIndex = 0; itemIndex < order.items.size(); ++itemIndex) {
		items.push_back({orderIndex, itemIndex, order.items.at(itemIndex)});
	}

	return rxcpp::observable<>::iterate(items);
}

auto delayedOrderItems(FoodOrder order, std::chrono::milliseconds delay)
{
	return rxcpp::observable<>::timer(delay)
		.flat_map([order](long) {
			return rxcpp::observable<>::iterate(order.items)
				.map([order](const std::string& item) {
					return order.orderId + " >> " + item;
				});
		});
}
}

void runTask3_2()
{
	const auto orders = getFoodOrders();

	std::cout << "Частина A — flatMap():\n";
	std::vector<IndexedFoodItem> flattenedItems;

	rxcpp::observable<>::range(0, static_cast<int>(orders.size() - 1))
		.flat_map([&orders](int orderIndex) {
			return indexedOrderItems(orders, static_cast<std::size_t>(orderIndex));
		})
		.as_blocking()
		.subscribe([&flattenedItems](const IndexedFoodItem& item) {
			flattenedItems.push_back(item);
		});

	std::ranges::sort(flattenedItems, {}, [](const IndexedFoodItem& item) {
		return std::pair{item.orderIndex, item.itemIndex};
	});

	for (const auto& item : flattenedItems) {
		std::cout << ">> " << item.name << '\n';
	}

	std::cout << "\nЧастина B — flatMap() із затримкою:\n";
	rxcpp::observable<>::iterate(orders)
		.flat_map([](const FoodOrder& order) {
			if (order.orderId == "ZAM-01") {
				return delayedOrderItems(order, 800ms);
			}
			if (order.orderId == "ZAM-02") {
				return delayedOrderItems(order, 650ms);
			}
			return delayedOrderItems(order, 500ms);
		})
		.as_blocking()
		.subscribe([](const std::string& item) {
			std::cout << "flatMap " << item << '\n';
		});

	std::cout << "\nЧастина B — concatMap() із затримкою:\n";
	rxcpp::observable<>::iterate(orders)
		.concat_map([](const FoodOrder& order) {
			return delayedOrderItems(order, 500ms);
		})
		.as_blocking()
		.subscribe([](const std::string& item) {
			std::cout << "concatMap " << item << '\n';
		});

	std::cout << "\nРізниця: flatMap підписується на внутрішні Observable паралельно, тому порядок може змінюватися. "
			  << "concatMap чекає завершення поточного Observable і зберігає порядок.\n";
}
