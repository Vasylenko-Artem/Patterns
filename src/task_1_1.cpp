#include "tasks.h"

#include <iomanip>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

namespace
{
	enum class OrderStatus
	{
		DELIVERED,
		PENDING,
		CANCELLED
	};

	struct Order
	{
		std::string id;
		OrderStatus status;
		double amount;
	};

	int countDeliveredOrders(const std::vector<Order> &orders)
	{
		auto deliveredOrders = orders | std::views::filter([](const Order &order)
								   { return order.status == OrderStatus::DELIVERED; });

		return static_cast<int>(std::ranges::distance(deliveredOrders));
	}

	double calculateDeliveredTotal(const std::vector<Order> &orders)
	{
		auto deliveredAmounts = orders | std::views::filter([](const Order &order)
								    { return order.status == OrderStatus::DELIVERED; }) |
					std::views::transform([](const Order &order)
							      { return order.amount; });

		return std::accumulate(deliveredAmounts.begin(), deliveredAmounts.end(), 0.0);
	}
}

void runTask1_1()
{
	const std::vector<Order> orders = {
	    {"O-001", OrderStatus::DELIVERED, 1500.00},
	    {"O-002", OrderStatus::PENDING, 300.00},
	    {"O-003", OrderStatus::CANCELLED, 75.00},
	    {"O-004", OrderStatus::DELIVERED, 2200.00},
	    {"O-005", OrderStatus::PENDING, 450.00},
	    {"O-006", OrderStatus::DELIVERED, 980.00}};

	const int count = countDeliveredOrders(orders);
	const double deliveredTotal = calculateDeliveredTotal(orders);

	std::cout << "Виконаних замовлень: " << count << '\n';
	std::cout << "Загальна сума: " << std::fixed << std::setprecision(1) << deliveredTotal << '\n';
}
