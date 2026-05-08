#include "tasks.h"

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

template <typename T>
void printVector(const std::vector<T> &numbers)
{
	for (const T n : numbers)
		std::cout << n << " ";
	std::cout << std::endl;
}

class Tasks
{
public:
	std::vector<int> filterOddNumbers(const std::vector<int> &numbers)
	{
		std::vector<int> odd_numbers;

		std::copy_if(numbers.begin(), numbers.end(),
			     std::back_inserter(odd_numbers),
			     [](int x)
			     { return x % 2 != 0; });

		return odd_numbers;
	}

	double findAverage(const std::vector<double> &values)
	{

		double sum = std::accumulate(values.begin(), values.end(), 0.0,
					     [](double acc, double x)
					     {
						     return acc + x;
					     });

		return sum / values.size();
	}

	std::vector<std::string> sortStrings(std::vector<std::string> strings)
	{
		std::sort(strings.begin(), strings.end(),
			  [](const std::string &a, const std::string &b)
			  {
				  return a < b;
			  });

		return strings;
	}

	int sumEvenNumbers(const std::vector<int> &numbers)
	{
		return std::accumulate(numbers.begin(), numbers.end(), 0,
				       [](int acc, int x)
				       {
					       return acc + (x % 2 == 0 ? x : 0);
				       });
	}

	int calculateFactorial(int number)
	{
		auto factorial = [](int n)
		{
			int fact = 1;
			for (int i = 1; i <= n; i++)
				fact *= i;
			return fact;
		};

		return factorial(number);
	}

	int calculateProduct(const std::vector<int> &numbers)
	{
		return std::accumulate(numbers.begin(), numbers.end(), 1,
				       [](int acc, int x)
				       {
					       return acc * x;
				       });
	}

	int calculateSum(const std::vector<int> &numbers)
	{
		return std::accumulate(numbers.begin(), numbers.end(), 0,
				       [](int acc, int x)
				       {
					       return acc + x;
				       });
	}

	std::pair<int, int> calculateProductAndSum(const std::vector<int> &numbers)
	{
		return std::accumulate(numbers.begin(), numbers.end(),
				       std::make_pair(1, 0),
				       [](std::pair<int, int> acc, int x)
				       {
					       return std::make_pair(
						   acc.first * x, // product
						   acc.second + x // sum
					       );
				       });
	}

	std::vector<int>
	calculateSquare(std::vector<int> numbers)
	{
		std::transform(numbers.begin(), numbers.end(), numbers.begin(),
			       [](int x)
			       {
				       return x * x;
			       });

		return numbers;
	}

	std::vector<std::string> sortStringsByLength(std::vector<std::string> strings)
	{
		std::sort(strings.begin(), strings.end(),
			  [](const std::string &a, const std::string &b)
			  {
				  return a.size() < b.size();
			  });

		return strings;
	}

	int countWordsInSentence(const std::string &sentence)
	{
		bool in_word = false;

		return std::count_if(sentence.begin(), sentence.end(),
				     [&in_word](char c)
				     {
					     if (!std::isspace(static_cast<unsigned char>(c)))
					     {
						     if (!in_word)
						     {
							     in_word = true;
							     return true; // new word
						     }
					     }
					     else
					     {
						     in_word = false;
					     }
					     return false;
				     });
	}

	std::string findFirstNonEmptyString(std::vector<std::string> strings)
	{
		auto it = std::find_if(strings.begin(), strings.end(),
				       [](const std::string &s)
				       {
					       return std::any_of(s.begin(), s.end(), [](char c)
								  { return !std::isspace(c); });
				       });

		if (it != strings.end())
			return *it;
		else
			return "";
	}

	bool checkIfAllStringsStartWithCapitalLetter(std::vector<std::string> strings)
	{
		return std::all_of(strings.begin(), strings.end(),
				   [](const std::string &s)
				   {
					   return !s.empty() && std::isupper(static_cast<unsigned char>(s[0])); // first letter
				   });
	}

	int findSecondLargestNumber(std::vector<int> numbers)
	{
		if (numbers.size() < 2)
			return -1;

		std::sort(numbers.begin(), numbers.end(),
			  [](int a, int b)
			  { return a > b; });

		return numbers[1]; // second largest
	}

	int findLargestEvenNumber(std::vector<int> numbers)
	{
		std::vector<int> even_numbers;

		std::copy_if(numbers.begin(), numbers.end(),
			     std::back_inserter(even_numbers),
			     [](int x)
			     { return x % 2 == 0; });

		auto it = std::max_element(even_numbers.begin(), even_numbers.end());

		if (it != even_numbers.end())
			return *it;
		else
			return -1;
	}
};

int task_04()
{

	Tasks task;

	// 1. Використовуючи лямбда - вираз, відфільтрувати непарні числа зі списку цілих чисел
	{
		auto result = task.filterOddNumbers({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
		printVector(result);
	}

	// 2. Використовуючи лямбда - вираз, знайти середнє зі списку дійсних значень
	{
		auto result = task.findAverage({1.1, 2.2, 3.3, 4.4, 5.5});
		std::cout << "Average: " << result << std::endl;
	}

	// 3. Використовуючи лямбда - вираз для сортування списку рядків в алфавітному порядку
	{
		auto result = task.sortStrings({"apple", "banana", "cherry", "date", "elderberry", "abc"});
		printVector(result);
	}

	// 4. Використовуючи лямбда - вираз для обчислення суми всіх парних чисел
	{
		auto result = task.sumEvenNumbers({1, 4, 6, 3, 10});
		std::cout << "Sum of even numbers: " << result << std::endl;
	}

	// 5. Використовуючи лямбда - вираз, обчислити факторіал заданого числа
	{
		int number = 5;
		auto result = task.calculateFactorial(number);
		std::cout << "Factorial of " << number << " is " << result << std::endl;
	}

	// 6. За допомогою лямбда - виразу розрахуйте множення та підсумовування всіх елементів у списку цілих чисел
	{
		auto [product, sum] = task.calculateProductAndSum({1, 2, 3, 4, 5});

		std::cout << "Product: " << product << std::endl;
		std::cout << "Sum: " << sum << std::endl;
	}
	// 7. За допомогою лямбда - виразу, розрахуйте квадрат кожного числа у списку цілих чисел
	{
		auto result = task.calculateSquare({1, 2, 3, 4, 5});
		printVector(result);
	}

	// 8. Використовуючи лямбда - вираз для сортування рядків на основі їх довжини в порядку зростання
	{
		auto result = task.sortStringsByLength({"apple", "banana", "cherry", "date", "elderberry", "abc"});
		printVector(result);
	}
	// 9. За допомогою лямбда - виразу підрахуйте кількість слів у реченні(слова розділені пробілами)
	{
		auto result = task.countWordsInSentence("test    test test");
		std::cout << "Word count: " << result << std::endl;
	}

	// 10. Використовуючи лямбда - вираз, знайти перший непорожній рядок у списку рядків
	{
		auto result = task.findFirstNonEmptyString({"", "", "apple", "", "banana", "", "cherry"});
		std::cout << "First non-empty string: " << result << std::endl;
	}

	// 11. За допомогою лямбда - виразу, перевірити, чи всі рядки у списку починаються з великої літери
	{
		bool result = task.checkIfAllStringsStartWithCapitalLetter({"Apple", "Banana", "Cherry"});
		std::cout << "All strings start with capital letter: " << (result ? "true" : "false") << std::endl;
	}

	// 12. Використовуючи лямбда - вираз, знайти друге за величиною число в списку цілих чисел
	{
		int second_largest = task.findSecondLargestNumber({1, 2, 3, 4, 5});
		std::cout << "Second largest number: " << second_largest << std::endl;
	}

	// 13. Використовуючи лямбда - вираз, знайти найбільше парне число в списку цілих чисел
	{
		int largest_even = task.findLargestEvenNumber({1, 2, 3, 4, 5});
		std::cout << "Largest even number: " << largest_even << std::endl;
	}
	return 0;
}
