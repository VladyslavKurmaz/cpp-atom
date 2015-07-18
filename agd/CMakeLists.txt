foreach(flag_var
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
   if(${flag_var} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
   endif(${flag_var} MATCHES "/MD")
endforeach(flag_var)

add_executable( 
  agd
  "./pch.cpp"
  "./appl.cpp"
  "./agd.cpp"
  "./frame.cpp"
  "./pref.cpp"
  "./window.cpp"
  "./bridge.cpp"
  "./shell.cpp"
  "./ad.cpp"
  "./am.cpp"
  "./atlas.cpp"
  "./panel.cpp"
  "./agd.rc"
)

add_executable( 
  proxy
  "./pch.cpp"
  "./proxy.cpp"
  "./bridge.cpp"
)