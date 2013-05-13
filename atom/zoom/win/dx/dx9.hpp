/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2013.05.09 - initial structure                                 |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_ZOOM_WIN_DX_DX9_HPP
#define ATOM_ZOOM_WIN_DX_DX9_HPP
#pragma once

#include "./../win.hpp"
#include <atom/util/ptr.hpp>

#include <d3dx9.h>

namespace atom { namespace zoom {

	ATOM_ZOOM_DEF_PTR( dx9render )
	ATOM_ZOOM_DEF_PTR( dx9mesh )
	
	class dx9 : public win {
	public:	
		///
		dx9() : win(), d3dpp(), d3d( NULL ), d3ddevice( NULL ) {
			memset( &d3dpp, 0, sizeof( d3dpp ) );
		}
		///
		~dx9() {
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

	ATOM_ZOOM_DEF_PTR( dx9 )

	class dx9holder {
	public:
		dx9holder( dx9_ptr d ) : dx9data( d ) {
		}
	protected:
		dx9_ptr
			dx9data;
	};


} }

#include "./dx9render.hpp"
#include "./dx9mesh.hpp"

#endif //ATOM_ZOOM_WIN_DX_DX9_HPP
