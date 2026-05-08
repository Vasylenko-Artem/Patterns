cmake_minimum_required(VERSION 3.24)

project(PatternsProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

FetchContent_Declare(
    boost_di
    GIT_REPOSITORY https://github.com/boost-ext/di.git
    GIT_TAG cpp14
)

FetchContent_MakeAvailable(nlohmann_json boost_di)

add_executable(patterns_project
    src/app/ConsoleApplication.cpp
    src/format/JsonReportFormatter.cpp
    src/main.cpp
    src/repository/JsonUserRepository.cpp
    src/service/UserService.cpp
)

target_include_directories(patterns_project PRIVATE
    src
    ${boost_di_SOURCE_DIR}/include
)

target_link_libraries(patterns_project PRIVATE
    nlohmann_json::nlohmann_json
)
