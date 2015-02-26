#pragma once
#include "./classes.hpp"

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > panel2logger;
typedef atom::nstorage< pref, boost::shared_ptr, atom::narray1 > panel2pref;
//
class panel;

ATOM_DEF_ONPAINT( panel )
ATOM_DEF_ONSIZE( panel )

class panel :	public atom::wwindow< panel, LOKI_TYPELIST_2( panel_onpaint_pair_t, panel_onsize_pair_t ) >,
				public atom::node< LOKI_TYPELIST_2( panel2logger, panel2pref ) >,
				public boost::enable_shared_from_this< panel > {
	typedef atom::wwindow< panel, LOKI_TYPELIST_2( panel_onpaint_pair_t, panel_onsize_pair_t ) >
		base_panel_t;
	typedef atom::node< LOKI_TYPELIST_2( panel2logger, panel2pref ) >
		base_node_t;
public:
	///
	static panel_ptr create( logger_ptr l, pref_ptr p ) {
		return panel_ptr( new panel( l, p ) );
	}
	///
	~panel();
	///
	bool init( HWND hParent );
	///
	void clear();
	//
	void addRecord( atom::string_t const& s, atom::string_t const& d );
	///
	///
	void onPaint( HWND hWnd );
	///
	void onSize( HWND hWnd, UINT state, int cx, int cy );

protected:
	LOGGER_ACCESSOR( panel2logger )
	PREF_ACCESSOR( panel2pref )

private:
	HWND
		hListView;
	///
	panel( logger_ptr l, pref_ptr p );
};

