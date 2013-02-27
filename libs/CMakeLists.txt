
add_definitions(-D_WIN32_WINNT=0x0501)
add_definitions(-D__USE_W32_SOCKETS)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTI_THREADED ON)


add_subdirectory ( "./cint" )
add_subdirectory ( "./node" )
