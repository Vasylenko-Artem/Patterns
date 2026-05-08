#pragma once

#include "domain/User.hpp"
#include "repository/IUserRepository.hpp"

#include <memory>
#include <string>
#include <vector>

class UserService {
public:
    explicit UserService(std::shared_ptr<IUserRepository> repository);

    std::vector<User> getUsers() const;
    std::vector<User> getUsersByEmailDomain(const std::string& domain) const;

private:
    std::shared_ptr<IUserRepository> repository_;
};
