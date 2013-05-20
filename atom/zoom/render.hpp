/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_ZOOM_RENDER_HPP
#define ATOM_ZOOM_RENDER_HPP
#pragma once

namespace atom { namespace zoom {

	class render : public entity< render > {
		typedef entity< render >
			base_entity_t;
	public:
		///
		virtual bool
		init( camera_ptr c ) = 0;
		///
		virtual bool
		frame() = 0;
		///
		virtual void
		clear() = 0;
		///
		virtual mesh_ptr
		create_mesh() = 0;
		///
		virtual texture_ptr
		create_texture() = 0;

	protected:
		///
		typedef boost::lock_guard< atom::lock_t >
			guard_t;
		atom::lock_t
			lock;
		///
		///
		canvas_ptr&
		get_canvas() { return ( this->canvas ); }
		///
		render( logger_ptr l, canvas_ptr c );
		///
		virtual ~render();

	private:
		///
		canvas_ptr
			canvas;
		///
		camera_ptr
			camera;
	};

} }

#endif//ATOM_ZOOM_RENDER_HPP
