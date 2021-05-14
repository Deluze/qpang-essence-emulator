set(LIB_NAME EssenceUtils)

CollectSourceFiles(
  ${CMAKE_CURRENT_SOURCE_DIR}
  LIB_SRCS)

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_library(${LIB_NAME} STATIC ${LIB_SRCS})

set_target_properties(
    ${LIB_NAME} 
    PROPERTIES 
     ARCHIVE_OUTPUT_DIRECTORY 
    ${CMAKE_INSTALL_PREFIX})


set_target_properties(
    ${LIB_NAME} 
    PROPERTIES 
    LINKER_LANGUAGE 
    CXX)
	
CollectIncludeDirectories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  LIB_INCLUDES)
 
target_include_directories(
    ${LIB_NAME}
	PUBLIC
		${LIB_INCLUDES}
		${Boost_INCLUDE_DIRS}
		${MYSQL_LIB_DIR})

target_link_libraries(
  ${LIB_NAME}
PRIVATE
  ${MYSQL_LIBRARIES}
  ${Boost_LIBRARIES}
  spdlog::spdlog
)