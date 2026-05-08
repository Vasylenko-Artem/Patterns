#include "repository/JsonUserRepository.hpp"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::vector<User> JsonUserRepository::findAll() const {
    const auto document = json::parse(R"([
        { "id": 1, "name": "Olena", "email": "olena@example.com" },
        { "id": 2, "name": "Andrii", "email": "andrii@example.com" },
        { "id": 3, "name": "Marta", "email": "marta@university.edu" },
        { "id": 4, "name": "Taras", "email": "taras@example.com" }
    ])");

    std::vector<User> users;
    users.reserve(document.size());

    for (const auto& item : document) {
        users.push_back(User{
            item.at("id").get<int>(),
            item.at("name").get<std::string>(),
            item.at("email").get<std::string>(),
        });
    }

    return users;
}
