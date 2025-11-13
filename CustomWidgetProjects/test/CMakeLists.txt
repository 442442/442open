cmake_minimum_required(VERSION 3.16)

set(NAME test_demo)
project(${NAME})

get_property(USE_HALCON GLOBAL PROPERTY USE_HALCON)
get_property(USE_VTK GLOBAL PROPERTY USE_VTK)

if(USE_HALCON)
    add_definitions(-DBUILD_WITH_HALCON)
endif()

if(USE_VTK)
    add_definitions(-DBUILD_WITH_VTK)
endif()

#------------------------------------------------------

#                       set qt

#------------------------------------------------------

find_package(QT NAMES Qt6 Qt5 REQUIRED Core Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Xml)
message(STATUS ${NAME} " build with Qt" ${QT_VERSION_MAJOR})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

#------------------------------------------------------

#               set out put path

#------------------------------------------------------

get_property(OUTPUT_PATH GLOBAL PROPERTY OUTPUT_PATH)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_PATH})

#------------------------------------------------------

#                   add source

#---------------------------------------------------

add_executable(${NAME}
    mainwindow.ui
    mainwindow.cpp mainwindow.h
    main.cpp
    # windoweffect.cpp windoweffect.h
)

set_property(TARGET ${NAME} PROPERTY DEBUG_POSTFIX d)

#------------------------------------------------------

#               add inckude path

#------------------------------------------------------

if(USE_HALCON)
include_directories(${CMAKE_SOURCE_DIR}/include/halcon)
endif()
include_directories(${CMAKE_SOURCE_DIR}/MyCustomWidget)

#------------------------------------------------------

#                   add libraries

#------------------------------------------------------

target_link_directories(${NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_directories(${NAME} PRIVATE ${OUTPUT_PATH})

if(USE_HALCON)
    target_link_libraries(${NAME} PRIVATE halconcpp)
endif()

target_link_libraries(${NAME} PRIVATE
    debug MyCustomWidgetd
    optimized MyCustomWidget)

set_target_properties(${NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(${NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Xml
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
