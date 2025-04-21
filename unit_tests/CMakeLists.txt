file(GLOB_RECURSE UNIT_TEST_SOURCES CONFIGURE_DEPENDS "*.cpp")

foreach(test_src ${UNIT_TEST_SOURCES})
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} ${test_src})
    target_link_libraries(${test_name} GTest::gtest GTest::gtest_main algos_lib)
    target_include_directories(${test_name} PRIVATE ${CMAKE_SOURCE_DIR}/include)
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()