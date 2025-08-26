cmake_minimum_required(VERSION 3.16)
project(numeraire-cpp)

# ---------------------------------------------------------------------------
# Global settings
# ---------------------------------------------------------------------------
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Debug & LLDB-friendly flags
add_compile_options("$<$<CONFIG:Debug>:-O0;-g3;-fno-omit-frame-pointer;-fno-limit-debug-info>")
add_link_options("$<$<CONFIG:Debug>:-fstandalone-debug>")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -fno-inline -DDEBUG")
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)

# libc++ in Debug only
add_compile_options("$<$<CONFIG:Debug>:-stdlib=libc++>")

# Shared bin/ for all configs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY         ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)

# (Optional) Xcode bits — harmless in CLion, keep for dsym/LLDB rendering
set(CMAKE_XCODE_GENERATE_SCHEME TRUE)
set(CMAKE_XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym")
set(CMAKE_XCODE_ATTRIBUTE_GCC_OPTIMIZATION_LEVEL "0")
set(CMAKE_OSX_ARCHITECTURES arm64)
set(CMAKE_OSX_DEPLOYMENT_TARGET "15.3.2")

# ---------------------------------------------------------------------------
# External packages
# ---------------------------------------------------------------------------
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(cpr REQUIRED)
find_package(pugixml REQUIRED)
find_package(GTest REQUIRED)

find_library(SQLITE3_LIBRARY sqlite3 PATHS /opt/homebrew/lib REQUIRED)
find_library(QUANTLIB_LIBRARY QuantLib PATHS /opt/homebrew/lib REQUIRED)

# Interface target to centralize third-party deps + include roots
add_library(numeraire_deps INTERFACE)
target_include_directories(numeraire_deps INTERFACE
        ${CMAKE_SOURCE_DIR}/header
        /opt/homebrew/include          # Homebrew headers
        /opt/homebrew/opt/sqlite/include
)
target_link_libraries(numeraire_deps INTERFACE
        spdlog::spdlog
        fmt::fmt
        cpr::cpr
        nlohmann_json::nlohmann_json
        pugixml::pugixml
        ${QUANTLIB_LIBRARY}
        ${SQLITE3_LIBRARY}
)

# ---------------------------------------------------------------------------
# File discovery
# ---------------------------------------------------------------------------
file(GLOB_RECURSE CORE_SRC             src/core/*.cpp)
file(GLOB_RECURSE UTILS_SRC            src/utils/*.cpp)
file(GLOB_RECURSE PRICING_SRC          src/pricing/*.cpp)
file(GLOB_RECURSE ASSET_EQUITY_SRC     src/pricing/asset_class/equity/*.cpp)

file(GLOB_RECURSE DATABASE_SRC         src/database/*.cpp)
file(GLOB_RECURSE SIMULATION_SRC       src/simulation/*.cpp)
file(GLOB_RECURSE MARKET_STORE_SRC     src/market_store/*.cpp)
file(GLOB_RECURSE CLI_SRC              src/cli/*.cpp)   # new cli module

file(GLOB_RECURSE CORE_HEADERS         header/core/*.hpp)
file(GLOB_RECURSE UTILS_HEADERS        header/utils/*.hpp)
file(GLOB_RECURSE PRICING_HEADERS      header/pricing/*.hpp)
file(GLOB_RECURSE ASSET_EQUITY_HEADERS header/pricing/asset_class/equity/*.hpp)

file(GLOB_RECURSE ENUMS_HEADERS        header/enums/*.hpp)
file(GLOB_RECURSE DATABASE_HEADERS     header/database/*.hpp)
file(GLOB_RECURSE SIMULATION_HEADERS   header/simulation/*.hpp)
file(GLOB_RECURSE MARKET_STORE_HEADERS header/market_store/*.hpp)
file(GLOB_RECURSE CLI_HEADERS          header/cli/*.hpp)

set(CACHE_MARKET_SRC    ${CMAKE_SOURCE_DIR}/src/main_cache_market_data.cpp)
set(PRICE_PORTFOLIO_SRC ${CMAKE_SOURCE_DIR}/src/main_price_portfolio.cpp)
set(SIMULATE_PATHS_SRC  ${CMAKE_SOURCE_DIR}/src/main_simulate_paths.cpp)

# ---------------------------------------------------------------------------
# Modular libraries (no monolithic project_lib)
# ---------------------------------------------------------------------------
add_library(core_lib            ${CORE_SRC}            ${CORE_HEADERS}            ${ENUMS_HEADERS})
add_library(utils_lib           ${UTILS_SRC}           ${UTILS_HEADERS})
add_library(pricing_lib         ${PRICING_SRC}         ${PRICING_HEADERS})
add_library(asset_equity_lib    ${ASSET_EQUITY_SRC}    ${ASSET_EQUITY_HEADERS})
add_library(database_lib        ${DATABASE_SRC}        ${DATABASE_HEADERS})
add_library(simulation_lib      ${SIMULATION_SRC}      ${SIMULATION_HEADERS})
add_library(market_store_lib    ${MARKET_STORE_SRC}    ${MARKET_STORE_HEADERS})
add_library(cli_args            ${CLI_SRC}             ${CLI_HEADERS})

# All libs inherit common deps and include roots
target_link_libraries(core_lib         PUBLIC numeraire_deps)
target_link_libraries(utils_lib        PUBLIC numeraire_deps)
target_link_libraries(pricing_lib      PUBLIC numeraire_deps)
target_link_libraries(asset_equity_lib PUBLIC numeraire_deps)
target_link_libraries(database_lib     PUBLIC numeraire_deps)
target_link_libraries(simulation_lib   PUBLIC numeraire_deps)
target_link_libraries(market_store_lib PUBLIC numeraire_deps)
target_link_libraries(cli_args         PUBLIC numeraire_deps)

# Convenience meta target (optional): link groups commonly used together
add_library(numeraire_all INTERFACE)
target_link_libraries(numeraire_all INTERFACE
        core_lib utils_lib pricing_lib
        asset_equity_lib
        database_lib simulation_lib market_store_lib
)

# ---------------------------------------------------------------------------
# Executables: link only what they need (+ cli_args)
# ---------------------------------------------------------------------------
if(EXISTS ${CACHE_MARKET_SRC})
    add_executable(main_cache_market_data ${CACHE_MARKET_SRC})
    target_link_libraries(main_cache_market_data PRIVATE
            numeraire_all
            cli_args
    )
endif()

if(EXISTS ${PRICE_PORTFOLIO_SRC})
    add_executable(price_portfolio ${PRICE_PORTFOLIO_SRC})
    target_link_libraries(price_portfolio PRIVATE
            numeraire_all
            cli_args
    )
endif()

if(EXISTS ${SIMULATE_PATHS_SRC})
    add_executable(simulate_paths ${SIMULATE_PATHS_SRC})
    target_link_libraries(simulate_paths PRIVATE
            numeraire_all
            cli_args
    )
endif()

# ---------------------------------------------------------------------------
# Unit tests
# ---------------------------------------------------------------------------
enable_testing()
file(GLOB_RECURSE TEST_SRC unit_tests/*.cpp)

add_executable(test_environment ${TEST_SRC})
target_link_libraries(test_environment PRIVATE
        numeraire_all
        cli_args
        GTest::gtest_main
)
add_test(NAME AllTests COMMAND test_environment)

# ---------------------------------------------------------------------------
# Source grouping (nice in CLion/Xcode/VS)
# ---------------------------------------------------------------------------
source_group(TREE ${CMAKE_SOURCE_DIR}/src PREFIX "src" FILES
        ${CORE_SRC} ${UTILS_SRC} ${PRICING_SRC}
        ${ASSET_EQUITY_SRC}
        ${DATABASE_SRC} ${SIMULATION_SRC} ${MARKET_STORE_SRC} ${CLI_SRC}
)
source_group(TREE ${CMAKE_SOURCE_DIR}/header PREFIX "header" FILES
        ${CORE_HEADERS} ${UTILS_HEADERS} ${PRICING_HEADERS}
        ${ASSET_EQUITY_HEADERS}
        ${ENUMS_HEADERS} ${DATABASE_HEADERS} ${SIMULATION_HEADERS}
        ${MARKET_STORE_HEADERS} ${CLI_HEADERS}
)

# configs (visible in IDE; optional)
file(GLOB CONFIG_FILES configs/*)
source_group(TREE ${CMAKE_SOURCE_DIR}/configs PREFIX "configs" FILES ${CONFIG_FILES})
# If you want them attached to any target, you can add them like this:
# target_sources(core_lib PRIVATE ${CONFIG_FILES})
