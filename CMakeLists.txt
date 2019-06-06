cmake_minimum_required(VERSION 3.1...3.14)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(ProjectBlazt VERSION 0.1.0
                     DESCRIPTION "A real fictional game."
                     LANGUAGES C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include(CTest)
enable_testing()

find_package(Allegro REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${ALLEGRO_INCLUDE_DIRS}
)

file(GLOB all_SRCS "${PROJECT_SOURCE_DIR}/src/*.c")

add_executable(ProjectBlazt ${all_SRCS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_link_libraries(ProjectBlazt
    m
    ${ALLEGRO_LIBRARIES}
)

set_property(TARGET ProjectBlazt PROPERTY C_STANDARD 11)
set_property(TARGET ProjectBlazt PROPERTY C_STANDARD_REQUIRED true)

file(COPY image DESTINATION .)
