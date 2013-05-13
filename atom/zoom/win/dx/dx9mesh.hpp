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

#ifndef ATOM_ZOOM_DX9MESH_HPP
#define ATOM_ZOOM_DX9MESH_HPP
#pragma once

namespace atom { namespace zoom {

	class dx9mesh : public mesh {

	public:
		///
		static dx9mesh_ptr create( logger_ptr l, stream_ptr s ) {
			return dx9mesh_ptr( new dx9mesh( l, s ) );
		}
		///
		virtual ~dx9mesh();
		///
		virtual void
		clear();

	protected:
		///
		dx9mesh( logger_ptr l, stream_ptr s );

	private:
	};

} }

#endif //ATOM_ZOOM_DX9MESH_HPP
