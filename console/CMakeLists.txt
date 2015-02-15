
# Project's structure

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
)

add_executable( 
	proxy
	"./pch.cpp"
	"./proxy.cpp"
	"./bridge.cpp"
)