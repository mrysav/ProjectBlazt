cmake_minimum_required(VERSION 3.0.0)
project(ProjectBlazt VERSION 0.1.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include(CTest)
enable_testing()

find_package(Allegro REQUIRED)

include_directories(
    ${ALLEGRO_INCLUDE_DIRS}
)

add_executable(ProjectBlazt game.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_link_libraries(ProjectBlazt
    m
    ${ALLEGRO_LIBRARIES}
)
