/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2013.05.09 - initial structure                                 |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_ZOOM_ZOOM_HPP
#define ATOM_ZOOM_ZOOM_HPP
#pragma once

#include <boost/function.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread/locks.hpp>

#include <atom/util/lock.hpp>


#define ATOM_ZOOM_DEF_PTR( x ) class x; typedef boost::shared_ptr< x > x##_ptr;

namespace atom { namespace zoom {

	ATOM_ZOOM_DEF_PTR( logger )
	ATOM_ZOOM_DEF_PTR( render )
	ATOM_ZOOM_DEF_PTR( entity )


	ATOM_ZOOM_DEF_PTR( stream )

	class stream {
	public:
		static stream_ptr create() {
			return stream_ptr( new stream() );
		}
	};
} }

#include "./logger.hpp"
#include "./entity.hpp"
#include "./render.hpp"

#endif//ATOM_ZOOM_ZOOM_HPP
