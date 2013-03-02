/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2011.03.08 - Created                                           |
|              2013.02.03 - has been moved to github/atom/util                |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
|                                                                             |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_UTIL_DBG_HPP
#define ATOM_UTIL_DBG_HPP

#ifdef _WINDOWS

#include <windows.h>
#include <objbase.h>
#include <stdlib.h>
#include <crtdbg.h>

#ifdef NDEBUG
#	define ATOM_DBG_MARK_BEGIN( _1, _Br )
#	define ATOM_DBG_MARK_END( _1, _2, _3 )
#else
#	define ATOM_DBG_MARK_BEGIN( _1, _Br )	_CrtMemState _1; _CrtMemCheckpoint( &_1 ); _crtBreakAlloc = _Br; {
#	define ATOM_DBG_MARK_END( _1, _2, _3, _4 )	} _CrtMemState _2, _3; _CrtMemCheckpoint( &_2 ); if ( _CrtMemDifference( &_3, &_1, &_2 ) ) { _CrtMemDumpStatistics( &_3 ); _CrtDumpMemoryLeaks(); assert( _4 ); }
#endif

namespace z3d { namespace dbg {
	//-------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------

} }

#else
#	define ATOM_DBG_MARK_BEGIN( _1, _Br )
#	define ATOM_DBG_MARK_END( _1, _2, _3 )
#endif

#endif //ATOM_UTIL_DBG_HPP
