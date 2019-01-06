#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SDL2::SDL2-static" for configuration "Release"
set_property(TARGET SDL2::SDL2-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SDL2::SDL2-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "m;user32;gdi32;winmm;imm32;ole32;oleaut32;version;uuid;advapi32;shell32;dinput8;dxerr8;mingw32;-Wl,--no-undefined;-mwindows"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libSDL2-static.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS SDL2::SDL2-static )
list(APPEND _IMPORT_CHECK_FILES_FOR_SDL2::SDL2-static "${_IMPORT_PREFIX}/lib/libSDL2-static.a" )

# Import target "SDL2::SDL2" for configuration "Release"
set_property(TARGET SDL2::SDL2 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SDL2::SDL2 PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libSDL2.dll.a"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "m;user32;gdi32;winmm;imm32;ole32;oleaut32;version;uuid;advapi32;shell32;dinput8;dxerr8;mingw32;-Wl,--no-undefined;-mwindows"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/SDL2.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS SDL2::SDL2 )
list(APPEND _IMPORT_CHECK_FILES_FOR_SDL2::SDL2 "${_IMPORT_PREFIX}/lib/libSDL2.dll.a" "${_IMPORT_PREFIX}/bin/SDL2.dll" )

# Import target "SDL2::SDL2main" for configuration "Release"
set_property(TARGET SDL2::SDL2main APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SDL2::SDL2main PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libSDL2main.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS SDL2::SDL2main )
list(APPEND _IMPORT_CHECK_FILES_FOR_SDL2::SDL2main "${_IMPORT_PREFIX}/lib/libSDL2main.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
