cmake_minimum_required(VERSION 3.16)

set(NAME MyCustomWidget)
project(${NAME})

get_property(USE_HALCON GLOBAL PROPERTY USE_HALCON)
get_property(USE_VTK GLOBAL PROPERTY USE_VTK)

if(USE_HALCON)
    add_definitions(-DBUILD_WITH_HALCON)
endif()

#------------------------------------------------------

#                       set qt

#------------------------------------------------------

find_package(QT NAMES Qt6 REQUIRED Designer)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Designer)
message(STATUS ${NAME} " build with Qt" ${QT_VERSION_MAJOR})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

#------------------------------------------------------

#               set out put path

#------------------------------------------------------

get_property(OUTPUT_PATH GLOBAL PROPERTY OUTPUT_PATH)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_PATH})

#------------------------------------------------------

#                   add source

#------------------------------------------------------
if(${QT_VERSION_MAJOR} EQUAL 5)
    qt5_add_resources(ALL_RESOURCES icons.qrc)
else()
    qt_add_resources(ALL_RESOURCES icons.qrc)
endif()

file(GLOB_RECURSE ALL_SOURCES
    *.cpp
    *.h
    *.ui
)

if(NOT USE_HALCON)
    file(GLOB_RECURSE HALCON_SOURCES
        QHalconDisplay/*
        QHalconDisplayMini/*
        QHalconMiniHud/*
    )
    list(REMOVE_ITEM ALL_SOURCES ${HALCON_SOURCES})
endif()

if(NOT USE_VTK)
    file(GLOB_RECURSE VTK_SOURCES
        QVtkPointCloudWidget/*
    )
    list(REMOVE_ITEM ALL_SOURCES ${VTK_SOURCES})
endif()

add_library(${NAME} SHARED ${ALL_SOURCES} ${ALL_RESOURCES})

source_group_by_dir(ALL_SOURCES)

add_definitions(-DQDESIGNER_EXPORT_WIDGETS)

set_property(TARGET ${NAME} PROPERTY DEBUG_POSTFIX d)

#------------------------------------------------------

#               add inckude path

#------------------------------------------------------

if(USE_HALCON)
    include_directories(${CMAKE_SOURCE_DIR}/include/halcon)
endif()
include_directories(${CMAKE_SOURCE_DIR}/include/tinyxml)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#------------------------------------------------------

#                   add libraries

#------------------------------------------------------

target_link_directories(${NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_directories(${NAME} PRIVATE ${CMAKE_SOURCE_DIR}/bin)

if(USE_HALCON)
    target_link_libraries(${NAME} PRIVATE halconcpp)
endif()
target_link_libraries(${NAME} PRIVATE
    debug tinyxml2d
    optimized tinyxml2
)

target_link_libraries(${NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Designer
)

#------------------------------------------------------

#                       vtk

#------------------------------------------------------

if(USE_VTK)
    find_package(VTK
      #9.0 # Compatibility support is not provided if 9.0 is requested.
      REQUIRED
      COMPONENTS
        # Old component names are OK, but deprecated.
        #vtkCommonCore
        #vtkRenderingOpenGL2
        # New names reflect the target names in use.
        CommonCore
        RenderingOpenGL2
        CommonCore
        RenderingOpenGL2
        CommonDataModel
        CommonExecutionModel
        FiltersSources
        InteractionStyle
        InteractionWidgets
        RenderingAnnotation
        RenderingCore
        RenderingFreeType
        GUISupportQt
    )
    target_link_libraries(${NAME} PRIVATE
        VTK::CommonCore
        VTK::RenderingOpenGL2
        VTK::CommonDataModel
        VTK::CommonExecutionModel
        VTK::FiltersSources
        VTK::InteractionStyle
        VTK::InteractionWidgets
        VTK::RenderingAnnotation
        VTK::RenderingCore
        VTK::RenderingFreeType
        VTK::GUISupportQt
    )
endif()

#------------------------------------------------------

#                   copy headers

#------------------------------------------------------

file(GLOB MY_PUBLIC_HEADERS Q442CustomGraphicItem.h Q442CustomWidget.h)
file(COPY ${MY_PUBLIC_HEADERS} DESTINATION ${OUTPUT_PATH}/../include)

file(GLOB MY_PUBLIC_HEADERS CustomEdits/*.h)
file(COPY ${MY_PUBLIC_HEADERS} DESTINATION ${OUTPUT_PATH}/../include/CustomEdits)

file(GLOB MY_PUBLIC_HEADERS QCpMonitor/qcpmonitor.h)
file(COPY ${MY_PUBLIC_HEADERS} DESTINATION ${OUTPUT_PATH}/../include/QCpMonitor)

file(GLOB MY_PUBLIC_HEADERS QDragTreeWidget/qdragtreewidget.h)
file(COPY ${MY_PUBLIC_HEADERS} DESTINATION ${OUTPUT_PATH}/../include/QDragTreeWidget)

file(GLOB MY_PUBLIC_HEADERS QGraphicMiniHud/qgrahpicviewminihud.h)
file(COPY ${MY_PUBLIC_HEADERS} DESTINATION ${OUTPUT_PATH}/../include/QGraphicMiniHud)

if(USE_HALCON)

file(GLOB MY_PUBLIC_HEADERS QHalconDisplay/*.h)
file(COPY ${MY_PUBLIC_HEADERS} DESTINATION ${OUTPUT_PATH}/../include/QHalconDisplay)

file(GLOB MY_PUBLIC_HEADERS QHalconDisplayMini/*.h)
file(COPY ${MY_PUBLIC_HEADERS} DESTINATION ${OUTPUT_PATH}/../include/QHalconDisplayMini)

file(GLOB MY_PUBLIC_HEADERS QHalconMiniHud/qhalconminihud.h)
file(COPY ${MY_PUBLIC_HEADERS} DESTINATION ${OUTPUT_PATH}/../include/QHalconMiniHud)

file(GLOB MY_PUBLIC_HEADERS Q442Halconwidget.h)
file(COPY ${MY_PUBLIC_HEADERS} DESTINATION ${OUTPUT_PATH}/../include)

endif()

file(GLOB MY_PUBLIC_HEADERS QXmlTreeWidget/*.h)
file(COPY ${MY_PUBLIC_HEADERS} DESTINATION ${OUTPUT_PATH}/../include/QXmlTreeWidget)

file(GLOB MY_PUBLIC_HEADERS QZoomGraphicItems/*.h)
file(COPY ${MY_PUBLIC_HEADERS} DESTINATION ${OUTPUT_PATH}/../include/QZoomGraphicItems)

file(GLOB MY_PUBLIC_HEADERS QZoomGraphicView/*.h)
file(COPY ${MY_PUBLIC_HEADERS} DESTINATION ${OUTPUT_PATH}/../include/QZoomGraphicView)

if(USE_VTK)

file(GLOB MY_PUBLIC_HEADERS QVtkPointCloudWidget/QVtkPointCloudWidget.h)
file(COPY ${MY_PUBLIC_HEADERS} DESTINATION ${OUTPUT_PATH}/../include)

endif()
