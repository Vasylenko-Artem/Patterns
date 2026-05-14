#include "tasks.h"

#include <iomanip>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

namespace
{
struct Product
{
	std::string name;
	double priceUsd;
};

struct AvailableProduct
{
	std::string name;
	double priceUah;
	std::string availability;
};

constexpr double UsdToUahRate = 41.5;

std::vector<Product> getProducts()
{
	return {
		{"Навушники Sony", 49.99},
		{"Клавіатура Logitech", 129.00},
		{"Монітор LG 27\"", 399.00},
		{"USB-хаб Anker", 35.00},
		{"Веб-камера Logitech", 149.00},
		{"Килимок для миші", 18.00},
		{"SSD Samsung 1TB", 110.00}
	};
}
}

void runTask3_1()
{
	const auto products = getProducts();
	auto availableExpensiveProducts = products
		| std::views::filter([](const Product& product) {
			return product.priceUsd > 100.0;
		})
		| std::views::transform([](const Product& product) {
			return AvailableProduct{
				product.name,
				product.priceUsd * UsdToUahRate,
				"(є в наявності)"
			};
		});

	for (const auto& product : availableExpensiveProducts) {
		std::cout << product.name << " -- "
				  << std::fixed << std::setprecision(2)
				  << product.priceUah << " грн "
				  << product.availability << '\n';
	}
}
