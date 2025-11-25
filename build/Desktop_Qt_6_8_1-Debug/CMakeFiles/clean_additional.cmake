# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "ui/CMakeFiles/moonlander_ui_autogen.dir/AutogenUsed.txt"
  "ui/CMakeFiles/moonlander_ui_autogen.dir/ParseCache.txt"
  "ui/moonlander_ui_autogen"
  )
endif()
