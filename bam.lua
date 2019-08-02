Import("configure.lua")

--- Setup Config -------
config = NewConfig()
config:Add(OptCCompiler("compiler"))
config:Finalize("config.lua")

settings = NewSettings()

compiler = config.compiler.driver
if compiler == "gcc" then
	SetDriversGCC(settings)
elseif compiler == "clang" then
	SetDriversClang(settings)
else
	-- apply compiler settings
	config.compiler:Apply(settings)
	compiler = config.compiler.driver
end

if platform == "macosx" then
	settings.link.libs:Add("glfw")
end

if platform == "linux" then
	settings.cc.flags_cxx:Add("-std=c++17")
	settings.link.libs:Add("glfw")
	settings.link.libs:Add("dl")
end

if platform == "win32" then
	settings.cc.includes:Add("extlibs/include")
	if compiler == "gcc" then
		SetDriversGCC(settings)
		settings.cc.flags_cxx:Add("-std=c++17")
		settings.link.libs:Add("glfw3")
		settings.link.libs:Add("gdi32")
	elseif compiler == "clang" then
		SetDriversClang(settings)
	elseif compiler == "cl" then
		SetDriversCL(settings)
		settings.link.libpath:Add("extlibs/libs-msvc/x86")
		settings.link.libs:Add("glfw3dll")
	else
		-- apply compiler settings
		config.compiler:Apply(settings)
		compiler = config.compiler.driver
	end
end

source = Collect("src/*.cpp", "src/pm/*.cpp", "extlibs/include/glad/*.c", "extlibs/include/imgui/*.cpp")
objects = Compile(settings, source)
exe = Link(settings, "bin/main", objects)
