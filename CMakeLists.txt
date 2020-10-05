cmake_minimum_required(VERSION 3.17)
project(mjhvbjh VERSION 1.0.0
  DESCRIPTION mjhvbjh
  LANGUAGES CXX
  ) 
cmake_policy(SET CMP0015 NEW)

OPTION(GUI_MODE "Option description" ON) # Enabled by default
IF(GUI_MODE)
    ADD_DEFINITIONS(-DGUI_MODE)
ENDIF(GUI_MODE)
########## enable cache system
include(cmake/Cache.cmake)

#set(CMAKE_EXECUTABLE_SUFFIX ".html")
########## link against the libs to apply the configurations to the target
# compiler warnings
set(PROJECT_WARNINGS project_warning) 
add_library(${PROJECT_WARNINGS} INTERFACE)
include(cmake/CompilerWarnings.cmake)
set_project_warnings(${PROJECT_WARNINGS})
# compiler options
set(PROJECT_OPTIONS project_options)
add_library(${PROJECT_OPTIONS} INTERFACE)
target_compile_features(${PROJECT_OPTIONS} INTERFACE cxx_std_20)
#target_compile_options(${PROJECT_OPTIONS} INTERFACE -ftime-trace)
#target_precompile_headers(${PROJECT_OPTIONS} INTERFACE /home/walde/.conan/data/nlohmann_json/3.9.1/_/_/package/d1091b2ed420e6d287293709a907ae824d5de508/include/nlohmann/json.hpp  /usr/lib64/gcc/x86_64-pc-linux-gnu/10.1.0/../../../../include/c++/10.1.0/bits/ios_base.h /usr/lib64/gcc/x86_64-pc-linux-gnu/10.1.0/../../../../include/c++/10.1.0/bits/char_traits.h)


include(cmake/Sanitizers.cmake)
enable_sanitizers(${PROJECT_OPTIONS})

# allow for static analysis options
#include(cmake/StaticAnalyzers.cmake) 
 
######## conan setup
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)


####### create executable
set(TARGET_NAME mjhvbjh)
add_executable(${TARGET_NAME}) 

######## add sub dir 
add_subdirectory(src) 


####### link
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_sources(${TARGET_NAME} PRIVATE "main.cxx")
include(GNUInstallDirs)
target_include_directories(${TARGET_NAME} PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )
  

target_link_libraries(${TARGET_NAME} PRIVATE ${PROJECT_WARNINGS} ${PROJECT_OPTIONS} 
CONAN_PKG::imgui-sfml
CONAN_PKG::spdlog
CONAN_PKG::range-v3
CONAN_PKG::cxxopts
)

#print all variables used in cmake
#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
