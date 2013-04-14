cpp-atom
========

structure
========
atom
* cint   integration with CInt
* cons   Windows consoles integrator
* node   headers-only libraries for cross-ref smart pointers
* util
    
portability
========
* mcvc9 | msvc10 | cygwin (gcc-4.5.3)
* x32 | x64
* boost-1.49.0 | boost-1.50.0 | boost-1.51.0 | boost-1.52.0 | boost-1.53.0
* loki-0.1.7 

external dependencies
========
* boost 	http://boost.org
* loki		http://loki-lib.sourceforge.net/
* CInt		http://root.cern.ch/drupal/content/cint

todo
========
- restore CInt build and samples
- build tests with msvc10|msvc12, x32|x64

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


build
========
