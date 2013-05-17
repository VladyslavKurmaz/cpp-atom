/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
\-----------------------------------------------------------------------------/
remove stream as required constructor parameter
*/

#ifndef ATOM_ZOOM_CONTEXT_HPP
#define ATOM_ZOOM_CONTEXT_HPP
#pragma once

namespace atom { namespace zoom {

	class context {
		public:
			uint_t
				width;
			uint_t
				height;
			float_t
				fov;
	};

} }

#endif //ATOM_ZOOM_CONTEXT_HPP
