/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2013.05.09 - initial structure                                 |
|              2013.05.12 - move logger ans stream to the enity               |
|-----------------------------------------------------------------------------|
| TODO:        move lock here?                                                |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_ZOOM_ENTITY_HPP
#define ATOM_ZOOM_ENTITY_HPP
#pragma once

namespace atom { namespace zoom {

	typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > entity2logger;
	typedef atom::nstorage< stream, boost::shared_ptr, atom::narray1 > entity2stream;

	template< class T >
	class entity :
		public atom::node< LOKI_TYPELIST_2( entity2logger, entity2stream ) >,
		public boost::enable_shared_from_this< T > {
			typedef atom::node< LOKI_TYPELIST_2( entity2logger, entity2stream ) >
				base_node_t;
	public:
		logger_ptr&
		get_logger() { return ( get_slot<render2logger>().item() ); }
		///
		stream_ptr&
		get_stream() { return ( get_slot<render2stream>().item() ); }

		///
		virtual void
		clear() = 0 {
			base_node_t::clear();
		}

	protected:
		///
		entity( logger_ptr l, stream_ptr s ) {
			atom::mount<entity2logger>( this, l );
			atom::mount<entity2stream>( this, s );
		}
		///
		virtual ~entity() = 0 {
		}
	private:
	};

} }

#endif//ATOM_ZOOM_ENTITY_HPP
