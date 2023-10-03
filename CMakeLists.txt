cmake_minimum_required(VERSION 3.24)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(BUILD_APPLE_BUNDLE "Build Apple Bundle, Used For Releases & Is Ignored On Non-Apple Target" OFF)

project(asciipen C CXX)
file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.c)

# On Windows, Enable RC Language & Add It To Sources
if(WIN32)
	enable_language(RC)
	set(SRC_FILES ${SRC_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/data/windows.rc)
endif()

if(APPLE AND BUILD_APPLE_BUNDLE)
	add_executable(asciipen MACOSX_BUNDLE ${SRC_FILES})
	set_source_files_properties(
		${CMAKE_CURRENT_SOURCE_DIR}/asciipen.icns PROPERTIES
		MACOSX_PACKAGE_LOCATION "Resources"
	)
	set_target_properties(
		asciipen PROPERTIES BUNDLE True
		MACOSX_BUNDLE_BUNDLE_NAME "asciipen"                  # CFBundleName
		MACOSX_BUNDLE_BUNDLE_VERSION "0.0.1"                  # CFBundleVersion
		MACOSX_BUNDLE_LONG_VERSION_STRING "0.0.1"             # CFBundleLongVersionString
		MACOSX_BUNDLE_SHORT_VERSION_STRING "0.0.1"            # CFBundleShortVersionString
		MACOSX_BUNDLE_COPYRIGHT "adityaraj"                   # NSHumanReadableCopyright
		MACOSX_BUNDLE_GUI_IDENTIFIER "pegvin.asciipen.github" # CFBundleIdentifier
		MACOSX_BUNDLE_ICON_FILE "asciipen.icns"               # CFBundleIconFile
		MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/data/Info.plist
	)
else()
	add_executable(asciipen ${SRC_FILES})
endif()

if(WIN32)
	add_custom_command(TARGET asciipen POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:asciipen> $<TARGET_RUNTIME_DLLS:asciipen>
		COMMAND_EXPAND_LISTS
	)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/imbase/)
target_link_libraries(asciipen PUBLIC imbase)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb/)
target_link_libraries(asciipen PUBLIC stb_image)

target_include_directories(asciipen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)

# Use Address Sanitizer in Debug Mode
if(CMAKE_BUILD_TYPE STREQUAL Debug)
	if(NOT (WIN32 OR CYGWIN))
		target_compile_options(asciipen PRIVATE -fsanitize=address)
		target_link_options(asciipen PRIVATE -fsanitize=address)
	endif()
endif()

target_compile_options(asciipen PRIVATE
	$<$<CXX_COMPILER_ID:MSVC>:/W4>
	$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

if(CMAKE_CXX_BYTE_ORDER STREQUAL "BIG_ENDIAN")
	target_compile_definitions(asciipen PRIVATE -DTARGET_IS_BIGENDIAN=1)
endif()

