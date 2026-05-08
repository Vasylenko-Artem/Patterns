#include "app/ConsoleApplication.hpp"

#include <iostream>
#include <utility>

ConsoleApplication::ConsoleApplication(UserService service, std::shared_ptr<IReportFormatter> formatter)
    : service_(std::move(service)), formatter_(std::move(formatter)) {}

void ConsoleApplication::run() const {
    const auto allUsers = service_.getUsers();
    const auto exampleUsers = service_.getUsersByEmailDomain("example.com");

    std::cout << "All users:\n" << formatter_->formatUsers(allUsers) << "\n\n";
    std::cout << "Users from example.com:\n" << formatter_->formatUsers(exampleUsers) << '\n';
}
