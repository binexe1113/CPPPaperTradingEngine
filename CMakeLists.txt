# Set the minimum required version of CMake.
cmake_minimum_required(VERSION 3.15)

# Define the project name and language.
project(CPPPaperTradingEngine LANGUAGES CXX)

# Set the C++ standard to C++20 and make it a requirement.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --------------------------------------------------------
# GLFW (from vendor)
# --------------------------------------------------------
add_subdirectory(vendor/glfw)

# --------------------------------------------------------
# OpenGL (system)
# --------------------------------------------------------
find_package(OpenGL REQUIRED)  # <-- you were missing this

# --------------------------------------------------------
# Dear ImGui
# --------------------------------------------------------
set(IMGUI_DIR ${PROJECT_SOURCE_DIR}/vendor/imgui)
set(IMGUI_SRC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)
target_link_libraries(imgui PUBLIC glfw OpenGL::GL)

# --------------------------------------------------------
# ImPlot
# --------------------------------------------------------
set(IMPLOT_DIR ${PROJECT_SOURCE_DIR}/vendor/implot)
set(IMPLOT_SRC
    ${IMPLOT_DIR}/implot.cpp
    ${IMPLOT_DIR}/implot_items.cpp
    ${IMPLOT_DIR}/implot_demo.cpp
)
add_library(implot STATIC ${IMPLOT_SRC})
target_include_directories(implot PUBLIC ${IMPLOT_DIR})
target_link_libraries(implot PUBLIC imgui)

# --------------------------------------------------------
# Your Executable
# --------------------------------------------------------
add_executable(CPPPaperTradingEngine
    main.cpp
    core/Portfolio.cpp
    utils/PriceFetcher.cpp
    api/tickerloader.cpp
)

# Include your headers
target_include_directories(CPPPaperTradingEngine PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# Link your app with ImGui + ImPlot + GLFW + OpenGL
target_link_libraries(CPPPaperTradingEngine PRIVATE imgui implot glfw OpenGL::GL)

# --------------------------------------------------------
# Message
# --------------------------------------------------------
message(STATUS "Configuring project: ${PROJECT_NAME}")
