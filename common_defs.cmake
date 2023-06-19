# Some Common Definitions...

add_definitions(-DIMGUI_DISABLE_OBSOLETE_FUNCTIONS=1)
add_definitions(-DIMGUI_DISABLE_OBSOLETE_KEYIO=1)

# Debug C/C++ flags
if(CMAKE_BUILD_TYPE STREQUAL Debug)
	target_compile_options(asciipen PRIVATE -fsanitize=address)
	target_link_options(asciipen PRIVATE -fsanitize=address)
	target_compile_definitions(asciipen PRIVATE -D_DEBUG)
else()
	target_compile_definitions(asciipen PRIVATE -D_NDEBUG)
endif()

# Enable All Warnings
if(MSVC)
	target_compile_options(asciipen PRIVATE /W4 /Wall)
else()
	target_compile_options(asciipen PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(CMAKE_CXX_BYTE_ORDER STREQUAL "BIG_ENDIAN")
	target_compile_definitions(asciipen PRIVATE -DTARGET_IS_BIGENDIAN=1)
endif()

if(WIN32)
	target_compile_definitions(asciipen PRIVATE -DTARGET_WINDOWS)
elseif(CYGWIN)
	target_compile_definitions(asciipen PRIVATE -DTARGET_WINDOWS)
	target_compile_definitions(asciipen PRIVATE -DTARGET_CYGWIN)
elseif(LINUX)
	target_compile_definitions(asciipen PRIVATE -DTARGET_LINUX)
elseif(BSD)
	target_compile_definitions(asciipen PRIVATE -DTARGET_BSD)
elseif(APPLE)
	target_compile_definitions(asciipen PRIVATE -DTARGET_APPLE)
else()
	if(ALLOW_UNKNOWN_TARGET)
		target_compile_definitions(asciipen PRIVATE -DTARGET_UNKNOWN)
	else()
		message(FATAL_ERROR "Unknown Target System: \"" ${CMAKE_SYSTEM} "\"!")
	endif()
endif()
