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

#ifndef ATOM_ZOOM_RENDER_HPP
#define ATOM_ZOOM_RENDER_HPP
#pragma once


namespace atom { namespace zoom {

	typedef atom::nstorage< canvas, boost::shared_ptr, atom::narray1 > render2canvas;

	class render : public entity< render > {
		typedef entity< render >
			base_entity_t;
	public:
		///
		virtual bool
		init() = 0;
		///
		virtual bool
		frame() = 0;
		///
		virtual void
		clear() = 0;

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
		render( logger_ptr l, stream_ptr s, canvas_ptr c );
		///
		virtual ~render();

	private:
		///
		canvas_ptr
			canvas;
	};

} }

#endif//ATOM_ZOOM_RENDER_HPP
