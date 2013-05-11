/*
/-----------------------------------------------------------------------------\
| Copyright � 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2013.05.10 - initial structure                                 |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_ZOOM_DX9RENDER_HPP
#define ATOM_ZOOM_DX9RENDER_HPP
#pragma once

#include <atom/zoom/render.hpp>
#include <d3dx9.h>

namespace atom { namespace zoom {

	class dx9render :
		public render {
	public:
		///
		static render_ptr create( logger_ptr l, stream_ptr s, canvas_ptr c ) {
			return render_ptr( new dx9render( l, s, c ) );
		}
		///
		virtual bool
		init();
		///
		virtual bool
		frame();
		///
		virtual void
		clear();
		///
		virtual ~dx9render();

	protected:
		///
		dx9render( logger_ptr l, stream_ptr s, canvas_ptr c );

	private:
		///
		D3DPRESENT_PARAMETERS
			d3dpp;
		///
		typedef boost::interprocess::unique_ptr< IDirect3D9, atom::unique_com_ptr_deleter< IDirect3D9 > >
			d3d_t;
		d3d_t
			d3d;
		///
		typedef boost::interprocess::unique_ptr< IDirect3DDevice9, atom::unique_com_ptr_deleter< IDirect3DDevice9 > >
			device_t;
		device_t
			device;
	};

} }

#endif//ATOM_ZOOM_DX9RENDER_HPP
