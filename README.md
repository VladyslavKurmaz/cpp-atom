cpp-atom
========

structure
========
* atom
** cint   integration with CInt
** cons   Windows consoles integrator
** dpm    Development package management
** node   headers-only libraries for cross-ref smart pointers
** util
** zoom   cross-palform 2d/3d/ag engine  
* doc
* libs

portability
========
* mcvc9 | msvc10 | msvc11 | cygwin (gcc-4.5.3)
* x32 | x64
* boost-1.53.0 | boost-1.54.0
* loki-0.1.7 

external dependencies
========
* boost 	http://boost.org
* loki		http://loki-lib.sourceforge.net/
* CInt		http://root.cern.ch/drupal/content/cint
* CMake     http://cmake.org/

todo
========
- restore CInt build and samples
- build tests with msvc10|msvc11, x32|x64

cons:
- hotkey definition with char (not only vk codes)
+ parse hotkey strings
- commands' history
- CLI for cons configuration
- states
- self test on start - check hotkeys
- close frame
- rotate frame
- console mapping between two processes
- share keyboard state between main application andchild console
- ctrl+break, ctr+c, correct exit after pressing prev combinations
- close from all processes in chain
- languages other than english
- run param +caption, +prefix, +nocolor
- hide consd inside cons resource
- ctrl+c, ctrl+v, ctrl+ins, shift+ins
- process mouse events


build
========
