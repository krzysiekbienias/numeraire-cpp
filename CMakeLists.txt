cmake_minimum_required(VERSION 3.16)
project(numeraire-cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# === Ustawienie wspólnego folderu na binarki niezależnie od konfiguracji
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)

# Debug flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -fno-inline -DDEBUG")
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)

# Optional: enforce libc++ only in Debug mode
add_compile_options("$<$<CONFIG:Debug>:-stdlib=libc++>")

# Xcode-specific setup
set(CMAKE_XCODE_GENERATE_SCHEME TRUE)
set(CMAKE_XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym")
set(CMAKE_XCODE_ATTRIBUTE_GCC_OPTIMIZATION_LEVEL "0")
set(CMAKE_OSX_ARCHITECTURES arm64)
set(CMAKE_OSX_DEPLOYMENT_TARGET "15.3.2")

# Third-party includes
include_directories(SYSTEM /opt/homebrew/include)
include_directories(/opt/homebrew/opt/sqlite/include)

# === External Libraries ===
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(cpr REQUIRED)
find_package(pugixml REQUIRED)
find_package(GTest REQUIRED)

find_library(SQLITE3_LIBRARY sqlite3 PATHS /opt/homebrew/lib REQUIRED)
find_library(QUANTLIB_LIBRARY QuantLib PATHS /opt/homebrew/lib REQUIRED)

# === File Discovery ===
file(GLOB_RECURSE CORE_SRC src/core/*.cpp)
file(GLOB_RECURSE UTILS_SRC src/utils/*.cpp)
file(GLOB_RECURSE PRICING_SRC src/pricing/*.cpp)
file(GLOB_RECURSE ASSET_EQUITY_SRC src/asset_class/equity/*.cpp)
file(GLOB_RECURSE ASSET_FX_SRC src/asset_class/fx/*.cpp)
file(GLOB_RECURSE ASSET_RATES_SRC src/asset_class/rates/*.cpp)
file(GLOB_RECURSE DATABASE_SRC src/database/*.cpp)
file(GLOB_RECURSE SIMULATION_SRC src/simulation/*.cpp)
file(GLOB_RECURSE MARKET_STORE_SRC src/market_store/*.cpp)


file(GLOB_RECURSE CORE_HEADERS header/core/*.hpp)
file(GLOB_RECURSE UTILS_HEADERS header/utils/*.hpp)
file(GLOB_RECURSE PRICING_HEADERS header/pricing/*.hpp)
file(GLOB_RECURSE ASSET_EQUITY_HEADERS header/asset_class/equity/*.hpp)
file(GLOB_RECURSE ASSET_FX_HEADERS header/asset_class/fx/*.hpp)
file(GLOB_RECURSE ASSET_RATES_HEADERS header/asset_class/rates/*.hpp)
file(GLOB_RECURSE ENUMS_HEADERS header/enums/*.hpp)
file(GLOB_RECURSE DATABASE_HEADERS header/database/*.hpp)
file(GLOB_RECURSE SIMULATION_HEADERS header/simulation/*.hpp)
file(GLOB_RECURSE MARKET_STORE_HEADERS header/market_store/*.hpp)

set(CACHE_MARKET_SRC ${CMAKE_SOURCE_DIR}/src/main_cache_market_data.cpp)
set(PRICE_PORTFOLIO_SRC ${CMAKE_SOURCE_DIR}/src/main_price_portfolio.cpp)
set(SIMULATE_PATHS_SRC ${CMAKE_SOURCE_DIR}/src/main_simulate_paths.cpp)

# === Library Target ===
add_library(project_lib)

target_sources(project_lib PRIVATE
    ${CORE_SRC}
    ${UTILS_SRC}
    ${PRICING_SRC}
    ${ASSET_EQUITY_SRC}
    ${ASSET_FX_SRC}
    ${ASSET_RATES_SRC}
    ${DATABASE_SRC}
    ${SIMULATION_SRC}
    ${MARKET_STORE_SRC}

    ${CORE_HEADERS}	
    ${UTILS_HEADERS}
    ${PRICING_HEADERS}
    ${ASSET_EQUITY_HEADERS}
    ${ASSET_FX_HEADERS}
    ${ASSET_RATES_HEADERS}
    ${ENUMS_HEADERS}
    ${DATABASE_HEADERS}
    ${SIMULATION_HEADERS}
    ${MARKET_STORE_HEADERS}
)

target_include_directories(project_lib PUBLIC
    header
    header/enums
    header/core
    header/utils
    header/pricing
    header/asset_class
    header/asset_class/equity
    header/asset_class/fx
    header/asset_class/rates
    header/database
    header/simulation
    header/market_store
    /opt/homebrew/include
)

target_link_libraries(project_lib PUBLIC
    spdlog::spdlog
    fmt::fmt
    cpr::cpr
    nlohmann_json::nlohmann_json
    pugixml::pugixml
    ${QUANTLIB_LIBRARY}
    ${SQLITE3_LIBRARY}
)



# === Market Caching Executable ===
if(EXISTS ${CACHE_MARKET_SRC})
    add_executable(main_cache_market_data ${CACHE_MARKET_SRC})
    target_link_libraries(main_cache_market_data PRIVATE project_lib)
    target_include_directories(main_cache_market_data PRIVATE
        ${CMAKE_SOURCE_DIR}/header
        ${CMAKE_SOURCE_DIR}/header/core
        ${CMAKE_SOURCE_DIR}/header/utils
        ${CMAKE_SOURCE_DIR}/header/pricing
        ${CMAKE_SOURCE_DIR}/header/asset_class
        ${CMAKE_SOURCE_DIR}/header/asset_class/equity
        ${CMAKE_SOURCE_DIR}/header/asset_class/fx
        ${CMAKE_SOURCE_DIR}/header/asset_class/rates
        ${CMAKE_SOURCE_DIR}/header/simulation
        ${CMAKE_SOURCE_DIR}/header/market_store
    )
endif()

# === Portfolio Pricing Executable ===
if(EXISTS ${PRICE_PORTFOLIO_SRC})
    add_executable(price_portfolio ${PRICE_PORTFOLIO_SRC})
    target_link_libraries(price_portfolio PRIVATE project_lib)
    target_include_directories(price_portfolio PRIVATE
        ${CMAKE_SOURCE_DIR}/header
        ${CMAKE_SOURCE_DIR}/header/core
        ${CMAKE_SOURCE_DIR}/header/utils
        ${CMAKE_SOURCE_DIR}/header/pricing
        ${CMAKE_SOURCE_DIR}/header/asset_class
        ${CMAKE_SOURCE_DIR}/header/asset_class/equity
        ${CMAKE_SOURCE_DIR}/header/asset_class/fx
        ${CMAKE_SOURCE_DIR}/header/asset_class/rates
        ${CMAKE_SOURCE_DIR}/header/simulation
        ${CMAKE_SOURCE_DIR}/header/market_store
    )
endif()

# === Simulation Executable ===
if(EXISTS ${SIMULATE_PATHS_SRC})
    add_executable(simulate_paths ${SIMULATE_PATHS_SRC})
    target_link_libraries(simulate_paths PRIVATE project_lib)
    target_include_directories(simulate_paths PRIVATE
        ${CMAKE_SOURCE_DIR}/header
        ${CMAKE_SOURCE_DIR}/header/core
        ${CMAKE_SOURCE_DIR}/header/utils
        ${CMAKE_SOURCE_DIR}/header/pricing
        ${CMAKE_SOURCE_DIR}/header/database
        ${CMAKE_SOURCE_DIR}/header/asset_class
        ${CMAKE_SOURCE_DIR}/header/simulation
        ${CMAKE_SOURCE_DIR}/header/market_store
    )
endif()


# === Unit Tests ===
enable_testing()

file(GLOB_RECURSE TEST_SRC unit_tests/*.cpp)

add_executable(test_environment ${TEST_SRC})

target_link_libraries(test_environment
    PRIVATE project_lib GTest::gtest_main
)

target_include_directories(test_environment PRIVATE
    header
    header/core
    header/utils
    header/pricing
    header/asset_class
    header/simulation
)

add_test(NAME AllTests COMMAND test_environment)

# === Xcode Source Grouping ===
source_group(TREE ${CMAKE_SOURCE_DIR}/src PREFIX "src" FILES 
    ${CORE_SRC} ${UTILS_SRC} ${PRICING_SRC} 
    ${ASSET_EQUITY_SRC} ${ASSET_FX_SRC} ${ASSET_RATES_SRC} ${MAIN_SRC} ${DATABASE_SRC} ${SIMULATION_SRC} ${MARKET_STORE_SRC})
source_group(TREE ${CMAKE_SOURCE_DIR}/header PREFIX "header" FILES
    ${CORE_HEADERS} ${UTILS_HEADERS} ${PRICING_HEADERS} 
    ${ASSET_EQUITY_HEADERS} ${ASSET_FX_HEADERS} ${ASSET_RATES_HEADERS} ${ENUMS_HEADERS} ${DATABASE_HEADERS}
    ${SIMULATION_HEADERS} ${MARKET_STORE_HEADERS})
source_group(TREE ${CMAKE_SOURCE_DIR}/unit_tests PREFIX "unit_tests" FILES ${TEST_SRC})

# === Source Groups for Xcode/VisualStudio ===
file(GLOB CONFIG_FILES configs/*)
source_group(TREE ${CMAKE_SOURCE_DIR}/configs PREFIX "configs" FILES ${CONFIG_FILES})

# Dodaj pliki JSON do targetu (żeby Xcode je widział)
target_sources(project_lib PRIVATE ${CONFIG_FILES})
