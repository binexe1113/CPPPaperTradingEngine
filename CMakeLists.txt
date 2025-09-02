# Set the minimum required version of CMake.
cmake_minimum_required(VERSION 3.15)

# Define the project name, version, and language.
project(CPPPaperTradingEngine LANGUAGES CXX)

# Set the C++ standard to C++20 and make it a requirement.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create the executable target for the trading engine.
# Source files will be added to this target in the next step.
add_executable(CPPPaperTradingEngine)

# Add all your source files (.cpp) to the executable target.
# If you create new source files, you should add them to this list.
target_sources(CPPPaperTradingEngine PRIVATE
    main.cpp
    core/Portfolio.cpp
    utils/PriceFetcher.cpp
    threads/TradeWorker.cpp
)

# Specify the directories where your header files (.h, .hpp) are located.
# Using ${PROJECT_SOURCE_DIR} ensures the path is always correct.
target_include_directories(CPPPaperTradingEngine PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# A simple message to show when CMake is configuring the project.
message(STATUS "Configuring project: ${PROJECT_NAME}")
