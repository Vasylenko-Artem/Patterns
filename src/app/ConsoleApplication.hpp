#pragma once

#include "format/IReportFormatter.hpp"
#include "service/UserService.hpp"

#include <memory>

class ConsoleApplication {
public:
    ConsoleApplication(UserService service, std::shared_ptr<IReportFormatter> formatter);

    void run() const;

private:
    UserService service_;
    std::shared_ptr<IReportFormatter> formatter_;
};
