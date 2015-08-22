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
#if defined(_WIN32)
//
#include <windows.h>

#include <boost/smart_ptr.hpp>
#include <boost/interprocess/smart_ptr/unique_ptr.hpp>

namespace atom {
	
	class shared_dc : public boost::shared_ptr < void >
	{
	public:
		///
		shared_dc() : shared_ptr() {}
		///
		shared_dc(HDC dc) : shared_ptr(dc, delete_dc) {}
		///
		shared_dc(HWND wnd) : shared_ptr(GetDC(wnd), boost::bind(&shared_dc::release_dc, _1, wnd)) {}
		///
		shared_dc(HWND wnd, int) : shared_ptr(GetWindowDC(wnd), boost::bind(&shared_dc::release_dc, _1, wnd)) {}
		///
		operator HDC() {
			return reinterpret_cast<HDC>(this->get());
		}
		///
		static void delete_dc(HDC dc) {
			if (dc != 0) DeleteDC(dc);
		}
		///
		static void release_dc(HDC dc, HWND wnd) {
			if (dc != 0) ReleaseDC(wnd, dc);
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

	template < typename T >
	struct unique_com_ptr_deleter {
		void operator()( T *p )
		{
			if ( p != 0 ) {
				p->Release();
			}
		}
	};

}

#endif

#endif //ATOM_UTIL_PTR_HPP
