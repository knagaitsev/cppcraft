cmake_minimum_required(VERSION 2.8)

project(cppcraft)

FILE(GLOB SOURCE_FILES src/** src/World/** src/Render/**)

FILE(GLOB JSONCPP_SOURCE_FILES deps/jsoncpp/dist/**)

add_executable(
    cppcraft
    ${SOURCE_FILES}
	${JSONCPP_SOURCE_FILES}
	deps/lodepng/lodepng.cpp)
	
add_subdirectory(deps/glfw)
include_directories(deps/glfw/include)

include_directories(deps/glm)

include_directories(deps/lodepng)

include_directories(deps/jsoncpp/include)

include_directories(deps/libnoise/include)

find_package(OpenGL REQUIRED)

include_directories(deps/glew/include)

if (MINGW)
    set(GLEW_LIBRARY "" CACHE FILEPATH deps/glew/lib/libglew32.a )
endif ()

#if (MSVC)
#	set(GLEW_LIBRARY "" CACHE FILEPATH deps/glew/lib/Release/Win32/glew32s.lib )
#endif ()

set(CMAKE_LIBRARY_PATH "deps/libnoise/bin/")
find_library(NOISE_LIBRARY NAMES LIBNOISE libnoise)

if (MSVC)

	SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
	SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
	SET (CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")

	set(CMAKE_PREFIX_PATH "deps/glew")
	set(CMAKE_LIBRARY_PATH "deps/glew/lib/Release/Win32/")

	find_path(GLEW_INCLUDE_DIR include)
	find_library(GLEW_LIBRARY NAMES GLEW glew32 glew glew32s PATH_SUFFIXES lib64)

	find_package(GLEW REQUIRED)
endif ()

target_link_libraries(cppcraft ${GLEW_LIBRARY} ${OPENGL_LIBRARY} glfw ${GLFW_LIBRARIES} ${NOISE_LIBRARY})
