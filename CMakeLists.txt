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

# httplib is header-only, you don’t need add_subdirectory unless you wrapped it
# add_subdirectory(vendor/httplib)

# --------------------------------------------------------
# libcurl (from submodule)
# --------------------------------------------------------
set(BUILD_CURL_EXE OFF CACHE BOOL "Don't build curl executable" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static curl" FORCE)

# Enable SSL via OpenSSL
set(ENABLE_SSL ON CACHE BOOL "Enable SSL" FORCE)
set(CURL_USE_OPENSSL ON CACHE BOOL "Use OpenSSL" FORCE)

# Disable optional stuff that often breaks Windows builds
set(CURL_USE_LIBPSL OFF CACHE BOOL "Disable libpsl" FORCE)
set(CURL_ZLIB OFF CACHE BOOL "Disable zlib" FORCE)

# Add curl submodule
add_subdirectory(vendor/curl)
# nlohmann/json is header-only
set(NLOHMANN_JSON_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/json/single_include)

# --------------------------------------------------------
# OpenGL (system)
# --------------------------------------------------------
find_package(OpenGL REQUIRED)

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
    symbols/tickerloader.cpp
    api/TiingoFinance.cpp
)

# Include your headers
target_include_directories(CPPPaperTradingEngine PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${NLOHMANN_JSON_INCLUDE_DIR}
)

# Link everything
target_link_libraries(CPPPaperTradingEngine PRIVATE
    imgui
    implot
    glfw
    OpenGL::GL
    libcurl   # <-- curl finally linked!
)

# --------------------------------------------------------
# Message
# --------------------------------------------------------
message(STATUS "Configuring project: ${PROJECT_NAME}")
