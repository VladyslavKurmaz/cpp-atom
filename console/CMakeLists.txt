
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
	"./ar.cpp"
	"./panel.cpp"
)

add_executable( 
	proxy
	"./pch.cpp"
	"./proxy.cpp"
	"./bridge.cpp"
)