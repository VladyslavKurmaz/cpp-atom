
# Project's structure

#add_definitions(-DUNICODE -D_UNICODE)

add_executable( 
	console
	"./pch.cpp"
	"./appl.cpp"
	"./console.cpp"
	"./frame.cpp"
	"./pref.cpp"
	"./window.cpp"
	"./bridge.cpp"
	"./shell.cpp"
	"./ad.cpp"
	"./atlas.cpp"
	"./panel.cpp"
	"./console.rc"
)

add_executable( 
	proxy
	"./pch.cpp"
	"./proxy.cpp"
	"./bridge.cpp"
)