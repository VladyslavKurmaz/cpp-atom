#pragma once

class area : public boost::enable_shared_from_this< area > {
public:
	static area_ptr create( area_ptr p, frame_ptr f ) {
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
	frame_ptr close() {
		assert( this->frame );
		frame_ptr r = frame_ptr();
		area_ptr p = this->parent;
		if ( p ){
			area_ptr a = area_ptr(); 
			if ( p->first != this->shared_from_this() ) { a = p->first; }
			if ( p->second != this->shared_from_this() ) { a = p->second; }
			//
			p->vert = a->vert;
			p->first = a->first; if ( p->first ) { p->first->parent = p; }
			p->second = a->second; if ( p->second ) { p->second->parent = p; }
			p->frame = a->frame;
			//
			p->reorder();
			r = p->find();
		}
		this->clear();
		return r;
	}
	//
	void closeAll() {
		frames_t frames;
		this->collect( frames );
		BOOST_FOREACH( frame_ptr f, frames ) {
			this->find( f )->close();
		}
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
	frame_ptr find() {
		frame_ptr result = this->frame;
		if ( !result && this->first ) { result = this->first->find(); }
		if ( !result && this->second ) { result = this->second->find(); }
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
	frame_ptr split( frame_ptr f ) {
		assert( this->frame );
		this->vert = true;
		this->first = create( this->shared_from_this(), this->frame );
		this->second = create( this->shared_from_this(), f );
		this->frame = frame_ptr();
		this->reorder();
		return f;
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
	void draw( boost::function< void( frame_ptr const&, frame_coord const&, bool const ) > func, frame_coord const& c, frame_ptr cf ) const {
		if ( this->frame ) {
			func( this->frame, c, ( this->frame == cf ) );
		} else {
			fraction const l = ((this->vert)?( c.left + fraction( 1, c.width * 2 ) ):( c.left ) );
			fraction const t = ((this->vert)?( c.top ):( c.top + fraction( 1, c.height * 2 ) ) );
			unsigned int const w = ((this->vert)?(c.width * 2):(c.width));
			unsigned int const h = ((this->vert)?(c.height):(c.height * 2));
			//
			if ( this->first ) {
				this->first->draw( func, frame_coord( c.left.get_n(), c.left.get_d(), c.top.get_n(), c.top.get_d(), w, h ), cf );
			}
			if ( this->second ) {
				this->second->draw( func, frame_coord( l.get_n(), l.get_d(), t.get_n(), t.get_d(), w, h ), cf );
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
	typedef std::vector< frame_ptr >
		frames_t;
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
		this->frame->clear();
		this->parent = this->first = this->second = area_ptr();
		this->frame = frame_ptr();
	}
	///
	void collect( frames_t& fs ) const {
		if ( this->frame ) {
			fs.push_back( this->frame );
		} else {
			if ( this->first ) {
				this->first->collect( fs );
			}
			if ( this->second ) {
				this->second->collect( fs );
			}
		}
	}
};
