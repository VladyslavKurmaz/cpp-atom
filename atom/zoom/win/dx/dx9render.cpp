#include "./dx9render.hpp"

namespace atom { namespace zoom {

	dx9render::dx9render( logger_ptr l, stream_ptr s, canvas_ptr c ) :
			render( l, s, c )
		,	d3dpp()
		,	d3d()
		,	device()
	{
	}

	dx9render::~dx9render() {
	}

	bool
	dx9render::init() {
		guard_t l( this->lock );
		if ( render::init() ) {
			//
			if ( this->d3d = d3d_t( Direct3DCreate9( D3D_SDK_VERSION ) ) ) {
				memset( &(this->d3dpp), 0, sizeof( this->d3dpp ) );
				//
				d3dpp.BackBufferWidth			=	800;
				d3dpp.BackBufferHeight			=	600;
				d3dpp.BackBufferFormat			=	D3DFMT_UNKNOWN;//D3DFMT_A8R8G8B8;
				d3dpp.BackBufferCount			=	1;

				d3dpp.MultiSampleType			=	D3DMULTISAMPLE_8_SAMPLES;//D3DMULTISAMPLE_NONE;;
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
				if( SUCCEEDED( this->d3d->CreateDevice(
											D3DADAPTER_DEFAULT,
											D3DDEVTYPE_HAL,
											d3dpp.hDeviceWindow,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,
											&this->d3dpp,
											&d ) ) )
				{
					this->device = device_t( d );
					return true;
				}
			}
		}
		return false;
	}

	bool
	dx9render::frame() {
		if ( render::frame() ) {
			this->device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 92, 92, 92 ), 1.0f, 0 );
			if( SUCCEEDED( this->device->BeginScene() ) )
			{
				this->device->EndScene();
			}

			// Present the backbuffer contents to the display
			this->device->Present( NULL, NULL, NULL, NULL );
			return true;
		}
		return false;
	}

	void
	dx9render::clear(){
		guard_t l( this->lock );
		render::clear();
	}

} }
