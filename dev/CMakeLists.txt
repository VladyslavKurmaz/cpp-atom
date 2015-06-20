
#set(CMAKE_CXX_FLAGS_RELEASE "/MT")
#set(CMAKE_CXX_FLAGS_DEBUG "/MTd")

# Project's structure
add_executable( 
	dev
	"./pch.cpp"
	"./dev.cpp"
	"./appl.cpp"
)
