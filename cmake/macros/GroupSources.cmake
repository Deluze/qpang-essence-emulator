# Copyright (C) 2008-2017 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

macro(GroupSources dir)
   # Include all header and c files
   file(GLOB_RECURSE elements RELATIVE ${dir} *.h *.hpp *.c *.cpp *.cc *.ui)

   foreach(element ${elements})
     # Extract filename and directory
     get_filename_component(element_name ${element} NAME)
     get_filename_component(element_dir ${element} DIRECTORY)
     if (NOT ${element_dir} STREQUAL "")
        # Use the full hierarchical structure to build source_groups.
        string(REPLACE "/" "\\" group_name ${element_dir})
        source_group("${group_name}" FILES ${dir}/${element})
     else()
       # If the file is in the root directory, place it in the root source_group.
       source_group("\\" FILES ${dir}/${element})
     endif()
   endforeach()
endmacro()