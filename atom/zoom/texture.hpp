/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| AUTHOR:      Vladislav Kurmaz                                               |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_ZOOM_TEXTURE_HPP
#define ATOM_ZOOM_TEXTURE_HPP
#pragma once

namespace atom { namespace zoom {

	class texture : public entity< texture > {

	public:
		virtual bool
		build( string_t const& s ) = 0;
		///
		virtual void
		clear() = 0 {
			entity< texture >::clear();
		}

	protected:
		///
		texture( logger_ptr l ) : entity< texture >( l ) {
		}
		///
		virtual ~texture() {
		}
	private:
	};

} }

#endif //ATOM_ZOOM_TEXTURE_HPP
