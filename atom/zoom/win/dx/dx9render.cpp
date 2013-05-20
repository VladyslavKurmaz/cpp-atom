#include "./dx9render.hpp"

namespace atom { namespace zoom {

	dx9render::dx9render( logger_ptr l, canvas_ptr c ) :
			render( l, c )
		,	dx9holder( dx9wincontext_ptr ( new dx9wincontext() ) )
		,	m()
		,	t()
	{
	}

	dx9render::~dx9render() {
	}

	bool
	dx9render::init( camera_ptr c ) {
		guard_t l( this->lock );
		if ( render::init( c ) ) {
			//
			if ( this->dx9data->d3d = Direct3DCreate9( D3D_SDK_VERSION ) ) {
				D3DPRESENT_PARAMETERS& d3dpp = this->dx9data->d3dpp;
				//
				d3dpp.BackBufferWidth			=	800;
				d3dpp.BackBufferHeight			=	600;
				d3dpp.BackBufferFormat			=	D3DFMT_UNKNOWN;//D3DFMT_A8R8G8B8;
				d3dpp.BackBufferCount			=	1;

				d3dpp.MultiSampleType			=	/*D3DMULTISAMPLE_8_SAMPLES*/D3DMULTISAMPLE_NONE;
				d3dpp.MultiSampleQuality		=	0;

				d3dpp.SwapEffect				=	D3DSWAPEFFECT_DISCARD;
				d3dpp.hDeviceWindow				=	static_cast< HWND >( this->get_canvas()->get_handle() );
				d3dpp.Windowed					=	TRUE;
				d3dpp.EnableAutoDepthStencil	=	TRUE;
				d3dpp.AutoDepthStencilFormat	=	D3DFMT_D16; //D3DFMT_D24S8;
				d3dpp.Flags						=	D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;

				d3dpp.FullScreen_RefreshRateInHz=	( d3dpp.Windowed ) ? ( 0 ) : ( 0 );
				d3dpp.PresentationInterval		=	D3DPRESENT_INTERVAL_DEFAULT;
				//
				IDirect3DDevice9* d = NULL;
				if( SUCCEEDED( this->dx9data->d3d->CreateDevice(
											D3DADAPTER_DEFAULT,
											D3DDEVTYPE_HAL,
											d3dpp.hDeviceWindow,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,
											&d3dpp,
											&d ) ) )
				{
					this->dx9data->d3ddevice = d;
					m = this->create_mesh();
					m->build_sphere( 1.f, 100, 100 );

					t = this->create_texture();
					t->build( string_t( "C:\\work\\env\\bmc\\env\\centers\\src\\data\\14-contour.dds" ) );
					return true;
				}
			}
		}
		return false;
	}

	inline void	get_vector3( const D3DXMATRIX& Mtx, const int Line, D3DXVECTOR3& Vec )
	{
		Vec.x = Mtx.m[ Line ][ 0 ];
		Vec.y = Mtx.m[ Line ][ 1 ];
		Vec.z = Mtx.m[ Line ][ 2 ];
	}

	void build_viewport( float const AngleX, float const AngleY, D3DXVECTOR3& Pos, D3DXMATRIX& ViewMtx )
	{
		D3DXMATRIX	mtx;
		D3DXMATRIX	rot_x;
		D3DXMATRIX	rot_y;
		D3DXMATRIX	pos;
		//
		D3DXMatrixRotationX( &rot_x, AngleX );
		D3DXMatrixRotationY( &rot_y, AngleY );
		D3DXMatrixTranslation( &pos, Pos.x, Pos.y, Pos.z );
		//
		mtx = rot_x * rot_y * pos;
		//
		D3DXVECTOR3	v3;
		D3DXVECTOR3	v2;
		D3DXVECTOR3	v1;
		get_vector3( mtx, 3, v3 );
		get_vector3( mtx, 2, v2 );
		get_vector3( mtx, 1, v1 );
		v2 += v3;
		//
		D3DXMatrixLookAtLH( 
			&ViewMtx,
			&v3,
			&v2,
			&v1 );
	}

	bool
	dx9render::frame() {
		if ( render::frame() ) {
			//
			this->dx9data->d3ddevice->SetRenderState( D3DRS_ZENABLE, TRUE );
			this->dx9data->d3ddevice->SetRenderState( D3DRS_AMBIENT, 0xFFFFFFFF );
			this->dx9data->d3ddevice->SetRenderState( D3DRS_LIGHTING, FALSE );
			//
			float_t const w = static_cast<float_t>( this->dx9data->d3dpp.BackBufferWidth );
			float_t const h = static_cast<float_t>( this->dx9data->d3dpp.BackBufferHeight );
			float_t const r = w / h;
			float_t const fovx = D3DXToRadian( 60.f );
			float_t const fovy = 2 * atan( tan( fovx / 2.f ) / r );

			D3DXMATRIXA16 matProj;
			D3DXMatrixPerspectiveFovLH( &matProj, fovy, r, 0.1f, 200.0f );
			this->dx9data->d3ddevice->SetTransform( D3DTS_PROJECTION, &matProj );
			//                           
			D3DXMATRIX matView;
			float_t const angle_x = 0.f;
			float_t const angle_y = 0.f;
			D3DXVECTOR3 pos( 0.f, 0.f, -2.f );
	
			build_viewport( angle_x, angle_y, pos, matView );
			this->dx9data->d3ddevice->SetTransform( D3DTS_VIEW, &matView );
			//
			this->dx9data->d3ddevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 92, 92, 92 ), 1.0f, 0 );
			this->dx9data->d3ddevice->SetRenderState( D3DRS_FILLMODE, ((( GetKeyState( VK_SPACE ) & 0x80 ))?(D3DFILL_WIREFRAME):(D3DFILL_SOLID)) );

			if( SUCCEEDED( this->dx9data->d3ddevice->BeginScene() ) )
			{
				m->render();
				this->dx9data->d3ddevice->EndScene();
			}

			// Present the backbuffer contents to the display
			this->dx9data->d3ddevice->Present( NULL, NULL, NULL, NULL );
			return true;
		}
		return false;
	}

	void
	dx9render::clear(){
		guard_t l( this->lock );
		render::clear();
	}

	mesh_ptr
	dx9render::create_mesh() {
		return ( dx9mesh::create( this->get_logger(), this->dx9data ) );
	}

	texture_ptr
	dx9render::create_texture() {
		return ( dx9texture::create( this->get_logger(), this->dx9data ) );
	}

} }
