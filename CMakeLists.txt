cmake_minimum_required(VERSION 3.22)

# set(CMAKE_BUILD_TYPE "Debug")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project(mandelbrot_generator)

find_package(Boost 1.82 REQUIRED)
find_package(JPEG REQUIRED)

add_executable(${PROJECT_NAME}
    src/error.cpp
    src/ColoringStrategies/ColoringStrategyBasicDistanceEstimation.cpp
    src/EscapeTimeAlgos/EscapeTimeAlgoDistanceEstimation.cpp
    src/EscapeTimeAlgos/EscapeTimeAlgoIterationOnly.cpp
    src/GeneratingStrategy.cpp
    src/GeneratingStrategies/GeneratingStrategyRandomZoom.cpp
    src/MySQLQuery.cpp
    src/MySQLQueries/MySQLQueryCreateImageTable.cpp
    src/MySQLQueries/MySQLQueryInsertImage.cpp
    src/MySQLQueryTypes.cpp
    src/MySQLStore.cpp
    src/Config.cpp
    src/Calculator.cpp
    src/Generator.cpp
    src/Image.cpp
    src/Data.cpp
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
    crypto
    jpeg
    boost_iostreams
)