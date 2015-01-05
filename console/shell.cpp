#include "./pch.hpp"
#include "./classes.hpp"
#include "./cmds.hpp"
#include "./log.hpp"
#include "./process.hpp"
#include "./frame.hpp"
#include "./area.hpp"
#include "./shell.hpp"

shell::shell( logger_ptr l, pref_ptr p ):
		headArea()
	,	currentFrame()
	,	expandMode( false ) {
	atom::mount<shell2logger>( this, l );
	atom::mount<shell2pref>( this, p );
	//
	this->currentFrame = frame::create( l );
	this->headArea = area::create( area_ptr(), this->currentFrame ); 
}

shell::~shell() {
}

bool shell::command( int const id ) {
	switch ( id ) {
	case CMDID_SPLIT:
#ifndef STANDALONE
		this->currentFrame = this->headArea->find( this->currentFrame )->split( frame::create( get_value( boost::mpl::identity< shell2logger >() ).item() ) );
#endif
		return true;
	case CMDID_EXPAND:
		this->expandMode = !this->expandMode;
		return true;
	case CMDID_ROTATE:
		this->headArea->find( this->currentFrame )->rotate();
		return true;
	case CMDID_NEXT:
		this->currentFrame = this->headArea->find( this->currentFrame )->next();
		return true;
	case CMDID_PREV:
		this->currentFrame = this->headArea->find( this->currentFrame )->prev();
		return true;
	case CMDID_CTRL_BREAK:
		return true;
	case CMDID_CTRL_C:
		return true;
	case CMDID_CLOSE:
		this->currentFrame = this->headArea->find( this->currentFrame )->close();
		if ( !this->currentFrame ) {
			PostQuitMessage( 0 );
		}
		return true;
	case CMDID_TTY1:
	case CMDID_TTY2:
	case CMDID_TTY3:
	case CMDID_TTY4:
	case CMDID_TTY5:
	case CMDID_TTY6:
		frame_ptr f = this->headArea->get_by_index( id - CMDID_TTY1 );
		if ( f ) {
			this->currentFrame = f;
		}
		return true;
	}
	return false;
}

void shell::key( KEY_EVENT_RECORD const& k ) {
	this->currentFrame->key( k );
}

void shell::paint( paint_param_t& paintParam, RECT const& rect ) {
	struct _{
		static void __( frame_ptr const& f, frame_coord const& coord, bool const cf, paint_param_t& pp, RECT r ) {
			HDC dc = pp.dc;
			RECT rt;
			int const rw = RECT_WIDTH( r );
			int const rh = RECT_HEIGHT( r );
			rt.left 	= r.left + coord.left.get_n() * rw / coord.left.get_d();
			rt.top 		= r.top + coord.top.get_n() * rh / coord.top.get_d();
			rt.right	= rt.left + rw / coord.width;
			rt.bottom	= rt.top + rh / coord.height;
			//
			//
			FrameRect( dc, &rt, ( cf )?( pp.borderActive ):( pp.borderInactive ) );
			InflateRect( &rt, -1, -1 );
			//
			SetBkMode( dc, TRANSPARENT );
			//
			atom::shared_gdiobj<HRGN> rgn = CreateRectRgn( rt.left, rt.top, rt.right, rt.bottom );
			SelectClipRgn( dc, rgn );
			{
				SelectObject( dc, pp.sysFont );
				SetTextColor( dc, pp.sysColor );
				std::stringstream ss;
				ss << " #" << f->get_index();
				DrawText( dc, ss.str().c_str(), -1, &rt, DT_RIGHT | DT_TOP | DT_SINGLELINE );
				//
				SelectObject( dc, pp.textFont );
				SetTextColor( dc, pp.textColor );
				f->paint( dc, rt );
			}
			SelectClipRgn( dc, NULL );
		}
	};
	frame_coord coord( 0, 1, 0, 1, 1, 1 );
	if ( this->expandMode ) {
		_::__( this->currentFrame, coord, true, paintParam, rect );
	} else {
		this->headArea->draw( boost::bind( &_::__, _1, _2, _3, boost::ref( paintParam ), rect ), coord, this->currentFrame );
	}
}


void shell::clear() {
	this->headArea->closeAll();
	this->headArea = area_ptr();
	currentFrame = frame_ptr();
	base_t::clear();
}


