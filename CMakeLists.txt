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
	
add_subdirectory(deps/glfw)
include_directories(deps/glfw/include)

include_directories(deps/glm)

include_directories(deps/lodepng)

include_directories(deps/proctree)

include_directories(deps/jsoncpp/include)

include_directories(deps/libnoise/include)

FIND_LIBRARY(LIBNOISE_LIBRARIES
  NAMES libnoise noise libnoiseutils noiseutils
  PATHS
    deps/libnoise/bin    
  PATH_SUFFIXES lib64 lib so
)

find_package(OpenGL REQUIRED)

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

target_link_libraries(cppcraft ${GLEW_LIBRARY} ${OPENGL_LIBRARY} glfw ${GLFW_LIBRARIES} ${LIBNOISE_LIBRARIES})
