/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| HISTORY:     2009.09.30 - Created                                           |
|              2010.03.23 - Move to new namepace, devide for diff files       |
|              2012.03.26 gcc(cygwin) build                                   |
|-----------------------------------------------------------------------------|
| TODO:		                                                                    |
\-----------------------------------------------------------------------------/
*/
#pragma once

#ifndef ATOM_UTIL_PTR_HPP
#define ATOM_UTIL_PTR_HPP
#ifdef _WINDOWS
//
#include <windows.h>

#include <boost/smart_ptr.hpp>

namespace atom {
	
	class shared_dc : public boost::shared_ptr< void >
	{
	public:
		///
		shared_dc() : shared_ptr() {}
		///
		shared_dc( HDC dc ) : shared_ptr( dc, release_dc ) {}
		///
		operator HDC() {
			return reinterpret_cast< HDC >( this->get() );
		}
		///
		static void release_dc( HDC dc ) {
			if ( dc != 0 ) DeleteDC( dc );
		}
	};

	template < typename T >
	class shared_gdiobj : public boost::shared_ptr< void >
	{
	public:
		///
		shared_gdiobj() : shared_ptr() {}
		///
		shared_gdiobj( T h ) : shared_ptr( h, release_gdiobj ) {}
		///
		operator T() {
			return reinterpret_cast< T >( this->get() );
		}
		///
		static void release_gdiobj( HGDIOBJ h ) {
			if ( h != 0 ) DeleteObject( h );
		}
	};
}

#endif

#endif //ATOM_UTIL_PTR_HPP
