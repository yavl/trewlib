cmake_minimum_required(VERSION 3.1)
project(Trewlib)

set (Trewlib_VERSION_MAJOR 1)
set (Trewlib_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SRC
	"../src/*.hpp"
	"../src/*.cpp"
)
file(GLOB SRC_trew
	"../src/trew/*.hpp"
	"../src/trew/*.cpp"
)
file(GLOB SRC_trew_nodes
	"../src/trew/nodes/*.hpp"
	"../src/trew/nodes/*.cpp"
)
file(GLOB SRC_trew_actions
	"../src/trew/actions/*.hpp"
	"../src/trew/actions/*.cpp"
)
file(GLOB SRC_trew_input
	"../src/trew/input/*.hpp"
	"../src/trew/input/*.cpp"
)
file(GLOB SRC_trew_input_impl
	"../src/trew/input/impl/*.hpp"
	"../src/trew/input/impl/*.cpp"
)
file(GLOB SRC_imgui
	"../extlibs/include/imgui/*.hpp"
	"../extlibs/include/imgui/*.cpp"
)
file(GLOB SRC_glad
	"../extlibs/include/glad/*.h"
	"../extlibs/include/glad/*.c"
)

add_executable(Trewlib ${SRC} ${SRC_trew} ${SRC_trew_nodes} ${SRC_trew_actions} ${SRC_trew_input} ${SRC_trew_input_impl} ${SRC_imgui} ${SRC_glad})

include_directories(Trewlib "../src")
source_group("imgui" FILES ${SRC_imgui})
source_group("glad" FILES ${SRC_glad})
source_group("trew" FILES ${SRC_trew})
source_group("trew\\nodes" FILES ${SRC_trew_nodes})
source_group("trew\\actions" FILES ${SRC_trew_actions})
source_group("trew\\input" FILES ${SRC_trew_input})
source_group("trew\\input\\impl" FILES ${SRC_trew_input_impl})
source_group("game" FILES ${SRC})


include_directories(Trewlib ../extlibs/include)
if(WIN32)
	target_link_libraries(Trewlib opengl32)
endif()

if(APPLE)
	include_directories(Trewlib /usr/local/include)
	link_directories(Trewlib /usr/local/lib)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Cocoa -framework OpenGL -framework IOKit")
endif()

if(UNIX AND NOT APPLE)
	target_link_libraries(Trewlib dl)
endif()

target_link_libraries(Trewlib spdlog glfw)
