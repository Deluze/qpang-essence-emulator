set(LIB_NAME tnl)

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
	
CollectIncludeDirectories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  
  LIB_HDRS)
 
target_include_directories(
    ${LIB_NAME}
	PUBLIC
		${LIB_HDRS}
		${CMAKE_SOURCE_DIR}/dependencies/libtomcrypt)
