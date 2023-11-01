#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "djiosdk-core" for configuration "Release"
set_property(TARGET djiosdk-core APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(djiosdk-core PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "C:/Program Files (x86)/onboardsdk/lib/djiosdk-core.lib"
  )

list(APPEND _cmake_import_check_targets djiosdk-core )
list(APPEND _cmake_import_check_files_for_djiosdk-core "C:/Program Files (x86)/onboardsdk/lib/djiosdk-core.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
