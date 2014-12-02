
#set(CMAKE_CXX_FLAGS_RELEASE "/MT")
#set(CMAKE_CXX_FLAGS_DEBUG "/MTd")

# Project's structure
add_executable( 
	dev
	"./pch.cpp"
	"./appl.cpp"
	"./comp.cpp"
	"./env.cpp"
	"./dev.cpp"
	"./util.cpp"
	"./context.cpp"
	"./filter.cpp"
	"./entity.cpp"
	"./repo.cpp"
)
