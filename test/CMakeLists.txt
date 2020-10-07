set(TEST_TARGET_NAME ${TARGET_NAME}_test)
add_executable(${TEST_TARGET_NAME} main.cxx gameTest.cxx)
target_link_libraries(${TEST_TARGET_NAME}
${PROJECT_OPTIONS}
CONAN_PKG::catch2
CONAN_PKG::imgui-sfml
CONAN_PKG::spdlog
CONAN_PKG::range-v3
CONAN_PKG::cxxopts
)
target_include_directories(${TEST_TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)

add_test(NAME ${TEST_TARGET_NAME}
         COMMAND ${TEST_TARGET_NAME} -r junit) 
