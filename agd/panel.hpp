#pragma once
#include "./classes.hpp"

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > panel2logger;
typedef atom::nstorage< pref, boost::shared_ptr, atom::narray1 > panel2pref;
typedef atom::nstorage< ad, boost::shared_ptr, atom::narray1 > panel2ad;
typedef atom::nstorage< langs, boost::shared_ptr, atom::narray1 > panel2langs;
//
class panel;

ATOM_DEF_ONPAINT( panel )
ATOM_DEF_ONSIZE( panel )
ATOM_DEF_ONNOTIFY( panel )
ATOM_DEF_ONCOMMAND( panel )



class panel :	public atom::wwindow< panel, LOKI_TYPELIST_4( panel_onpaint_pair_t, panel_onsize_pair_t, panel_onnotify_pair_t,  panel_oncommand_pair_t ) >,
	public atom::node< LOKI_TYPELIST_4(panel2logger, panel2pref, panel2ad, panel2langs) >,
				public boost::enable_shared_from_this< panel > {
	typedef atom::wwindow< panel, LOKI_TYPELIST_4( panel_onpaint_pair_t, panel_onsize_pair_t, panel_onnotify_pair_t, panel_oncommand_pair_t ) >
		base_panel_t;
	typedef atom::node< LOKI_TYPELIST_4(panel2logger, panel2pref, panel2ad, panel2langs) >
		base_node_t;
public:
	///
	static panel_ptr create(logger_ptr l, pref_ptr p, ad_ptr a, langs_ptr lg) {
		return panel_ptr( new panel( l, p, a, lg ) );
	}
	///
	~panel();
	///
	bool init( HWND hParent );
	///
	void clear();
	//
	void beginCapture();
	//
	void endCapture(atom::string_t const& s, atom::string_t const& d, RECT const& rect);
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
	void setADState(bool const a);
	///
	void setADVisible(bool const v);

protected:
	LOGGER_ACCESSOR( panel2logger )
	PREF_ACCESSOR( panel2pref )
	ad_ptr getAD() { return ((base_node_t::get_value(boost::mpl::identity< panel2ad >()).item())); }
	langs_ptr getLangs() { return ((base_node_t::get_value(boost::mpl::identity< panel2langs >()).item())); }
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
	HWND
		gloss;
	HGDIOBJ
		glossFont;
	///
	panel(logger_ptr l, pref_ptr p, ad_ptr a, langs_ptr lg);
};

