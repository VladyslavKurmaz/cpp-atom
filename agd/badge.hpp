#pragma once
#include "./classes.hpp"

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > badge2logger;
typedef atom::nstorage< pref, boost::shared_ptr, atom::narray1 > badge2pref;
typedef atom::nstorage< window, boost::shared_ptr, atom::narray1 > badge2window;
//
class badge;

ATOM_DEF_ONPAINT(badge)
ATOM_DEF_ONLBUTTONDOWN(badge)
ATOM_DEF_ONMOUSEMOVE(badge)
ATOM_DEF_ONLBUTTONUP(badge)

class badge : public atom::wwindow< badge, LOKI_TYPELIST_4(badge_onpaint_pair_t, badge_onlbuttondown_pair_t, badge_onmousemove_pair_t, badge_onlbuttonup_pair_t) >,
	public atom::node< LOKI_TYPELIST_3(badge2logger, badge2pref, badge2window) >,
	public boost::enable_shared_from_this < badge > {
	typedef atom::wwindow < badge, LOKI_TYPELIST_4(badge_onpaint_pair_t, badge_onlbuttondown_pair_t, badge_onmousemove_pair_t, badge_onlbuttonup_pair_t) >
		base_badge_t;
	typedef atom::node < LOKI_TYPELIST_3(badge2logger, badge2pref, badge2window) >
		base_node_t;
public:
	///
	static badge_ptr create(logger_ptr l, pref_ptr p, window_ptr w) {
		return badge_ptr(new badge(l, p, w));
	}
	///
	~badge();
	///
	bool init(HWND hParent);
	///
	void clear();
	//
	void onPaint(HWND hWnd);
	//
	void onLButtonDown(HWND hWnd, BOOL dblClickint, int x, int y, UINT state);
	//
	void onMouseMove(HWND hWnd, int x, int y, UINT state);
	//
	void onLButtonUp(HWND hWnd, int x, int y, UINT state);

protected:
	LOGGER_ACCESSOR(badge2logger)
	PREF_ACCESSOR(badge2pref)
	WINDOW_ACCESSOR(badge2window)

private:
	///
	badge(logger_ptr l, pref_ptr p, window_ptr w);
	///
	dcb_t
		badgeImg;
};

