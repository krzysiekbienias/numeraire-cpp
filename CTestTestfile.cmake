# CMake generated Testfile for 
# Source directory: /Users/krzysiekbienias/Dev/cpp/clicker-plus-plus
# Build directory: /Users/krzysiekbienias/Dev/cpp/clicker-plus-plus
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(AllTests "/Users/krzysiekbienias/Dev/cpp/clicker-plus-plus/Debug/test_environment")
  set_tests_properties(AllTests PROPERTIES  _BACKTRACE_TRIPLES "/Users/krzysiekbienias/Dev/cpp/clicker-plus-plus/CMakeLists.txt;67;add_test;/Users/krzysiekbienias/Dev/cpp/clicker-plus-plus/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(AllTests "/Users/krzysiekbienias/Dev/cpp/clicker-plus-plus/Release/test_environment")
  set_tests_properties(AllTests PROPERTIES  _BACKTRACE_TRIPLES "/Users/krzysiekbienias/Dev/cpp/clicker-plus-plus/CMakeLists.txt;67;add_test;/Users/krzysiekbienias/Dev/cpp/clicker-plus-plus/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(AllTests "/Users/krzysiekbienias/Dev/cpp/clicker-plus-plus/MinSizeRel/test_environment")
  set_tests_properties(AllTests PROPERTIES  _BACKTRACE_TRIPLES "/Users/krzysiekbienias/Dev/cpp/clicker-plus-plus/CMakeLists.txt;67;add_test;/Users/krzysiekbienias/Dev/cpp/clicker-plus-plus/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(AllTests "/Users/krzysiekbienias/Dev/cpp/clicker-plus-plus/RelWithDebInfo/test_environment")
  set_tests_properties(AllTests PROPERTIES  _BACKTRACE_TRIPLES "/Users/krzysiekbienias/Dev/cpp/clicker-plus-plus/CMakeLists.txt;67;add_test;/Users/krzysiekbienias/Dev/cpp/clicker-plus-plus/CMakeLists.txt;0;")
else()
  add_test(AllTests NOT_AVAILABLE)
endif()
