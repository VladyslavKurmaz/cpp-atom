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

#ifndef ATOM_ZOOM_WO_HPP
#define ATOM_ZOOM_WO_HPP
#pragma once

namespace atom { namespace zoom {

	class wo : public entity< wo > {

	public:
		///
		matrix const&
		get_mtx() const {
			return ( this->mtx ); }
		///
		void
		set_mtx( matrix const& m ) {
			this->mtx = m;  }
		///
		virtual void
		clear() = 0 {
			entity< wo >::clear();
		}

	protected:
		///
		wo( logger_ptr l, stream_ptr s ) : entity< wo >( l, s ) {
		}
		///
		virtual ~wo() {
		}
	private:
		///
		matrix
			mtx;
	};

} }

#endif //ATOM_ZOOM_WO_HPP
