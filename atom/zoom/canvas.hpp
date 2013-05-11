/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2013.05.11 - initial structure                                 |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_ZOOM_CANVAS_HPP
#define ATOM_ZOOM_CANVAS_HPP
#pragma once

namespace atom { namespace zoom {

	typedef atom::nstorage< stream, boost::shared_ptr, atom::narray1 > canvas2stream;

	class canvas :
		public atom::node< LOKI_TYPELIST_1( canvas2stream ) >,
		public boost::enable_shared_from_this< canvas > {

			typedef atom::node< LOKI_TYPELIST_1( canvas2stream ) >
				base_node_t;

	public:
		///
		virtual bool
		init() = 0;
		///
		virtual void*
		get_handle() = 0;
		///
		virtual void
		run( boost::function< bool() > ) = 0;
		///
		virtual void
		clear() = 0;

	protected:
		///
		canvas( stream_ptr s );
		///
		virtual ~canvas();

	private:
	};

} }

#endif //ATOM_ZOOM_CANVAS_HPP
