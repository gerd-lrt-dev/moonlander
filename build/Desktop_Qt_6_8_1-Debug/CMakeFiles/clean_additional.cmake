# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "ui/CMakeFiles/Moonlander_UI_autogen.dir/AutogenUsed.txt"
  "ui/CMakeFiles/Moonlander_UI_autogen.dir/ParseCache.txt"
  "ui/Moonlander_UI_autogen"
  )
endif()
