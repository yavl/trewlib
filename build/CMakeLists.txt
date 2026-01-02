cmake_minimum_required(VERSION 3.10)
project(Trewlib)

enable_language(ASM)

set (Trewlib_VERSION_MAJOR 1)
set (Trewlib_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SRC
	"../src/*.hpp"
	"../src/*.cpp"
)
file(GLOB SRC_ecs_components
	"../src/ecs/components/*.hpp"
	"../src/ecs/components/*.cpp"
)
file(GLOB SRC_ecs_systems
	"../src/ecs/systems/*.hpp"
	"../src/ecs/systems/*.cpp"
)
file(GLOB SRC_ui
	"../src/ui/*.hpp"
	"../src/ui/*.cpp"
)
file(GLOB SRC_trew
	"../src/trew/*.hpp"
	"../src/trew/*.cpp"
)
file(GLOB SRC_trew_actions
	"../src/trew/actions/*.hpp"
	"../src/trew/actions/*.cpp"
)
file(GLOB SRC_trew_app
	"../src/trew/app/*.hpp"
	"../src/trew/app/*.cpp"
)
file(GLOB SRC_trew_drawables
	"../src/trew/drawables/*.hpp"
	"../src/trew/drawables/*.cpp"
)
file(GLOB SRC_trew_graphics
	"../src/trew/graphics/*.hpp"
	"../src/trew/graphics/*.cpp"
)
file(GLOB SRC_trew_graphics
	"../src/trew/graphics/*.hpp"
	"../src/trew/graphics/*.cpp"
)
file(GLOB SRC_trew_input
	"../src/trew/input/*.hpp"
	"../src/trew/input/*.cpp"
)
file(GLOB SRC_trew_net
	"../src/trew/net/*.hpp"
	"../src/trew/net/*.cpp"
)
file(GLOB SRC_trew_nodes
	"../src/trew/nodes/*.hpp"
	"../src/trew/nodes/*.cpp"
)
file(GLOB SRC_trew_scripting
	"../src/trew/scripting/*.hpp"
	"../src/trew/scripting/*.cpp"
)
file(GLOB SRC_imgui
	"../extlibs/include/imgui/*.hpp"
	"../extlibs/include/imgui/*.cpp"
)
file(GLOB_RECURSE SRC_angelscript
	"../extlibs/include/angelscript/*.h"
	"../extlibs/include/angelscript/*.cpp"
)

if(WIN32)
	if(MSVC)
	    add_compile_options("/utf-8")
	endif()
	file(GLOB SRC_angelscript_obj
		"../extlibs/libs-msvc/x64/as_callfunc_x64_msvc_asm.obj"
	)
	add_executable(Trewlib ${SRC} ${SRC_trew} ${SRC_ecs_components} ${SRC_ecs_systems} ${SRC_ui} ${SRC_trew_actions} ${SRC_trew_app} ${SRC_trew_drawables} ${SRC_trew_graphics} ${SRC_trew_input} ${SRC_trew_net} ${SRC_trew_nodes} ${SRC_trew_scripting} ${SRC_imgui} ${SRC_angelscript} ${SRC_angelscript_obj})
elseif(APPLE)
	file(GLOB SRC_angelscript_obj
		"../extlibs/libs-mac/as_callfunc_arm64_xcode.S"
	)
	add_executable(Trewlib ${SRC} ${SRC_trew} ${SRC_ecs_components} ${SRC_ecs_systems} ${SRC_ui} ${SRC_trew_actions} ${SRC_trew_app} ${SRC_trew_drawables} ${SRC_trew_graphics} ${SRC_trew_input} ${SRC_trew_net} ${SRC_trew_nodes} ${SRC_trew_scripting} ${SRC_imgui} ${SRC_angelscript} ${SRC_angelscript_obj})
else()
	add_executable(Trewlib ${SRC} ${SRC_trew} ${SRC_ecs_components} ${SRC_ecs_systems} ${SRC_ui} ${SRC_trew_actions} ${SRC_trew_app} ${SRC_trew_drawables} ${SRC_trew_graphics} ${SRC_trew_input} ${SRC_trew_net} ${SRC_trew_nodes} ${SRC_trew_scripting} ${SRC_imgui} ${SRC_angelscript})
endif()

include_directories(Trewlib "../src")
source_group("angelscript" FILES ${SRC_angelscript})
source_group("imgui" FILES ${SRC_imgui})
source_group("trew" FILES ${SRC_trew})
source_group("trew\\actions" FILES ${SRC_trew_actions})
source_group("trew\\app" FILES ${SRC_trew_app})
source_group("trew\\drawables" FILES ${SRC_trew_drawables})
source_group("trew\\graphics" FILES ${SRC_trew_graphics})
source_group("trew\\input" FILES ${SRC_trew_input})
source_group("trew\\net" FILES ${SRC_trew_net})
source_group("trew\\nodes" FILES ${SRC_trew_nodes})
source_group("trew\\scripting" FILES ${SRC_trew_scripting})
source_group("game" FILES ${SRC})
source_group("game\\ecs\\components" FILES ${SRC_ecs_components})
source_group("game\\ecs\\systems" FILES ${SRC_ecs_systems})
source_group("game\\ui" FILES ${SRC_ui})


include_directories(Trewlib ../extlibs/include ../extlibs/include/angelscript/addons)
if(WIN32)
	target_link_libraries(Trewlib freetype SDL3 SDL3_image SDL3_ttf enet flecs ws2_32 winmm)
endif()

if(UNIX)
	if(APPLE)
		include_directories(Trewlib /opt/homebrew/include)
		include_directories(Trewlib /opt/homebrew/include/freetype2)
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/opt/homebrew/lib -L../extlibs/libs-mac")
	else() # Linux
		target_link_libraries(Trewlib dl)
	endif()
	
	target_link_libraries(Trewlib freetype SDL3 SDL3_image SDL3_ttf enet flecs)
endif()
