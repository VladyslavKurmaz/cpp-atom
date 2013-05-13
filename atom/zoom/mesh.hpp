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

#ifndef ATOM_ZOOM_MESH_HPP
#define ATOM_ZOOM_MESH_HPP
#pragma once

namespace atom { namespace zoom {

	class mesh : public wo {

	public:
		///
		virtual bool
		build_sphere( float_t radius, uint_t slices, uint_t stacks ) = 0;
		///
		virtual void
		render() = 0;
		///
		virtual void
		clear();

	protected:
		///
		mesh( logger_ptr l, stream_ptr s );
		///
		virtual ~mesh();

	private:
	};

} }

#endif//ATOM_ZOOM_MESH_HPP
