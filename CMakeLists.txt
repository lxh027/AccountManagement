cmake_minimum_required(VERSION 3.12)
project(AccountManagement)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
add_executable(AccountManagement main.cpp menu.cpp menu.h model.h card_service.cpp card_service.h tool.cpp tool.h json/json.h json/json-forwards.h card_file.cpp card_file.h global.h service.cpp service.h billing_service.cpp billing_service.h billing_file.cpp billing_file.h jsoncpp.cpp money_file.cpp money_file.h)