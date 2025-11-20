# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/MoonLanderUI_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/MoonLanderUI_autogen.dir/ParseCache.txt"
  "MoonLanderUI_autogen"
  )
endif()
