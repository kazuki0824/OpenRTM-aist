cmake_minimum_required (VERSION 3.0.2)

project (ArtExecutionContext
	VERSION ${RTM_VERSION}
	LANGUAGES CXX)

link_directories(${ORB_LINK_DIR})
add_definitions(${ORB_C_FLAGS_LIST})
add_definitions(${COIL_C_FLAGS_LIST})
if(WIN32)
	add_definitions(-DRTM_SKEL_IMPORT_SYMBOL)
endif()


set(target ArtExecutionContext)

set(srcs ArtExecutionContext.cpp ArtExecutionContext.h)


if(VXWORKS AND NOT RTP)
	set(libs ${RTCSKEL_PROJECT_NAME})

	add_executable(${target} ${srcs})
	openrtm_common_set_compile_props(${target})
	openrtm_include_rtm(${target})
	target_link_libraries(${target} ${libs})
	add_dependencies(${target} ${RTM_PROJECT_NAME})

	install(TARGETS ${target} LIBRARY DESTINATION ${INSTALL_RTM_EXT_DIR}/ec
				ARCHIVE DESTINATION ${INSTALL_RTM_EXT_DIR}/ec
				RUNTIME DESTINATION ${INSTALL_RTM_EXT_DIR}/ec
				COMPONENT ext)

else()
	set(libs ${RTM_PROJECT_NAME} ${ORB_LIBRARIES} ${COIL_PROJECT_NAME} ${DATATYPE_FACTORIES})


	add_library(${target} SHARED ${srcs})
	openrtm_common_set_compile_props(${target})
	openrtm_include_rtm(${target})
	target_link_libraries(${target} ${libs} ${RTM_LINKER_OPTION})
	add_dependencies(${target} ${RTM_PROJECT_NAME})
	set_target_properties(${target} PROPERTIES PREFIX "")


	install(TARGETS ${target} LIBRARY DESTINATION ${INSTALL_RTM_EXT_DIR}/ec
				ARCHIVE DESTINATION ${INSTALL_RTM_EXT_DIR}/ec
				RUNTIME DESTINATION ${INSTALL_RTM_EXT_DIR}/ec
				COMPONENT ext)
endif()






if(VXWORKS)
	if(RTP)
	else(RTP)	
		set_target_properties(${target} PROPERTIES SUFFIX ".out")
	endif(RTP)
endif(VXWORKS)



