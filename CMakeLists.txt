cmake_minimum_required(VERSION 3.13.4)

# Automatically set CMAKE_PREFIX_PATH from GEODE_SDK environment variable if not already set
if(DEFINED ENV{GEODE_SDK})
    list(APPEND CMAKE_PREFIX_PATH "$ENV{GEODE_SDK}")
endif()

project(3Dmodel-importer VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Geode REQUIRED)

setup_geode_mod(
    NAME "3Dmodel importer"
    ID "puttipongnark2558-creator.3dmodel_importer"
)

add_subdirectory(src)