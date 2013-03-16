#pragma once
#include "./classes.hpp"
#include <boost/thread/locks.hpp>
#include <boost/circular_buffer.hpp>

template < typename T >
class cons_buffer : public boost::noncopyable {
	typedef T
		elem_t;
	struct wcs {
		CRITICAL_SECTION
		cs;
		wcs(){ InitializeCriticalSection( &cs ); }
		~wcs(){ DeleteCriticalSection( &cs ); }
		 
		void lock() { EnterCriticalSection( &cs ); }
		void unlock() { LeaveCriticalSection( &cs ); }
	};
	///
	static const elem_t BEL = '\x07';
	static const elem_t LF = '\x0A';
	static const elem_t FF = '\x0C';
	static const elem_t CR = '\x0D';
	static const elem_t ESC = '\x1B';

public:
	///
	cons_buffer() :
			buf_lock()
		,	width( 0 )
		,	lines_count( 0 )
		,	buffer()
		,	lines( 0 )
		,	esc_mode( false ) {
	}
	///
	~cons_buffer(){
	}
	///
	bool
	init( size_t const w, size_t const lc ) {
		guard_t l( this->buf_lock );
		//
		this->width = w;
		this->lines_count = lc;
		//
		size_t const table_sz = this->width * this->lines_count;
		this->buffer = boost::shared_array< elem_t >( new elem_t[ table_sz ] );
		reset();
		return true;
	}
	///
	void
	reset() {
		size_t const table_sz = this->width * this->lines_count;
		memset( this->buffer.get(), 0, sizeof( elem_t ) * table_sz );
		this->lines.clear();
		this->lines.rset_capacity( this->lines_count );
		line nl = { this->buffer.get(), 0 };
		this->lines.push_back( nl );
	}
	///
	bool
	write( elem_t const* b, size_t const sz ) {
		guard_t l( this->buf_lock );
		//
		for( size_t i = 0; i < sz; ++i ) {
			line nl = { 0 };
			bool add = false;
			lines_t::reverse_iterator lit = this->lines.rbegin();
			//
			if ( this->esc_mode ) {
				if ( b[i] != '\x5d' ) {
					this->esc_mode = !( ( '\x40' <= b[i] ) && ( b[i] <= '\x7e' ) );
				}
			} else if ( b[i] == BEL ) {
				// bell
			} else if ( b[i] == LF ) {
				// line feed
				nl = (*lit);
				nl.pos = 0;
			} else if ( b[i] == FF ) {
				// form feed
				this->reset();
			} else if ( b[i] == CR ) {
				// carriage return
				(*lit).pos = 0;
			} else if ( b[i] == ESC ) {
				this->esc_mode = true;
			} else {
				if ( (*lit).pos >= this->width ) {
					nl = (*lit);
					nl.pos = 0;
				}
				add = true;
			}
			//
			if ( nl.ptr != NULL ) {
				nl.ptr += sizeof( elem_t ) * this->width;
				if ( nl.ptr >= ( this->buffer.get() + this->width * this->lines_count ) ) {
					nl.ptr = this->buffer.get();
				}
				memset( nl.ptr, 0, sizeof( elem_t ) * this->width );
				this->lines.push_back( nl );
			}
			//
			if ( add ) {
				lines_t::reverse_iterator lit = this->lines.rbegin();
				(*lit).ptr[ (*lit).pos ] = b[i];
				(*lit).pos++;
			}
		}
		return true;
	}
	///
	void for_each( size_t const first, boost::function< bool( T const*, size_t const ) > pred  ) {
		guard_t l( this->buf_lock );
		//
		lines_t::const_reverse_iterator it 	= this->lines.rbegin();
		lines_t::const_reverse_iterator eit = this->lines.rend();
		for( ; it != eit; ++it ) {
			line ml = (*it);
			if ( !pred( ml.ptr, this->width ) ) {
				break;
			}
		}
	}

protected:
private:
	///
	wcs
	buf_lock;
	///
	typedef boost::lock_guard<wcs>
	guard_t;
	///
	size_t
	width;
	///
	size_t
	lines_count;
	///
	boost::shared_array< elem_t >
		buffer;
	///
	struct line {
		elem_t* ptr;
		size_t pos;
	};
	typedef boost::circular_buffer<line>
	lines_t;
	lines_t
	lines;
	//
	bool
	esc_mode;
};