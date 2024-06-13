cmake_minimum_required(VERSION 3.16)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(CustomWidgetProjects LANGUAGES CXX)

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

#                   add projects

#------------------------------------------------------

add_subdirectory(test)

add_subdirectory(MyCustomWidget)
