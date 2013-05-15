/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2013.05.15 - initial structure                                 |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_ZOOM_CAMERA_HPP
#define ATOM_ZOOM_CAMERA_HPP
#pragma once

namespace atom { namespace zoom {

	class camera : public entity< camera > {

	public:
		///
		static camera_ptr create( logger_ptr l, stream_ptr s ) {
			return camera_ptr( new camera( l, s ) );
		}
		///
		virtual ~camera();
		///
		void input( input_ptr i );
		///
		virtual void
		clear();

	protected:
		///
		camera( logger_ptr l, stream_ptr s );

	private:
	};

} }

#endif //ATOM_ZOOM_CAMERA_HPP
