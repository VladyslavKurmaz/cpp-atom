/*
/-----------------------------------------------------------------------------\
| Copyright � 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2013.05.09 - initial structure                                 |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
\-----------------------------------------------------------------------------/
remove stream as required constructor parameter
*/

#ifndef ATOM_ZOOM_ZOOM_HPP
#define ATOM_ZOOM_ZOOM_HPP
#pragma once

#include <boost/function.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread/locks.hpp>


#include <atom/node.hpp>
#include <atom/util/lock.hpp>


#define ATOM_ZOOM_DEF_PTR( x ) class x; typedef boost::shared_ptr< x > x##_ptr;

namespace atom { namespace zoom {

	typedef int
		int_t;

	typedef unsigned int
		uint_t;

	typedef float
		float_t;

	typedef std::string
		string_t;

	ATOM_ZOOM_DEF_PTR( stream )

	class stream {
	public:
		static stream_ptr create() {
			return stream_ptr( new stream() );
		}
	};

	ATOM_ZOOM_DEF_PTR( input )

	class input {
	};

	struct matrix {
	};

	ATOM_ZOOM_DEF_PTR( logger )
	ATOM_ZOOM_DEF_PTR( context )
	ATOM_ZOOM_DEF_PTR( canvas )
	ATOM_ZOOM_DEF_PTR( render )
	ATOM_ZOOM_DEF_PTR( mesh )
	ATOM_ZOOM_DEF_PTR( texture )
	ATOM_ZOOM_DEF_PTR( camera )


} }

#include "./context.hpp"
#include "./logger.hpp"
#include "./entity.hpp"
#include "./wo.hpp"
#include "./canvas.hpp"
#include "./render.hpp"
#include "./mesh.hpp"
#include "./texture.hpp"
#include "./camera.hpp"

#endif//ATOM_ZOOM_ZOOM_HPP
