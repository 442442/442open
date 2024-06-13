cmake_minimum_required(VERSION 3.16)

set(NAME MyCustomWidget)
project(${NAME})

#------------------------------------------------------

#                       set qt

#------------------------------------------------------

find_package(QT NAMES Qt6 Qt5 REQUIRED Designer)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Designer)
message(STATUS ${NAME} " build with Qt" ${QT_VERSION_MAJOR})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

#------------------------------------------------------

#               set out put path

#------------------------------------------------------

get_property(OUTPUT_PATH GLOBAL PROPERTY OUTPUT_PATH)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RLEASE ${OUTPUT_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_PATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_PATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RLEASE ${OUTPUT_PATH})

#------------------------------------------------------

#                   add source

#------------------------------------------------------

qt_add_resources(ALL_RESOURCES icons.qrc)

file(GLOB_RECURSE ALL_SOURCES
    *.cpp
    *.h
    *.ui
)

add_library(${NAME} SHARED ${ALL_SOURCES} ${ALL_RESOURCES})

source_group_by_dir(ALL_SOURCES)

add_definitions(-DQDESIGNER_EXPORT_WIDGETS)

set_property(TARGET ${NAME} PROPERTY DEBUG_POSTFIX d)

#------------------------------------------------------

#               add inckude path

#------------------------------------------------------

include_directories(${CMAKE_SOURCE_DIR}/include/halcon)
include_directories(${CMAKE_SOURCE_DIR}/include/tinyxml)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#------------------------------------------------------

#                   add libraries

#------------------------------------------------------

target_link_directories(${NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_directories(${NAME} PRIVATE ${CMAKE_SOURCE_DIR}/bin)

target_link_libraries(${NAME} PRIVATE halconcpp)
target_link_libraries(${NAME} PRIVATE
    debug tinyxml2d
    optimized tinyxml2
)

target_link_libraries(${NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Designer
)

