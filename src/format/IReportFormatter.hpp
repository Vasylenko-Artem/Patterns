#pragma once

#include "domain/User.hpp"

#include <string>
#include <vector>

class IReportFormatter {
public:
    virtual ~IReportFormatter() = default;
    virtual std::string formatUsers(const std::vector<User>& users) const = 0;
};
