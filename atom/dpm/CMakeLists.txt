
#set(CMAKE_CXX_FLAGS_RELEASE "/MT")
#set(CMAKE_CXX_FLAGS_DEBUG "/MTd")

# Project's structure
add_executable( 
	dpm
	"./pch.cpp"
	"./appl.cpp"
	"./comp.cpp"
	"./env.cpp"
	"./dpm.cpp"
	"./util.cpp"
	"./context.cpp"
	"./filter.cpp"
)
