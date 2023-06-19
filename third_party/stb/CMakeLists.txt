include(${CMAKE_CURRENT_SOURCE_DIR}/../../common_defs.cmake)

file(
	GLOB IMGUI_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/impl.c
)

add_library(stb_image STATIC ${IMGUI_SOURCES})
target_include_directories(stb_image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
