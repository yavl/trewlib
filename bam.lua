settings = NewSettings()
settings.cc.flags_cxx:Add("-std=c++14")

if platform == "macosx" then
	settings.link.libs:Add("glfw")
end

if platform == "win32" then
	settings.link.libs:Add("glfw3")
	settings.link.libs:Add("gdi32")
end

if platform == "linux" then
	settings.link.libs:Add("glfw")
	settings.link.libs:Add("dl")
end

source = Collect("src/*.cpp", "src/pm/*.cpp", "src/pm/external/glad/*.c", "src/pm/external/imgui/*.cpp")
objects = Compile(settings, source)
exe = Link(settings, "bin/main", objects)
