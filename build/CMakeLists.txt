cmake_minimum_required(VERSION 3.1)
project(pmEngine)

set (pmEngine_VERSION_MAJOR 1)
set (pmEngine_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SRC
	"../src/*.hpp"
	"../src/*.cpp"
)
file(GLOB SRC_pm
	"../src/pm/*.hpp"
	"../src/pm/*.cpp"
)
file(GLOB SRC_pm_nodes
	"../src/pm/nodes/*.hpp"
	"../src/pm/nodes/*.cpp"
)
file(GLOB SRC_imgui
	"../extlibs/include/imgui/*.hpp"
	"../extlibs/include/imgui/*.cpp"
)
file(GLOB SRC_glad
	"../extlibs/include/glad/*.h"
	"../extlibs/include/glad/*.c"
)

add_executable(pmEngine ${SRC} ${SRC_pm} ${SRC_pm_nodes} ${SRC_imgui} ${SRC_glad})

include_directories(pmEngine "../src")
source_group("imgui" FILES ${SRC_imgui})
source_group("glad" FILES ${SRC_glad})
source_group("pm" FILES ${SRC_pm})
source_group("pm\\nodes" FILES ${SRC_pm_nodes})
source_group("game" FILES ${SRC})

if(WIN32)
	include_directories(pmEngine ../extlibs/include)
	target_link_libraries (pmEngine opengl32)
endif()

if(APPLE)
	include_directories(pmEngine ../extlibs/include)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Cocoa -framework OpenGL -framework IOKit -L../extlibs/libs-mac -lglfw3")
endif()