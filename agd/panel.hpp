#pragma once
#include "./classes.hpp"

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > panel2logger;
typedef atom::nstorage< pref, boost::shared_ptr, atom::narray1 > panel2pref;
//
class panel;

ATOM_DEF_ONPAINT( panel )
ATOM_DEF_ONSIZE( panel )
ATOM_DEF_ONNOTIFY( panel )
ATOM_DEF_ONCOMMAND( panel )



class panel :	public atom::wwindow< panel, LOKI_TYPELIST_4( panel_onpaint_pair_t, panel_onsize_pair_t, panel_onnotify_pair_t,  panel_oncommand_pair_t ) >,
				public atom::node< LOKI_TYPELIST_2( panel2logger, panel2pref ) >,
				public boost::enable_shared_from_this< panel > {
	typedef atom::wwindow< panel, LOKI_TYPELIST_4( panel_onpaint_pair_t, panel_onsize_pair_t, panel_onnotify_pair_t, panel_oncommand_pair_t ) >
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
	///
	void onNotify( int id, LPNMHDR lpnm );
	///
	void onCommand( int id, HWND hwndCtl, UINT codeNotify );
	///
	bool isLocked() const;
	///
	void setADState(bool const a) {
		this->adActive = a;
	};
	///
	void setADVisible(bool const v) {
		this->adVisible = v;
	};

protected:
	LOGGER_ACCESSOR( panel2logger )
	PREF_ACCESSOR( panel2pref )
	///
	void popupLangMenu( int const id );
	///
	void updateLangsImages();


private:
	bool
		adActive;
	bool
		adVisible;
	///
	atom::wImageList
		imageList;
	///
	atom::wctrlListView
		listView;
	///
	atom::wctrlToolbar
		toolbar;
	///
	panel( logger_ptr l, pref_ptr p );
};

