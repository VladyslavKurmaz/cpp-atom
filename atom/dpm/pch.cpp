#include "./pch.hpp"

int atom::node_cnt = 0;


// inline UML diagrams
// http://yuml.me/diagram/scruffy/class/[appl]-1[logger],[appl]1-0..1[env],[env]-1[logger],[env]*-0..1[env],[env]1-*[comp],[comp]-1[logger],[comp]-1[appl]


// Known issues
//+[1] - If DPM2_HOME environment variable is not defined - application crashed
//o[2] - returns invalid value on x64 if you are x86 process http://stackoverflow.com/questions/1738985/why-processor-architecture-always-returns-x86-instead-of-amd64
//+[3] - change environmwnt output [/] "d:\work2" -> [/@d:\work2]
//o[4] - remove constraints for root environment, add environment creation command

/* commands
help				print help message
cd <env>			change current environment
ls [-v]				print environment hierarchy, -v with components
sync [-r]			update components system info, from .dpm folder
status [-v] [-r]	print status about environment repository, [-r] recursive, [-v] - also check components repos
<component> <stage> [-y][-t][-n][-a][-c]
			sync
			conf
			build

*/
/* environment template
 /
 |-/dpm
 | |-* catalog.conf - contains list of components including initial configuration for them: repository, user, pass, additional folder etc.
 |
 |-/boost-1.55.0
 |
 |-/cpp-atom
 | |-/builds
 | |-/src
 |   |-* dpm.conf - additional component configuration script, act as clarification for catalog.json from embracing environment
 |
 |-* dpm.conf - json file, that points to repository for environment, also marks folder as environment not component
 |
 */
