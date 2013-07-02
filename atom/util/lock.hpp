/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2013.05.10 - Created                                           |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
|                                                                             |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_UTIL_LOCK_HPP
#define ATOM_UTIL_LOCK_HPP

namespace atom {

#if defined(_WIN32)
	class wcs {
		CRITICAL_SECTION
		cs;
	public:

		wcs()
		{ InitializeCriticalSection( &cs ); }

		~wcs()
		{ DeleteCriticalSection( &cs ); }
	 
		void lock()
		{ EnterCriticalSection( &cs ); }

		void unlock()
		{ LeaveCriticalSection( &cs ); }
	};
#endif

	class empty_lock {
	public:
		void lock()
		{ }

		void unlock()
		{ }
	};

#ifdef ATOM_MT
	#if defined(_WIN32)
	typedef	wcs
		lock_t; 
	#endif
#else
	typedef	empty_lock
		lock_t; 
#endif

}

#endif //ATOM_UTIL_LOCK_HPP
