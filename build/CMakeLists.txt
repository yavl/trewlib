cmake_minimum_required(VERSION 3.10)
project(Trewlib)

enable_language(ASM)

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
file(GLOB SRC_trew_actions
	"../src/trew/actions/*.hpp"
	"../src/trew/actions/*.cpp"
)
file(GLOB SRC_trew_app
	"../src/trew/app/*.hpp"
	"../src/trew/app/*.cpp"
)
file(GLOB SRC_trew_app_impl_glfw
	"../src/trew/app/impl_glfw/*.hpp"
	"../src/trew/app/impl_glfw/*.cpp"
)
file(GLOB SRC_trew_drawables
	"../src/trew/drawables/*.hpp"
	"../src/trew/drawables/*.cpp"
)
file(GLOB SRC_trew_drawables_impl_glfw
	"../src/trew/drawables/impl_glfw/*.hpp"
	"../src/trew/drawables/impl_glfw/*.cpp"
)
file(GLOB SRC_trew_input
	"../src/trew/input/*.hpp"
	"../src/trew/input/*.cpp"
)
file(GLOB SRC_trew_input_impl_glfw
	"../src/trew/input/impl_glfw/*.hpp"
	"../src/trew/input/impl_glfw/*.cpp"
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
file(GLOB SRC_glad
	"../extlibs/include/glad/*.h"
	"../extlibs/include/glad/*.c"
)
file(GLOB_RECURSE SRC_angelscript
	"../extlibs/include/angelscript/*.h"
	"../extlibs/include/angelscript/*.cpp"
)

if(WIN32)
	file(GLOB SRC_angelscript_obj
		"../extlibs/libs-msvc/x64/as_callfunc_x64_msvc_asm.obj"
	)
	add_executable(Trewlib ${SRC} ${SRC_trew} ${SRC_trew_actions} ${SRC_trew_app} ${SRC_trew_app_impl_glfw} ${SRC_trew_drawables} ${SRC_trew_drawables_impl_glfw} ${SRC_trew_input} ${SRC_trew_input_impl_glfw} ${SRC_trew_nodes} ${SRC_trew_scripting} ${SRC_imgui} ${SRC_glad} ${SRC_angelscript} ${SRC_angelscript_obj})
elseif(APPLE)
	file(GLOB SRC_angelscript_obj
		"../extlibs/libs-mac/as_callfunc_arm64_xcode.S"
	)
	add_executable(Trewlib ${SRC} ${SRC_trew} ${SRC_trew_actions} ${SRC_trew_app} ${SRC_trew_app_impl_glfw} ${SRC_trew_drawables} ${SRC_trew_drawables_impl_glfw} ${SRC_trew_input} ${SRC_trew_input_impl_glfw} ${SRC_trew_nodes} ${SRC_trew_scripting} ${SRC_imgui} ${SRC_glad} ${SRC_angelscript} ${SRC_angelscript_obj})
else()
	add_executable(Trewlib ${SRC} ${SRC_trew} ${SRC_trew_actions} ${SRC_trew_app} ${SRC_trew_app_impl_glfw} ${SRC_trew_drawables} ${SRC_trew_drawables_impl_glfw} ${SRC_trew_input} ${SRC_trew_input_impl_glfw} ${SRC_trew_nodes} ${SRC_trew_scripting} ${SRC_imgui} ${SRC_glad} ${SRC_angelscript})
endif()

include_directories(Trewlib "../src")
source_group("angelscript" FILES ${SRC_angelscript})
source_group("imgui" FILES ${SRC_imgui})
source_group("glad" FILES ${SRC_glad})
source_group("trew" FILES ${SRC_trew})
source_group("trew\\actions" FILES ${SRC_trew_actions})
source_group("trew\\app" FILES ${SRC_trew_app})
source_group("trew\\app\\impl_glfw" FILES ${SRC_trew_app_impl_glfw})
source_group("trew\\drawables" FILES ${SRC_trew_drawables})
source_group("trew\\drawables\\impl_glfw" FILES ${SRC_trew_drawables_impl_glfw})
source_group("trew\\input" FILES ${SRC_trew_input})
source_group("trew\\input\\impl_glfw" FILES ${SRC_trew_input_impl_glfw})
source_group("trew\\nodes" FILES ${SRC_trew_nodes})
source_group("trew\\scripting" FILES ${SRC_trew_scripting})
source_group("game" FILES ${SRC})


include_directories(Trewlib ../extlibs/include ../extlibs/include/angelscript/addons)
if(WIN32)
	target_link_libraries(Trewlib opengl32 glfw3 freetype SDL3)
endif()

if(UNIX)
	if(APPLE)
		include_directories(Trewlib /opt/homebrew/include)
		include_directories(Trewlib /opt/homebrew/include/freetype2)
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Cocoa -framework OpenGL -framework IOKit -L/opt/homebrew/lib")
	else() # Linux
		target_link_libraries(Trewlib dl)
	endif()
	
	target_link_libraries(Trewlib glfw freetype)
endif()