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

#include <atom/zoom/render.hpp>

namespace atom { namespace zoom {

	class dx9render :
		public render {
	public:
		///
		static render_ptr create( logger_ptr l, stream_ptr s ) {
			return render_ptr( new dx9render( l, s ) );
		}
		///
		virtual bool
		init();
		///
		virtual void
		clear();
		///
		virtual ~dx9render();

	protected:
		///
		dx9render( logger_ptr l, stream_ptr s );

	private:
	};

} }

#endif//ATOM_ZOOM_DX9RENDER_HPP
