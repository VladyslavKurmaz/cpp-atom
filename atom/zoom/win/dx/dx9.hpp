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

#ifndef ATOM_ZOOM_WIN_DX_DX9_HPP
#define ATOM_ZOOM_WIN_DX_DX9_HPP
#pragma once

#include "./../win.hpp"
#include <atom/util/ptr.hpp>

#include <d3dx9.h>

namespace atom { namespace zoom {

	ATOM_ZOOM_DEF_PTR( dx9render )
	ATOM_ZOOM_DEF_PTR( dx9mesh )
	ATOM_ZOOM_DEF_PTR( dx9texture )
	

} }

#include "./dx9wincontext.hpp"
#include "./dx9render.hpp"
#include "./dx9mesh.hpp"
#include "./dx9texture.hpp"

#endif // ATOM_ZOOM_WIN_DX_DX9_HPP
