# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/JamS8_FilmScannerUI_Qt6_Widgets_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/JamS8_FilmScannerUI_Qt6_Widgets_autogen.dir/ParseCache.txt"
  "JamS8_FilmScannerUI_Qt6_Widgets_autogen"
  )
endif()
