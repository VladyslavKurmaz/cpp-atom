/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
\-----------------------------------------------------------------------------/
*/
#ifndef ATOM_ZOOM_WINCONTEXT_HPP
#define ATOM_ZOOM_WINCONTEXT_HPP
#pragma once

namespace atom { namespace zoom {

	class wincontext : public context {
	public:
		///
		wincontext() : context(), wnd( NULL ) {
		}
		///
		~wincontext() {
		}
		///
		HWND
			wnd;
	};

} }

#endif //ATOM_ZOOM_WINCONTEXT_HPP
