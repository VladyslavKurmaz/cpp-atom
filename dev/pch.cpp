#include "./pch.hpp"

int atom::node_cnt = 0;



/* commands
help							print help message
cd <env>						change current environment
create <env> [-u] [-p] [-m]		create new environment as child of current
.			<action> [-v] [-r]	apply named action to the environment [-r] recursive, [-v] - also apply action to the all nested components
			info				print environment info
			sync				sync components repository, from .dpm folder (checkout, if there is no .dpm folder)
			status				print status about environment repository
<component> <action> [-r][-y][-t][-n][-a][-c]
			info
			sync				download, unpack, update repositories, 
			conf				update relations between components (conf apache, etc.) 
			build				build projects
			<custom step>

*/

/* environment template
 /
 |-/.dpm
 | |-* catalog.conf - contains list of components including initial configuration for them: repository, user, pass, additional folder etc.
 |
 |-/boost-1.55.0
 |
 |-/cpp-atom
 | |-/builds
 | |-/src
 |   |-* dpm.conf - additional component configuration script, provide details for catalog.json from embracing environment
 |
 |-* dpm.conf - json file, that points to repository for environment, also marks folder as environment not component
 |
 */

/* DSL for environment variables
"var":{
	"<name>":"<value>"
}
g - generate name or path, u - use name or path, m - modify name
[1] gg	"gg|SRC":"src"											-> SET DPM__CPP_ATOM_SRC=D:\work2\cpp-atom\src
[2] gu	"gu|9_vars":"%VS90COMNTOOLS%..\\..\\VC\\vcvarsall.bat"	-> SET DPM__MSVC_9_VARS=%VS90COMNTOOLS%..\\..\\VC\\vcvarsall.bat
[3] ug	"ug|CYGWIN":""											-> SET CYGWIN=D:\work2\cpp-atom\cygwin-32
[4] uu	"uu|ROOT":"d:\work2"									-> SET ROOT=d:\work2
[5] mg	"mg|PATH":"bin"											-> SET PATH=%PATH%;d:\work2\cmake\bin 
[6] mu	"mu|PATH":"d:\work2"									-> SET PATH=%PATH%;d:\work2

*/
