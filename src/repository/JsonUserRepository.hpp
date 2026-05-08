#pragma once

#include "repository/IUserRepository.hpp"

class JsonUserRepository final : public IUserRepository {
public:
    std::vector<User> findAll() const override;
};
