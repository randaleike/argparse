# vargintf Library CMake file
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
cmake_policy(SET CMP0048 NEW)
project(vargintf VERSION 1.0.0.0 DESCRIPTION "Varg interface base" HOMEPAGE_URL "grocsoftware")

####
# vargintf directory cmake include Files
####

####
# vargintf Files
####
set (vargIntfInclude
    ${CMAKE_CURRENT_LIST_DIR}/inc/varg_intf.h
    )

set (vargIntfSrc
    ${CMAKE_CURRENT_LIST_DIR}/src/varg_intf.cpp
    )

set (vargIntfTest
    )

set (vargIntfMockInclude
    )

set (vargIntfMockSrc
    )

####
# vargintf library
####
add_library(${PROJECT_NAME} STATIC ${vargIntfSrc})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
