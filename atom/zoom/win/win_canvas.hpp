/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2013.05.11 - initial structure                                 |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_ZOOM_WINCANVAS_HPP
#define ATOM_ZOOM_WINCANVAS_HPP
#pragma once

namespace atom { namespace zoom {

	typedef atom::nstorage< stream, boost::shared_ptr, atom::narray1 > canvas2stream;

	class win_canvas :
		public canvas {

	public:
		///
		static canvas_ptr create( stream_ptr s ) {
			return canvas_ptr( new win_canvas( s ) );
		}

		///
		virtual void
		clear();
		///
		virtual ~win_canvas();

	protected:
		///
		win_canvas( stream_ptr s );

	private:
	};

} }

#endif //ATOM_ZOOM_WINCANVAS_HPP
