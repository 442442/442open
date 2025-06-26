cmake_minimum_required(VERSION 3.16)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(CustomWidgetProjects LANGUAGES CXX VERSION 1.5)

#------------------------------------------------------

#                   group macro

#------------------------------------------------------
macro(source_group_by_dir source_files)       
	set(sgbd_cur_dir ${CMAKE_CURRENT_SOURCE_DIR}/)
	foreach(sgbd_file ${${source_files}})
		string(REGEX REPLACE ${sgbd_cur_dir} "" sgbd_fpath ${sgbd_file})
		string(REGEX REPLACE "[^/\\\\]+$" "" sgbd_group_name ${sgbd_fpath})
		if(sgbd_group_name)
                    string(REPLACE "/"   "\\\\"  sgbd_group_name_normal ${sgbd_group_name})
                    # if(${sgbd_file} MATCHES ".cpp$")
                    #     string(REPLACE "/"   "/Source Files"  sgbd_group_name_normal ${sgbd_group_name})
                    # elseif(${sgbd_file} MATCHES ".h$")
                    #     string(REPLACE "/"   "/Header Files"  sgbd_group_name_normal ${sgbd_group_name})
                    # elseif(${sgbd_file} MATCHES ".ui$")
                    #     string(REPLACE "/"   "/Form Files"  sgbd_group_name_normal ${sgbd_group_name})
                    # else()
                    #    string(REPLACE "/"   "\\\\"  sgbd_group_name_normal ${sgbd_group_name})
                    # endif()
                    source_group(${sgbd_group_name_normal} FILES ${sgbd_file})
		endif(sgbd_group_name)	
	endforeach(sgbd_file)
endmacro(source_group_by_dir)

#------------------------------------------------------

#                   build option

#------------------------------------------------------

option(BUILD_TEST "Build test_demo" ON)
option(BUILD_WITH_HALCON "build with Halcon" ON)
option(BUILD_WITH_VTK "build with vtk" ON)

set_property(GLOBAL PROPERTY USE_HALCON ${BUILD_WITH_HALCON})
set_property(GLOBAL PROPERTY USE_VTK ${BUILD_WITH_VTK})

if(BUILD_TEST)
    message(STATUS "build test_demo")
endif()

if(BUILD_WITH_HALCON)
    message(STATUS "build with halcon")
else()
    message(STATUS "build without halcon")
endif()

if(BUILD_WITH_VTK)
    message(STATUS "build with vtk")
else()
    message(STATUS "build without vtk")
endif()

#------------------------------------------------------

#                   add projects

#------------------------------------------------------

if(BUILD_TEST)
    add_subdirectory(test)
endif()

add_subdirectory(MyCustomWidget)
