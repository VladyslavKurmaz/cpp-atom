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
		public render< dx9render, atom::lock_t > {
	public:
		///
		static dx9render_ptr create( logger_ptr l ) {
			return dx9render_ptr( new dx9render( l ) );
		}
		///
		~dx9render();

	protected:

	private:
		///
		dx9render( logger_ptr l );
	};

} }

#endif//ATOM_ZOOM_DX9RENDER_HPP
