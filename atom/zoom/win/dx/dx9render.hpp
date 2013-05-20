/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
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

namespace atom { namespace zoom {

	class dx9render : public render, protected dx9holder {
	public:
		///
		static dx9render_ptr create( logger_ptr l, canvas_ptr c ) {
			return dx9render_ptr( new dx9render( l, c ) );
		}
		///
		virtual ~dx9render();
		///
		virtual bool
		init( camera_ptr c );
		///
		virtual bool
		frame();
		///
		virtual void
		clear();
		///
		virtual mesh_ptr
		create_mesh();
		///
		virtual texture_ptr
		create_texture();

	protected:
		///
		dx9render( logger_ptr l, canvas_ptr c );

	private:
		///
		dx9wincontext_ptr
			context;
		///
		mesh_ptr
			m;
		///
		texture_ptr
			t;
	};

} }

#endif//ATOM_ZOOM_DX9RENDER_HPP
