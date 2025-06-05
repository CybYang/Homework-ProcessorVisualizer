# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\HardwareVis_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\HardwareVis_autogen.dir\\ParseCache.txt"
  "HardwareVis_autogen"
  )
endif()
