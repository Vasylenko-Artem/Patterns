#pragma once

#include "domain/User.hpp"

#include <vector>

class IUserRepository {
public:
    virtual ~IUserRepository() = default;
    virtual std::vector<User> findAll() const = 0;
};
