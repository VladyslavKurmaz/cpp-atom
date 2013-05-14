
namespace atom { namespace zoom {

	dx9texture::dx9texture( logger_ptr l, stream_ptr s, dx9_ptr d ) : 
			texture( l, s )
		,	dx9holder( d )
		,	tex( NULL ) {
	}

	dx9mesh::~dx9mesh() {
		if ( tex != NULL ) {
			tex->Release();
		}
	}

	bool
	dx9mesh::build_sphere( float_t radius, uint_t slices, uint_t stacks ) {
		if( SUCCEEDED( D3DXCreateSphere(
									this->dx9data->d3ddevice,
									radius,
									slices,
									stacks,
				  				&this->dxmesh,
									NULL ) ) ) {
			return true;
		}
		return false;
	}

	void
	dx9mesh::render() {
		D3DXMATRIX wmtx;
		D3DXMatrixIdentity( &wmtx );
		this->dx9data->d3ddevice->SetTransform( D3DTS_WORLD, &wmtx );
		dxmesh->DrawSubset( 0 );
	}

	void
	dx9mesh::clear() {
		mesh::clear();
	}

} }
