#include "tasks.h"

#include <exception>
#include <functional>
#include <iostream>
#include <rxcpp/rx.hpp>
#include <stdexcept>
#include <string>
#include <utility>

namespace
{
using MaybeString = rxcpp::observable<std::string>;
using SingleString = rxcpp::observable<std::string>;
using CompletableSource = rxcpp::observable<int>;

MaybeString findInCache(const std::string& key)
{
	if (key == "user:1") {
		return rxcpp::observable<>::just(std::string("{'name':'Леся','age':28}")).as_dynamic();
	}

	if (key == "user:2") {
		return rxcpp::observable<>::empty<std::string>().as_dynamic();
	}

	if (key == "user:error") {
		return rxcpp::observable<>::error<std::string>(
			std::make_exception_ptr(std::runtime_error("Redis недоступний"))).as_dynamic();
	}

	return rxcpp::observable<>::empty<std::string>().as_dynamic();
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

class Completable
{
public:
	explicit Completable(CompletableSource source)
		: source(std::move(source))
	{
	}

	Completable andThen(const Completable& next) const
	{
		return Completable(source.concat(next.source).as_dynamic());
	}

	SingleString andThen(const SingleString& next) const
	{
		return rxcpp::observable<>::create<std::string>(
			[first = source, next](rxcpp::subscriber<std::string> subscriber) {
				first.as_blocking().subscribe(
					[](int) {
					},
					[subscriber](std::exception_ptr error) {
						subscriber.on_error(error);
					},
					[subscriber, next] {
						next.as_blocking().subscribe(
							[subscriber](const std::string& value) {
								subscriber.on_next(value);
							},
							[subscriber](std::exception_ptr error) {
								subscriber.on_error(error);
							},
							[subscriber] {
								subscriber.on_completed();
							});
					});
			})
			.as_dynamic();
	}

	CompletableSource asObservable() const
	{
		return source;
	}

private:
	CompletableSource source;
};

Completable completedAction(const std::function<void()>& action)
{
	return Completable(rxcpp::observable<>::create<int>([action](rxcpp::subscriber<int> subscriber) {
		try {
			action();
			subscriber.on_completed();
		} catch (...) {
			subscriber.on_error(std::current_exception());
		}
	}).as_dynamic());
}

Completable failedAction(const std::function<void()>& action, const std::string& message)
{
	return Completable(rxcpp::observable<>::create<int>([action, message](rxcpp::subscriber<int> subscriber) {
		try {
			action();
			throw std::runtime_error(message);
		} catch (...) {
			subscriber.on_error(std::current_exception());
		}
	}).as_dynamic());
}

Completable validateInput()
{
	return completedAction([] {
		std::cout << "[ПОШУК] Перевірка даних...\n";
		std::cout << "(+) Дані валідні\n";
	});
}

Completable saveToDatabase(bool shouldFail)
{
	if (shouldFail) {
		return failedAction([] {
			std::cout << "[DB] Збереження в БД...\n";
		}, "Помилка збереження в БД");
	}

	return completedAction([] {
		std::cout << "[DB] Збереження в БД...\n";
		std::cout << "(+) Збережено\n";
	});
}

SingleString generateToken()
{
	return rxcpp::observable<>::just(std::string("eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.demo"))
		.map([](const std::string& token) {
			std::cout << "[ТОКЕН] Токен: " << token << '\n';
			return token;
		})
		.as_dynamic();
}

void runRegistration(bool failOnSave)
{
	validateInput()
		.andThen(saveToDatabase(failOnSave))
		.andThen(generateToken())
		.as_blocking()
		.subscribe(
			[](const std::string&) {
				std::cout << "(+) Реєстрацію завершено успішно\n";
			},
			[](std::exception_ptr error) {
				printError("(-) Реєстрацію перервано: ", error);
			});
}
}

void runTask4_2()
{
	std::cout << "Частина A — Maybe\n";

	findInCache("user:1")
		.default_if_empty(std::string("Завантажено з БД"))
		.as_blocking()
		.subscribe(
			[](const std::string& value) {
				std::cout << "[КЕШ (+)] Знайдено: " << value << '\n';
			},
			[](std::exception_ptr error) {
				printError("[КЕШ (+)] Помилка: ", error);
			});

	findInCache("user:2")
		.default_if_empty(std::string("Завантажено з БД"))
		.as_blocking()
		.subscribe(
			[](const std::string& value) {
				std::cout << "[КЕШ (-)] Кеш-міс. Значення: " << value << '\n';
			},
			[](std::exception_ptr error) {
				printError("[КЕШ (-)] Помилка: ", error);
			});

	findInCache("user:error")
		.default_if_empty(std::string("Завантажено з БД"))
		.as_blocking()
		.subscribe(
			[](const std::string& value) {
				std::cout << "[КЕШ (!)] Значення: " << value << '\n';
			},
			[](std::exception_ptr error) {
				printError("[КЕШ (!)] Помилка: ", error);
			});

	std::cout << "\nЧастина B — Completable + andThen(), успіх\n";
	runRegistration(false);

	std::cout << "\nЧастина B — Completable + andThen(), помилка saveToDatabase()\n";
	runRegistration(true);
}
