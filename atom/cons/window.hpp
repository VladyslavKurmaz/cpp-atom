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
	frame_close( frame_ptr f, frame_ptr n );
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
				if ( this->parent->left && this->parent->left != this->shared_from_this() ) { a = this->parent->left; }
				if ( this->parent->top && this->parent->top != this->shared_from_this() ) { a = this->parent->top; }
				if ( this->parent->right && this->parent->right != this->shared_from_this() ) { a = this->parent->right; }
				if ( this->parent->bottom && this->parent->bottom != this->shared_from_this() ) { a = this->parent->bottom; }
				//
				this->parent->left = a->left; if ( this->parent->left ) { this->parent->left->parent = this->parent; }
				this->parent->top = a->top; if ( this->parent->top ) { this->parent->top->parent = this->parent; }
				this->parent->right = a->right; if ( this->parent->right ) { this->parent->right->parent = this->parent; }
				this->parent->bottom = a->bottom; if ( this->parent->bottom ) { this->parent->bottom->parent = this->parent; }
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
			if ( !result && this->left ) { result = this->left->find( f ); }
			if ( !result && this->top ) { result = this->top->find( f ); }
			if ( !result && this->right ) { result = this->right->find( f ); }
			if ( !result && this->bottom ) { result = this->bottom->find( f ); }
			return ( result );
		}
		//
		void collect( frames_t& fs ) {
			if ( this->frame ) {
				fs.push_back( this->frame );
			} else {
				if ( this->left ) { this->left->collect( fs ); }
				if ( this->top ) { this->top->collect( fs ); }
				if ( this->right ) { this->right->collect( fs ); }
				if ( this->bottom ) { this->bottom->collect( fs ); }
			}
		}
		void split( frame_ptr f ) {
			this->left = create( this->shared_from_this(), this->frame );
			this->right = create( this->shared_from_this(), f );
			this->frame = frame_ptr();
		}
		void rotate( bool const cw ) {
			if ( this->parent ) {
				area_ptr b = area_ptr();
				std::swap( b, this->parent->left );
				std::swap( b, this->parent->top );
				std::swap( b, this->parent->right );
				std::swap( b, this->parent->bottom );
				std::swap( b, this->parent->left );
			}
		}

	protected:
	private:
		area_ptr
			parent;
		area_ptr
			left;
		area_ptr
			top;
		area_ptr
			right;
		area_ptr
			bottom;
		frame_ptr
			frame;
		///
		area( area_ptr p, frame_ptr f ) :
				parent( p )
			,	left()
			,	top()
			,	right()
			,	bottom()
			,	frame( f ) {
		}
		///
		void clear() {
			this->parent = this->left = this->top = this->right = this->bottom = area_ptr();
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

