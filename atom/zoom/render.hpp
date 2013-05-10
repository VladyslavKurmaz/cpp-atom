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

	class render :
		public atom::node< LOKI_TYPELIST_1( render2logger ) >,
		public boost::enable_shared_from_this< render > {


	public:

	protected:

		typedef boost::lock_guard< atom::lock_t >
			guard_t;
		atom::lock_t
			lock;
		///
		logger&
			get_logger() { return ( *( get_slot<render2logger>().item() ) ); }

		///
		render( logger_ptr l ) :
				lock() {
			atom::mount<render2logger>( this, l );
		}
		///
		virtual ~render() {
		}

	private:
	};

} }

#endif//ATOM_ZOOM_RENDER_HPP
