#defs

#add_definitions(-DUNICODE -D_UNICODE)

# Inc/Lib

# Project's structure
add_executable( 
	cons
	"./pch.cpp"
	"./appl.cpp"
	"./cons.cpp"
	"./frame.cpp"
	"./pref.cpp"
	"./window.cpp"
	"./process.cpp"
)