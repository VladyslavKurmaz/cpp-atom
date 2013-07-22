#pragma once
#include "./classes.hpp"

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > window2logger;
typedef atom::nstorage< pref, boost::shared_ptr, atom::narray1 > window2pref;
//
class window;

typedef void(window::* onkeydown_t)( HWND, UINT, BOOL, int, UINT );
typedef boost::mpl::pair< boost::mpl::int_< WM_KEYDOWN >::type, onkeydown_t >::type
	onkeydown_pair_type_t;

typedef void(window::* onkeyup_t)( HWND, UINT, BOOL, int, UINT );
typedef boost::mpl::pair< boost::mpl::int_< WM_KEYUP >::type, onkeyup_t >::type
	onkeyup_pair_type_t;

typedef void(window::* onchar_t)( HWND, TCHAR, int );
typedef boost::mpl::pair< boost::mpl::int_< WM_CHAR >::type, onchar_t >::type
	onchar_pair_type_t;

typedef void(window::* onhotkey_t)( HWND, int, UINT, UINT);
typedef boost::mpl::pair< boost::mpl::int_< WM_HOTKEY >::type, onhotkey_t >::type
	onhotkey_pair_type_t;

typedef void(window::* onpaint_t)( HWND );
typedef boost::mpl::pair< boost::mpl::int_< WM_PAINT >::type, onpaint_t >::type
	onpaint_pair_type_t;

typedef void(window::* onclose_t)( HWND );
typedef boost::mpl::pair< boost::mpl::int_< WM_CLOSE >::type, onclose_t >::type
	onclose_pair_type_t;

typedef void(window::* onsettingchange_t)( HWND, UINT, LPCTSTR );
typedef boost::mpl::pair< boost::mpl::int_< WM_SETTINGCHANGE >::type, onsettingchange_t >::type
	onsettingchange_pair_type_t;

typedef void(window::* ontimer_t)( HWND, UINT );
typedef boost::mpl::pair< boost::mpl::int_< WM_TIMER >::type, ontimer_t >::type
	ontimer_pair_type_t;

typedef void(window::* oncommand_t)( HWND hWnd, int id, HWND hwndCtl, UINT codeNotify );
typedef boost::mpl::pair< boost::mpl::int_< WM_COMMAND >::type, oncommand_t >::type
	oncommand_pair_type_t;


class window :	public atom::wwindow< window, LOKI_TYPELIST_9( onkeydown_pair_type_t, onkeyup_pair_type_t, onchar_pair_type_t, onhotkey_pair_type_t, onpaint_pair_type_t, onclose_pair_type_t, onsettingchange_pair_type_t, ontimer_pair_type_t, oncommand_pair_type_t ) >,
				public atom::node< LOKI_TYPELIST_2( window2logger, window2pref ) >,
				public boost::enable_shared_from_this< window > {
	typedef atom::wwindow< window, LOKI_TYPELIST_9( onkeydown_pair_type_t, onkeyup_pair_type_t, onchar_pair_type_t, onhotkey_pair_type_t, onpaint_pair_type_t, onclose_pair_type_t, onsettingchange_pair_type_t, ontimer_pair_type_t, oncommand_pair_type_t ) >
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
	frame_close( frame_ptr f );
	///
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
	///
	window& operator()( preferences::type const mode, atom::po::id_t const opt );

protected:
	//
	logger_ptr
	get_logger() { return ( get_slot< window2logger >().item() ); }
	//
	pref_ptr
	get_pref() { return ( get_slot< window2pref >().item() ); }
	//
	void
	update_hotkeys();
	//
	void
	update_placement();
	//
	void
	update_position( HWND hWnd, bool dir, float mult );
	//
	void
	update_accel( WORD const cmd, atom::po::id_t const opt );

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
		static area_ptr create( area_ptr p, frame_ptr f ) {
			return ( area_ptr( new area( p, f ) ) );
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
		void collect( frames_t& fs ) {
			if ( this->frame ) {
				fs.push_back( this->frame );
			} else {
				this->first->collect( fs );
				this->second->collect( fs );
			}
		}
		void split( frame_ptr f ) {
			this->vert = true;
			this->first = create( this->shared_from_this(), this->frame );
			this->second = create( this->shared_from_this(), f );
			this->frame = frame_ptr();
		}
		void rotate() {
			if ( this->parent ) {
				this->parent->vert = !this->parent->vert;  
				if ( this->parent->vert ) {
					std::swap( this->parent->first, this->parent->second );
				}
			}
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
		void draw( boost::function< void( frame_ptr const&, frame_coord const& ) > func, frame_coord const& c ) {
			if ( this->frame ) {
				func( this->frame, c );
			} else {
				fraction const l = ((this->vert)?( c.left + fraction( 1, c.width * 2 ) ):( c.left ) );
				fraction const t = ((this->vert)?( c.top ):( c.top + fraction( 1, c.height * 2 ) ) );
				unsigned int const w = ((this->vert)?(c.width * 2):(c.width));
				unsigned int const h = ((this->vert)?(c.height):(c.height * 2));
				//
				this->first->draw( func, frame_coord( c.left.get_n(), c.left.get_d(), c.top.get_n(), c.top.get_d(), w, h ) );
				this->second->draw( func, frame_coord( l.get_n(), l.get_d(), t.get_n(), t.get_d(), w, h ) );
			}
		}

	protected:
		///
		area_ptr get( bool const f ) {
			if ( f ) {
				return ( this->first );
			}
			return ( this->second );
		}

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
	POINT
		anchor;
	RECT
		in_rect;
	///
	int
		slide_dir;
	DWORD
		slide_start_time;
	UINT_PTR
		slide_timer_id;
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

