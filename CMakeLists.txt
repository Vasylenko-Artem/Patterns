cmake_minimum_required(VERSION 3.20)
project(Patterns LANGUAGES CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

FetchContent_Declare(
	rxcpp
	GIT_REPOSITORY https://github.com/ReactiveX/RxCpp.git
	GIT_TAG v4.1.1
	GIT_SHALLOW TRUE
	GIT_PROGRESS TRUE
	UPDATE_DISCONNECTED TRUE
)

FetchContent_GetProperties(rxcpp)
if(NOT rxcpp_POPULATED)
	FetchContent_Populate(rxcpp)
endif()

add_executable(patterns
	src/main.cpp
	src/task_1_1.cpp
	src/task_1_2.cpp
	src/task_2_1.cpp
	src/task_2_2.cpp
	src/task_3_1.cpp
	src/task_3_2.cpp
	src/task_4_1.cpp
	src/task_4_2.cpp
	src/task_5_1.cpp
	src/task_5_2.cpp
	src/task_6_1.cpp
	src/task_6_2.cpp
	src/task_7_1.cpp
	src/task_7_2.cpp
)

target_include_directories(patterns PRIVATE
	${rxcpp_SOURCE_DIR}/Rx/v2/src
)
