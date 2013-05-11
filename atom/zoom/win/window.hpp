#if 0
#pragma once
#include <atom/util/wwindow.hpp>

namespace atom { namespace zoom {

class window3d;
ATOM_DEF_ONLBUTTONDOWN( window3d )
ATOM_DEF_ONLBUTTONUP( window3d )
ATOM_DEF_ONMOUSEMOVE( window3d )
ATOM_DEF_ONCLOSE( window3d )

class window3d : public atom::wwindow< window3d, LOKI_TYPELIST_4( onlbuttondown_pair_t, onlbuttonup_pair_t, onmousemove_pair_t, onclose_pair_t ) >
{
public:
	///
	window3d() : wwindow( *this, INITLIST_4( &window3d::onlbuttondown, &window3d::onlbuttonup, &window3d::onmousemove, &window3d::onclose ) ), mouse_dx( 0 ), mouse_dy( 0 ){
	}
	///
	void
		render();
	///
	void
		onlbuttondown( HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags );
	///
	void
		onlbuttonup( HWND hWnd, int x, int y, UINT keyFlags );
	///
	void
		onmousemove( HWND hWnd, int x, int y, UINT keyFlags );
	///
	void
		onclose( HWND hWnd );

protected:
private:
	///
	int
		mouse_dx;
	///
	int
		mouse_dy;

};
#endif