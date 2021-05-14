set(EXE_NAME EssenceAuth)

CollectSourceFiles(
  ${CMAKE_CURRENT_SOURCE_DIR}
  EXE_SRCS)
  
GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${EXE_NAME} ${EXE_SRCS})

set_target_properties(
    ${EXE_NAME} 
    PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY 
    ${CMAKE_INSTALL_PREFIX})
	
CollectIncludeDirectories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  EXE_HDRS)
  
target_include_directories(
    ${EXE_NAME}
	PUBLIC
		EssenceCore
		EssenceUtils
		${EXE_HDRS}
		${CMAKE_SOURCE_DIR}/dep
    ${Boost_INCLUDE_DIRS})

target_link_libraries(
    ${EXE_NAME}
	EssenceCore
	EssenceUtils
	${Boost_LIBRARIES}
	bcrypt
)