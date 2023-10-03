add_library(stb_image STATIC ${CMAKE_CURRENT_SOURCE_DIR}/impl.c)
target_include_directories(stb_image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)

