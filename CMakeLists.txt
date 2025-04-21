cmake_minimum_required(VERSION 3.16)
project(clicker_plus_plus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_XCODE_GENERATE_SCHEME TRUE)
set(CMAKE_OSX_ARCHITECTURES arm64)
set(CMAKE_OSX_DEPLOYMENT_TARGET "15.3.2")

set(CMAKE_CONFIGURE_DEPENDS_NEW OFF CACHE INTERNAL "" FORCE)

# === File Discovery ===
file(GLOB_RECURSE ALL_SRC CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS header/*.hpp)
file(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS unit_tests/*.cpp)

# === Filter out main.cpp ===
set(SRC_FILES "")
foreach(file ${ALL_SRC})
    if(NOT file MATCHES ".*main\\.cpp$")
        list(APPEND SRC_FILES ${file})
    endif()
endforeach()

# === Source grouping (Xcode folders reflect real folders like arrays/, recursion/, etc.) ===
foreach(file ${SRC_FILES})
    file(RELATIVE_PATH rel_path "${CMAKE_SOURCE_DIR}/src" ${file})
    get_filename_component(dir "${rel_path}" DIRECTORY)
    source_group("src/${dir}" FILES ${file})
endforeach()

foreach(file ${HEADER_FILES})
    file(RELATIVE_PATH rel_path "${CMAKE_SOURCE_DIR}/header" ${file})
    get_filename_component(dir "${rel_path}" DIRECTORY)
    source_group("header/${dir}" FILES ${file})
endforeach()

foreach(file ${TEST_FILES})
    file(RELATIVE_PATH rel_path "${CMAKE_SOURCE_DIR}/unit_tests" ${file})
    get_filename_component(dir "${rel_path}" DIRECTORY)
    source_group("unit_tests/${dir}" FILES ${file})
endforeach()

# === Shared Library ===
add_library(project_lib ${SRC_FILES} ${HEADER_FILES})
target_include_directories(project_lib PUBLIC header)

# === Optional Main App ===
if(EXISTS "${CMAKE_SOURCE_DIR}/src/main.cpp")
    add_executable(clicker_plus_plus src/main.cpp)
    target_link_libraries(clicker_plus_plus PRIVATE project_lib)
endif()

# === Unit Tests ===
enable_testing()
find_package(GTest REQUIRED)

add_executable(test_environment ${TEST_FILES})
target_link_libraries(test_environment PRIVATE project_lib GTest::gtest_main)
target_include_directories(test_environment PRIVATE header)

add_test(NAME AllTests COMMAND test_environment)
