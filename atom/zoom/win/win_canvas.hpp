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

#include <atom/util/wwindow.hpp>

namespace atom { namespace zoom {

	class win_canvas;
	ATOM_DEF_ONLBUTTONDOWN( win_canvas )
	ATOM_DEF_ONLBUTTONUP( win_canvas )
	ATOM_DEF_ONMOUSEMOVE( win_canvas )
	ATOM_DEF_ONCLOSE( win_canvas )


	typedef atom::nstorage< stream, boost::shared_ptr, atom::narray1 > canvas2stream;

	class win_canvas :
		public canvas,
		protected atom::wwindow< win_canvas, LOKI_TYPELIST_4( onlbuttondown_pair_t, onlbuttonup_pair_t, onmousemove_pair_t, onclose_pair_t ) >
	{
		typedef atom::wwindow< win_canvas, LOKI_TYPELIST_4( onlbuttondown_pair_t, onlbuttonup_pair_t, onmousemove_pair_t, onclose_pair_t ) >
			wwindow_base_t;

	public:
		///
		static canvas_ptr create( logger_ptr l ) {
			return canvas_ptr( new win_canvas( l ) );
		}
		///
		virtual bool
		init();
		///
		virtual void*
		get_handle();
		///
		virtual void
		run( boost::function< bool() > tick, boost::function< void( input_ptr ) > handler );
		///
		virtual void
		clear();
		///
		virtual ~win_canvas();

	protected:
		///
		win_canvas( logger_ptr l );
		///
		void
		onlbuttondown( HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags );
		///
		void
		onlbuttonup( HWND hWnd, int x, int y, UINT keyFlags );
		///
		void
		onmousemove( HWND hWnd, int x, int y, UINT keyFlags );
		///
		void
		onclose( HWND hWnd );

	private:
		///
		boost::function< void( input_ptr ) >
			input_handler;
	};

} }

#endif //ATOM_ZOOM_WINCANVAS_HPP
