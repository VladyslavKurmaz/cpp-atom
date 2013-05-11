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

#include <atom/node.hpp>

namespace atom { namespace zoom {

	typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > render2logger;
	typedef atom::nstorage< stream, boost::shared_ptr, atom::narray1 > render2stream;

	class render :
		public atom::node< LOKI_TYPELIST_2( render2logger, render2stream ) >,
		public boost::enable_shared_from_this< render > {
			typedef atom::node< LOKI_TYPELIST_2( render2logger, render2stream ) >
				base_node_t;

	public:
		///
		virtual bool
		init() = 0;
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
		logger&
		get_logger() { return ( *( get_slot<render2logger>().item() ) ); }
		///
		stream&
		get_stream() { return ( *( get_slot<render2stream>().item() ) ); }
		///
		render( logger_ptr l, stream_ptr s );
		///
		virtual ~render();

	private:
	};

} }

#endif//ATOM_ZOOM_RENDER_HPP
