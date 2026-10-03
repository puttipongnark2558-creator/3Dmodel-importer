cmake_minimum_required(VERSION 3.13.4)

# Allow legacy policy for CMake 4+ runner environments
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "4.0.0")
    cmake_policy(SET CMP0169 OLD)
    set(CMAKE_POLICY_VERSION_MINIMUM "3.5" CACHE STRING "Minimum policy version" FORCE)
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