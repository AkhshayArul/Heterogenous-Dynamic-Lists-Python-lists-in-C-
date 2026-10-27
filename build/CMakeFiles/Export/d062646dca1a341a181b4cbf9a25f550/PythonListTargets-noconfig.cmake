#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "PythonList::PythonList" for configuration ""
set_property(TARGET PythonList::PythonList APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(PythonList::PythonList PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libPythonList.a"
  )

list(APPEND _cmake_import_check_targets PythonList::PythonList )
list(APPEND _cmake_import_check_files_for_PythonList::PythonList "${_IMPORT_PREFIX}/lib/libPythonList.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
