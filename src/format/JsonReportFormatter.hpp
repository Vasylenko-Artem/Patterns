#pragma once

#include "format/IReportFormatter.hpp"

class JsonReportFormatter final : public IReportFormatter {
public:
    std::string formatUsers(const std::vector<User>& users) const override;
};
