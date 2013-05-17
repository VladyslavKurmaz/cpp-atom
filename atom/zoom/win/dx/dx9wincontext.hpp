/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_ZOOM_WIN_DX_DX9WINCONTEXT_HPP
#define ATOM_ZOOM_WIN_DX_DX9WINCONTEXT_HPP
#pragma once

#include "./../win.hpp"
#include <atom/util/ptr.hpp>

#include <d3dx9.h>

namespace atom { namespace zoom {

	class dx9wincontext : public wincontext {
	public:	
		///
		dx9wincontext() : wincontext(), d3dpp(), d3d( NULL ), d3ddevice( NULL ) {
			memset( &d3dpp, 0, sizeof( d3dpp ) );
		}
		///
		~dx9wincontext() {
			if ( d3ddevice != NULL ) {
				d3ddevice->Release();
			}
			if ( d3d != NULL ) {
				d3d->Release();
			}
		}
		///
		D3DPRESENT_PARAMETERS
			d3dpp;
		///
		IDirect3D9*
			d3d;
		///
		IDirect3DDevice9*
			d3ddevice;
	};

	ATOM_ZOOM_DEF_PTR( dx9wincontext )

	class dx9holder {
	public:
		dx9holder( dx9wincontext_ptr d ) : dx9data( d ) {
		}
	protected:
		dx9wincontext_ptr
			dx9data;
	};


} }


#endif // ATOM_ZOOM_WIN_DX_DX9WINCONTEXT_HPP
