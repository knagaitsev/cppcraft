cmake_minimum_required(VERSION 2.8)

project(cppcraft)

FILE(GLOB SOURCE_FILES src/** src/World/** src/Render/** src/Shaders/** src/Texture/**)

FILE(GLOB JSONCPP_SOURCE_FILES deps/jsoncpp/dist/**)

FILE(GLOB PROCTREE_SOURCE_FILES deps/proctree/**)

add_executable(
    cppcraft
    ${SOURCE_FILES}
	${JSONCPP_SOURCE_FILES}
	${PROCTREE_SOURCE_FILES}
	deps/lodepng/lodepng.cpp)
	
# add_subdirectory(deps/glfw)
# include_directories(deps/glfw/include)

include_directories(deps/glm)

include_directories(deps/lodepng)

include_directories(deps/proctree)

include_directories(deps/jsoncpp/include)

include_directories(deps/libnoise/include)

find_package(OpenGL REQUIRED)

include_directories(deps/glew/include)
