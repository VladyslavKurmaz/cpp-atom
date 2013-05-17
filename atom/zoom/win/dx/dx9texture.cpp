namespace atom { namespace zoom {

	dx9texture::dx9texture( logger_ptr l, stream_ptr s, dx9wincontext_ptr c ) : 
			texture( l, s )
		,	dx9holder( c )
		,	tex( NULL ) {
	}

	dx9texture::~dx9texture() {
		if ( tex != NULL ) {
			tex->Release();
		}
	}

	bool
	dx9texture::build( string_t const& s ) {
		if( SUCCEEDED( D3DXCreateTextureFromFile(
				this->dx9data->d3ddevice,
				s.c_str(),
				&(this->tex) ) )
			) {
			return true;
		}
		return false;
	}

	void
	dx9texture::clear() {
		texture::clear();
	}

} }
