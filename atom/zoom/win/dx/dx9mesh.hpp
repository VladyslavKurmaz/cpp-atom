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

	class dx9mesh : public mesh, protected dx9holder {

	public:
		///
		static dx9mesh_ptr create( logger_ptr l, stream_ptr s, dx9_ptr d ) {
			return dx9mesh_ptr( new dx9mesh( l, s, d ) );
		}
		///
		virtual ~dx9mesh();
		///
		virtual bool
		build_sphere( float_t radius, uint_t slices, uint_t stacks );
		///
		virtual void
		render();
		///
		virtual void
		clear();

	protected:
		///
		dx9mesh( logger_ptr l, stream_ptr s, dx9_ptr d );

	private:
		///
		ID3DXMesh*
			dxmesh;	
	};

} }

#endif //ATOM_ZOOM_DX9MESH_HPP
