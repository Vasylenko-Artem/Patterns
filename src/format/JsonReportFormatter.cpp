#include "format/JsonReportFormatter.hpp"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string JsonReportFormatter::formatUsers(const std::vector<User>& users) const {
    json items = json::array();

    for (const auto& user : users) {
        items.push_back({
            {"id", user.id},
            {"name", user.name},
            {"email", user.email},
        });
    }

    const json report{
        {"total", users.size()},
        {"users", items},
    };

    return report.dump(2);
}
