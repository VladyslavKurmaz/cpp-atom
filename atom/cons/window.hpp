#pragma once
#include "./classes.hpp"

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > window2logger;
typedef atom::nstorage< pref, boost::shared_ptr, atom::narray1 > window2pref;
//
class window;

ATOM_DEF_ONKEYDOWN( window )
ATOM_DEF_ONKEYUP( window )
ATOM_DEF_ONCHAR( window )
ATOM_DEF_ONHOTKEY( window )
ATOM_DEF_ONPAINT( window )
ATOM_DEF_ONCLOSE( window )
ATOM_DEF_ONSETTINGCHANGE( window )
ATOM_DEF_ONTIMER( window )
ATOM_DEF_ONCOMMAND( window )

class window :	public atom::wwindow< window, LOKI_TYPELIST_9( onkeydown_pair_t, onkeyup_pair_t, onchar_pair_t, onhotkey_pair_t, onpaint_pair_t, onclose_pair_t, onsettingchange_pair_t, ontimer_pair_t, oncommand_pair_t ) >,
				public atom::node< LOKI_TYPELIST_2( window2logger, window2pref ) >,
				public boost::enable_shared_from_this< window > {
	typedef atom::wwindow< window, LOKI_TYPELIST_9( onkeydown_pair_t, onkeyup_pair_t, onchar_pair_t, onhotkey_pair_t, onpaint_pair_t, onclose_pair_t, onsettingchange_pair_t, ontimer_pair_t, oncommand_pair_t ) >
		base_window_t;
	typedef atom::node< LOKI_TYPELIST_2( window2logger, window2pref ) >
		base_node_t;
public:
	///
	static window_ptr create( logger_ptr l, pref_ptr p ) {
		return window_ptr( new window( l, p ) );
	}
	///
	~window();
	///
	bool
	init();
	///
	void
	run();
	//
	void
	clear();
	///
	void onkey( HWND hWnd, UINT vk, BOOL down, int repeat, UINT flags );
	//
	void onchar( HWND hWnd, TCHAR ch, int cRepeat );
	///
	void onhotkey(HWND hWnd, int idHotKey, UINT fuModifiers, UINT vk);
	///
	void onpaint( HWND hWnd );
	///
	void onclose( HWND );
	///
	void onsettingchange( HWND hWnd, UINT uiAction, LPCTSTR lpName );
	///
	void ontimer( HWND hWnd, UINT id );
	///
	void oncommand( HWND hWnd, int id, HWND hwndCtl, UINT codeNotify );
	//
	void parse_conf( TCHAR const* conf );
	///
	void process_empty() {}
	///
	void process_autostart();
	///
	void process_hotkeys();
	///
	void process_fonts();
	//
	void process_window();
	//
	void process_ui();

protected:
	//
	logger_ptr
	get_logger() { return ( get_slot< window2logger >().item() ); }
	//
	pref_ptr
	get_pref() { return ( get_slot< window2pref >().item() ); }
	//
	void
	close_frame( frame_id_t const id );
	//
	void
	update_hotkeys();
	//
	void
	calculate_docks();
	//
	void
	slide_begin();
	//
	void
	slide_update( float mult );
	//
	void
	slide_end();

private:
	class area;
	typedef boost::shared_ptr< area >
		area_ptr;
	///
	typedef std::vector< frame_ptr >
		frames_t;
	///
	window( logger_ptr l, pref_ptr p );
	///
	class area : public boost::enable_shared_from_this< area > {
	public:
		static area_ptr create( area_ptr p, frame_ptr f, bool const r ) {
			area_ptr a = area_ptr( new area( p, f ) );
			if ( a ) {
				a->reorder();
			} 
			return ( a );
		};
		///
		~area() {
		}
		//
		void close() {
			if ( this->parent ){
				area_ptr a = area_ptr(); 
				if ( this->parent->first != this->shared_from_this() ) { a = this->parent->first; }
				if ( this->parent->second != this->shared_from_this() ) { a = this->parent->second; }
				//
				this->parent->vert = a->vert;
				this->parent->first = a->first; if ( this->parent->first ) { this->parent->first->parent = this->parent; }
				this->parent->second = a->second; if ( this->parent->second ) { this->parent->second->parent = this->parent; }
				this->parent->frame = a->frame;
				//
				a->clear();
				this->parent->reorder();
			}
			this->clear();
		}
		//
		area_ptr find( frame_ptr f ) {
			area_ptr result = area_ptr();
			if ( this->frame == f ) { result = this->shared_from_this(); }
			if ( !result && this->first ) { result = this->first->find( f ); }
			if ( !result && this->second ) { result = this->second->find( f ); }
			return ( result );
		}
		//
		frame_ptr get_by_index( unsigned int const i ) {
			frame_ptr result = frame_ptr();
			if ( this->frame && ( this->frame->get_index() == i ) ) { result = this->frame; }
			if ( !result && this->first ) { result = this->first->get_by_index( i ); }
			if ( !result && this->second ) { result = this->second->get_by_index( i ); }
			return ( result );
		}
		//
		frame_ptr get_by_id( frame_id_t const id ) {
			frame_ptr result = frame_ptr();
			if ( this->frame && ( this->frame->get_id() == id ) ) { result = this->frame; }
			if ( !result && this->first ) { result = this->first->get_by_id( id ); }
			if ( !result && this->second ) { result = this->second->get_by_id( id ); }
			return ( result );
		}
		//
		void collect( frames_t& fs ) const {
			if ( this->frame ) {
				fs.push_back( this->frame );
			} else {
				this->first->collect( fs );
				this->second->collect( fs );
			}
		}
		void split( frame_ptr f ) {
			this->vert = true;
			this->first = create( this->shared_from_this(), this->frame, false );
			this->second = create( this->shared_from_this(), f, false );
			this->frame = frame_ptr();
			this->reorder();
		}
		void rotate() {
			if ( this->parent ) {
				this->parent->vert = !this->parent->vert;  
				if ( this->parent->vert ) {
					std::swap( this->parent->first, this->parent->second );
				}
			}
			this->reorder();
		}
		frame_ptr next() {
			if ( this->parent ) {
				return ( this->parent->search( this->shared_from_this(), true, true ) );
			}
			return ( this->frame );
		}
		frame_ptr prev() {
			if ( this->parent ) {
				return ( this->parent->search( this->shared_from_this(), true, false) );
			}
			return ( this->frame );
		}
		void draw( boost::function< void( frame_ptr const&, frame_coord const& ) > func, frame_coord const& c ) const {
			if ( this->frame ) {
				func( this->frame, c );
			} else {
				fraction const l = ((this->vert)?( c.left + fraction( 1, c.width * 2 ) ):( c.left ) );
				fraction const t = ((this->vert)?( c.top ):( c.top + fraction( 1, c.height * 2 ) ) );
				unsigned int const w = ((this->vert)?(c.width * 2):(c.width));
				unsigned int const h = ((this->vert)?(c.height):(c.height * 2));
				//
				if ( this->first ) {
					this->first->draw( func, frame_coord( c.left.get_n(), c.left.get_d(), c.top.get_n(), c.top.get_d(), w, h ) );
				}
				if ( this->second ) {
					this->second->draw( func, frame_coord( l.get_n(), l.get_d(), t.get_n(), t.get_d(), w, h ) );
				}
			}
		}

	protected:
		///
		area_ptr get( bool const f ) const {
			if ( f ) {
				return ( this->first );
			}
			return ( this->second );
		}
		///
		frame_ptr search( area_ptr a, bool const up, bool const n ) {
			if ( this->frame ) {
				return ( this->frame );
			} else {
				if ( up ) {
					// moving up
					if ( ( this->parent ) && ( this->get( !n ) == a ) ) {
						return ( this->parent->search( this->shared_from_this(), up, n ) );
					}
					return ( this->get( !( this->get( true ) == a ) )->search( area_ptr(), false, n ) );
				} else {
					// moving down
					return ( this->get( n )->search( area_ptr(), up, n ) );
				}
			}
		}
		///
		void reorder() {
			unsigned int i = 0;
			area_ptr a = this->shared_from_this();
			while( a->parent ) {
				a = a->parent;
			}
			a->reorder( i );
		}
		///
		void reorder( unsigned int& i ) const {
			if ( this->frame ) {
				this->frame->set_index( ++i );
			} else {
				this->first->reorder( i );
				this->second->reorder( i );
			}
		}
	private:
		area_ptr
			parent;
		bool
			vert;
		area_ptr
			first;
		area_ptr
			second;
		frame_ptr
			frame;
		///
		area( area_ptr p, frame_ptr f ) :
				parent( p )
			,	vert( false )
			,	first()
			,	second()
			,	frame( f ) {
		}
		///
		void clear() {
			this->parent = this->first = this->second = area_ptr();
			this->frame = frame_ptr();
		}
	};
	//
	area_ptr
		head_area;
	//
	frame_ptr
		current_frame;
	//
	bool
		expand_mode;
	//
	struct hotkey_t {
		int id;
		UINT mods;
		UINT vk;
		hotkey_t() : id(0), mods(), vk() {}   
		int operator==( hotkey_t const& r ) const { return ( ( this->mods == r.mods ) && ( this->vk == r.vk ) ); }
	} appear_hk;
	//
	atom::accel
		accel;
	//
	RECT
		rect_active;
	RECT
		rect_inactive;
	bool
		sliding;
	DWORD
		slide_start_time;
	UINT_PTR
		slide_timer_id;
	RECT
		slide_rect_src;
	RECT
		slide_rect_dest;
	//
	struct paint_param_t {
		atom::shared_dc
			mem_dc;
		atom::shared_gdiobj< HBITMAP >
			mem_bitmap;
		atom::shared_gdiobj< HBRUSH >
			bk_brush;
		unsigned int
			margin_size;
		atom::shared_gdiobj< HBRUSH >
			border_brush;
		atom::shared_gdiobj< HBRUSH >
			border_brush_inactive;
		unsigned int
			border_size;
		unsigned int
			padding_size;
		atom::shared_gdiobj< HBRUSH >
			scroll_brush;
		unsigned int
			scroll_size;
		atom::shared_gdiobj< HFONT >
			font_text;
		COLORREF
			font_text_color;
		atom::shared_gdiobj< HFONT >
			font_sys;
		COLORREF
			font_sys_color;
	} paint_param;

};

