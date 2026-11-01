cmake_minimum_required(VERSION 3.14)
project(AutoPartsPlantSim VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(PLANT_SOURCES
    MaterialAndDetails.cpp
    ProductionAndEquipment.cpp
    QualityControl.cpp
    WarehouseAndLogistics.cpp
    OrdersAndManagement.cpp
)

set(PLANT_HEADERS
    Exceptions.hpp
    MaterialAndDetails.hpp
    ProductionAndEquipment.hpp
    QualityControl.hpp
    WarehouseAndLogistics.hpp
    OrdersAndManagement.hpp
)

add_library(plant_core STATIC ${PLANT_SOURCES} ${PLANT_HEADERS})
target_include_directories(plant_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(plant_sim main.cpp)
target_link_libraries(plant_sim PRIVATE plant_core)

find_path(UNITTESTPP_INCLUDE_DIR
    NAMES UnitTest++/UnitTest++.h
    PATHS /usr/include /usr/local/include /opt/homebrew/include
)

find_library(UNITTESTPP_LIBRARY
    NAMES UnitTest++ unittest++
    PATHS /usr/lib /usr/local/lib /opt/homebrew/lib /usr/lib/x86_64-linux-gnu
)

if(UNITTESTPP_INCLUDE_DIR AND UNITTESTPP_LIBRARY)
    message(STATUS "UnitTest++ found: ${UNITTESTPP_LIBRARY}")

    add_executable(run_tests tests.cpp)
    target_include_directories(run_tests PRIVATE ${UNITTESTPP_INCLUDE_DIR})
    target_link_libraries(run_tests PRIVATE plant_core ${UNITTESTPP_LIBRARY})

    enable_testing()
    add_test(NAME AllPlantTests COMMAND run_tests)
else()
    message(WARNING "UnitTest++ не найдена! Цель 'run_tests' пропущена. ")
endif()