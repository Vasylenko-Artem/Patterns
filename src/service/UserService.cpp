#include "service/UserService.hpp"

#include <algorithm>
#include <utility>

UserService::UserService(std::shared_ptr<IUserRepository> repository)
    : repository_(std::move(repository)) {}

std::vector<User> UserService::getUsers() const {
    return repository_->findAll();
}

std::vector<User> UserService::getUsersByEmailDomain(const std::string& domain) const {
    const auto users = repository_->findAll();

    std::vector<User> filtered;
    std::copy_if(users.begin(), users.end(), std::back_inserter(filtered), [&](const User& user) {
        const auto at = user.email.find('@');
        return at != std::string::npos && user.email.substr(at + 1) == domain;
    });

    return filtered;
}
