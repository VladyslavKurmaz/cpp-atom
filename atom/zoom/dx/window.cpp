#include "./pch.hpp"
#include "./window3d.hpp"
#include <time.h>
#include <math.h>

LPDIRECT3D9         d3d = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9   device = NULL; // Our rendering device

LPD3DXMESH          g_pMesh = NULL; // Our mesh object in sysmem
D3DMATERIAL9*       g_pMeshMaterials = NULL; // Materials for our mesh
LPDIRECT3DTEXTURE9* g_pMeshTextures = NULL; // Textures for our mesh
DWORD               g_dwNumMaterials = 0L;   // Number of mesh materials

extern POINT position;

class model
{
public:
	typedef boost::shared_ptr< mesh >
		mesh_ptr;
	mesh_ptr
		mesh;
	D3DXMATRIX
		wmtx;
	float
		rotate_y;
	float
		offset_y;
	int
		vk;
	float
		phase_ry;
	float
		phase_oy;
	POINT*
		pos_ptr;
};

typedef std::vector< model >
	scene_t;

inline void	GetVector3( const D3DXMATRIX& Mtx, const int Line, D3DXVECTOR3& Vec )
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
	GetVector3( mtx, 3, v3 );
	GetVector3( mtx, 2, v2 );
	GetVector3( mtx, 1, v1 );
	v2 += v3;
	//
	D3DXMatrixLookAtLH( 
		&ViewMtx,
		&v3,
		&v2,
		&v1 );
}

static void get_center_pos( HWND hWnd, POINT& pt )
{
	RECT rect;
	GetClientRect( hWnd, &rect );
	//
	pt.x = rect.right / 2;
	pt.y = rect.bottom /2;
	//
	MapWindowPoints( hWnd, NULL, &pt, 1 );
}

void window3d::onlbuttondown(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags) {
	SetCapture( hWnd );
	//
	POINT pt;
	GetCursorPos( &pt );
	//
	POINT c_pt;
	get_center_pos( hWnd, c_pt );
	//
	mouse_dx = 0;
	mouse_dy = 0;
	//
	SetCursorPos( c_pt.x, c_pt.y );
}

void window3d::onlbuttonup(HWND hWnd, int x, int y, UINT keyFlags){
	if ( GetCapture() == hWnd )
		ReleaseCapture();
}

void window3d::onmousemove(HWND hWnd, int x, int y, UINT keyFlags){
	if ( GetCapture() == hWnd ) {
		POINT c_pt;
		get_center_pos( hWnd, c_pt );
		//
		POINT pt;
		pt.x = x;
		pt.y = y;
		MapWindowPoints( hWnd, NULL, &pt, 1 );
		//
		mouse_dx += ( pt.x - c_pt.x );
		mouse_dy += ( pt.y - c_pt.y );
		//
		if ( ( c_pt.x != pt.x ) || ( c_pt.y != pt.y ) )
			SetCursorPos( c_pt.x, c_pt.y );
		SetCursor( NULL );
	}
}

void window3d::onclose( HWND hWnd ) {
	PostQuitMessage( 0 );
}


void window3d::render()
{
	TCHAR p[MAX_PATH];
	GetCurrentDirectory( MAX_PATH, p );
	if( NULL == ( d3d = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return;
	RECT rect;
	GetClientRect( this->get_hwnd(), &rect );
	//
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );
	//
	d3dpp.Windowed					= TRUE;
	d3dpp.BackBufferCount			= 1;
	d3dpp.MultiSampleType			= D3DMULTISAMPLE_8_SAMPLES;//D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality		= 0;
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil	= TRUE;
	d3dpp.hDeviceWindow				= this->get_hwnd();
	d3dpp.Flags						= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL
#ifdef DEF_GUI_DIALOGS
		| D3DPRESENTFLAG_LOCKABLE_BACKBUFFER
#endif
		;
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16; //D3DFMT_D24S8
	d3dpp.BackBufferWidth			= rect.right;
	d3dpp.BackBufferHeight			= rect.bottom;
	d3dpp.BackBufferFormat			= D3DFMT_UNKNOWN;//D3DFMT_A8R8G8B8
	d3dpp.FullScreen_RefreshRateInHz= 0;
	d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_DEFAULT;
	//
	// Create the D3DDevice
	if( FAILED( d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		this->get_hwnd(),
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&device ) ) )
	{
		return;
	}
	device->SetRenderState( D3DRS_ZENABLE, TRUE );
	device->SetRenderState( D3DRS_AMBIENT, 0xFFFFFFFF );
	device->SetRenderState( D3DRS_LIGHTING, FALSE );


	float const w = (float)d3dpp.BackBufferWidth;
	float const h = (float)d3dpp.BackBufferHeight;
	float const r = w / h;
	float const fovx = D3DXToRadian( 60.f );
	float const fovy = 2 * atan( tan( fovx / 2.f ) / r );

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, fovy, r, 0.1f, 200.0f );
	device->SetTransform( D3DTS_PROJECTION, &matProj );

	struct _
	{
		static bool __( window3d& w, LPDIRECT3DDEVICE9 device, scene_t& scene, DWORD& t )
		{
			static struct cams_t {
				float x;
				float y;
				float z;
				float rx;
				float ry;
			} cams[] = {
				{ 30.28f, 11.46f, -7.67f, 0.53f, 1.13f },
				{ 83.84f, 19.29f, -31.90f, 0.47f, -0.85f },
				{ 53.40f, 8.78f, -10.09f, 0.47f, -0.88f },
				{ 88.01f, 36.38f, -36.71f, 0.60f, -0.93f }
			};
			static size_t cams_cnt = sizeof( cams ) / sizeof( cams[0] );
			static size_t cam_i = 0;

			static D3DXVECTOR3 cam( 30.28f, 11.46f, -7.67f );
			static float angle_x = 0.53f;
			static float angle_y = 1.13f;
			static bool down = false;

			DWORD time = timeGetTime();
			float dt = ( (float)time - float(t) ) / 1000.f;
			t = time;
			//
			float const speed = 10.f;
			D3DXMATRIX	mtx;
			D3DXMatrixRotationY( &mtx, angle_y );

			if ( GetKeyState( 'W' ) & 0x80 ) cam += *((D3DXVECTOR3*)&mtx._31) * speed * dt;
			if ( GetKeyState( 'S' ) & 0x80 ) cam -= *((D3DXVECTOR3*)&mtx._31) * speed * dt;
			if ( GetKeyState( 'A' ) & 0x80 ) cam -= *((D3DXVECTOR3*)&mtx._11) * speed * dt;
			if ( GetKeyState( 'D' ) & 0x80 ) cam += *((D3DXVECTOR3*)&mtx._11) * speed * dt;
			if ( GetKeyState( 'Q' ) & 0x80 ) cam += *((D3DXVECTOR3*)&mtx._21) * speed * dt;
			if ( GetKeyState( 'E' ) & 0x80 ) cam -= *((D3DXVECTOR3*)&mtx._21) * speed * dt;
			if ( GetKeyState( VK_ESCAPE ) & 0x80 ) {
				down = true;
			} else if ( down ) {
				down = false;
#if 1
				cam.x = cams[ cam_i ].x;
				cam.y = cams[ cam_i ].y;
				cam.z = cams[ cam_i ].z;
				angle_x = cams[ cam_i ].rx;
				angle_y = cams[ cam_i ].ry;
				cam_i++;
				if ( cam_i >= cams_cnt ) {
					cam_i = 0;
				}
#else
				char s[1024] = { 0 };
				sprintf_s( s, "%.2f %.2f %.2f %.2f %.2f", cam.x, cam.y, cam.z, angle_x, angle_y );
				MessageBox( 0, s, "", MB_OK );
#endif
			}


			//
			if ( w.mouse_dx || w.mouse_dy )
			{
				angle_y += (float)w.mouse_dx / 1000.f;
				angle_x += (float)w.mouse_dy / 1000.f;
				//std::cout << "!" << w.mouse_dx << " " << w.mouse_dy << " " << angle_y << std::endl;
				w.mouse_dx = w.mouse_dy = 0;
			}
			// Clear the backbuffer and the zbuffer
			device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
				D3DCOLOR_XRGB( 92, 92, 92 ), 1.0f, 0 );
			//
			device->SetRenderState( D3DRS_FILLMODE, ((( GetKeyState( VK_SPACE ) & 0x80 ))?(D3DFILL_WIREFRAME):(D3DFILL_SOLID)) );
			// Begin the scene
			if( SUCCEEDED( device->BeginScene() ) )
			{
				D3DXMATRIX matView;
				build_viewport( angle_x, angle_y, cam, matView );
				device->SetTransform( D3DTS_VIEW, &matView );
				//
				D3DXMATRIX wmtx;
				int i = 0;
				for( scene_t::iterator it = scene.begin(); it != scene.end(); ++it )
				{
					if ( !(*it).vk || ( (*it).vk && GetKeyState( (*it).vk ) & 0x01 ) )
					{
						if ( (*it).mesh )
						{
							wmtx = (*it).wmtx;
							if ( (*it).rotate_y )
							{
								D3DXMATRIX roty;
								D3DXMatrixRotationY( &roty, (*it).rotate_y * ( (float)time + (*it).phase_ry ) );
								D3DXMatrixMultiply( &wmtx, &roty, &wmtx );
							}
							if ( (*it).offset_y )
							{
								wmtx._42 += (*it).offset_y * sin( ( (float)time + (*it).phase_oy )/500.f );
							}
							if ( (*it).pos_ptr != NULL )
							{
								wmtx._41 = 34.f+0.5f+40.f*(float)((*it).pos_ptr->x)/1107.f;
							}

							device->SetTransform( D3DTS_WORLD, &wmtx );
							(*it).mesh->render( device );
						}
					}
					++i;
				}
				// End the scene
				device->EndScene();
			}

			// Present the backbuffer contents to the display
			device->Present( NULL, NULL, NULL, NULL );
			return true;
		}
	};

	{
		enum model_type_t { mtMesh, mtText, mtSphere };

		D3DXMATRIX matScale;
		D3DXMatrixScaling( &matScale, 1.f, 1.f, 1.f );

		D3DXMATRIX matScaleWifi;
		D3DXMatrixScaling( &matScaleWifi, 1.5f, 1.5f, 1.5f );

		D3DXMATRIX matScalePrinter;
		D3DXMatrixScaling( &matScalePrinter, 1.0f, 1.0f, 1.0f );

		D3DXMATRIX matScaleArror;
		D3DXMatrixScaling( &matScaleArror, 1.5f, 1.5f, 1.5f );

		D3DXMATRIX matScaleRack;
		D3DXMatrixScaling( &matScaleRack, 2.0f, 2.0f, 2.0f );

		D3DXMATRIX matScalePC;
		D3DXMatrixScaling( &matScalePC, 1.5f, 1.5f, 1.5f );

		D3DXMATRIX matScaleSphere;
		D3DXMatrixScaling( &matScaleSphere, 0.5f, 0.5f, 0.5f );

		struct md
		{
			model_type_t type;
			char* 		name;
			float 		offs_x;
			float 		offs_y;
			float 		offs_z;
			D3DXMATRIX& scale;
			float 		rotate_y;
			float 		offset_y;
			float 		radius;
			char* 		texture;
			int 		vk;
			POINT*		pos_ptr;
		} mdls[] = {

			{ mtMesh,	"stairs-01.x", 	-76.f, 0.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,	 	0,	NULL },
			{ mtMesh,	"area-l.x", 	-54.f, 0.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,		0,	NULL },
			{ mtMesh,	"stairs-02.x", 	-29.5f, 0.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,		0,	NULL },
			{ mtMesh,	"area-c.x", 	0.f, 0.f, 0.f,				matScale,		0.f, 0.f,		0,		NULL,		0,	NULL },
			{ mtMesh,	"stairs-02.x", 	29.5f, 0.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,		0,	NULL },
			{ mtMesh,	"area-r.x", 	54.f, 0.f, 0.f,				matScale,		0.f, 0.f,		0,		NULL,		0,	NULL },
			{ mtMesh,	"stairs-01.x", 	76.f, 0.f, 0.f,				matScale,		0.f, 0.f,		0,		NULL,	 	0,	NULL },

			//#if 0
			//
			{ mtMesh,	"stairs-01.x", 	-76.f, -3.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,	 	0, NULL },
			{ mtMesh,	"area-l-tile.x",-54.f, -3.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,		0, NULL },
			{ mtMesh,	"stairs-02.x", 	-29.5f, -3.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,		0, NULL },
			{ mtMesh,	"area-c-tile.x",0.f, -3.f, 0.f,				matScale,		0.f, 0.f,		0,		NULL,		0, NULL },
			{ mtMesh,	"stairs-02.x", 	29.5f, -3.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,		0, NULL },
			{ mtMesh,	"area-r-tile.x",54.f, -3.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,		0, NULL },
			{ mtMesh,	"stairs-01.x", 	76.f, -3.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,	 	0, NULL },
			//
			{ mtMesh,	"stairs-01.x", 	-76.f, -6.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,	 	0, NULL },
			{ mtMesh,	"area-l-tile.x",-54.f, -6.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,		0, NULL },
			{ mtMesh,	"stairs-02.x", 	-29.5f, -6.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,		0, NULL },
			{ mtMesh,	"area-c-tile.x",0.f, -6.f, 0.f,				matScale,		0.f, 0.f,		0,		NULL,		0, NULL },
			{ mtMesh,	"stairs-02.x", 	29.5f, -6.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,		0, NULL },
			{ mtMesh,	"area-r-tile.x",54.f, -6.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,		0, NULL },
			{ mtMesh,	"stairs-01.x", 	76.f, -6.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,	 	0, NULL },
			//
			{ mtMesh,	"stairs-01.x", 	-76.f, -9.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,	 	0, NULL },
			{ mtMesh,	"area-l-tile.x",-54.f, -9.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,		0, NULL },
			{ mtMesh,	"stairs-02.x", 	-29.5f, -9.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,		0, NULL },
			{ mtMesh,	"area-c-tile.x",0.f, -9.f, 0.f,				matScale,		0.f, 0.f,		0,		NULL,		0, NULL },
			{ mtMesh,	"stairs-02.x", 	29.5f, -9.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,		0, NULL },
			{ mtMesh,	"area-r-tile.x",54.f, -9.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,		0, NULL },
			{ mtMesh,	"stairs-01.x", 	76.f, -9.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,	 	0, NULL },
			//
			{ mtMesh,	"stairs-01.x", 	-76.f, -12.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,	 	0, NULL },
			{ mtMesh,	"area-l-tile.x",-54.f, -12.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,		0, NULL },
			{ mtMesh,	"stairs-02.x", 	-29.5f, -12.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,		0, NULL },
			{ mtMesh,	"area-c-tile.x",0.f, -12.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,		0, NULL },
			{ mtMesh,	"stairs-02.x", 	29.5f, -12.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,		0, NULL },
			{ mtMesh,	"area-r-tile.x",54.f, -12.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,		0, NULL },
			{ mtMesh,	"stairs-01.x", 	76.f, -12.f, 0.f,			matScale,		0.f, 0.f,		0,		NULL,	 	0, NULL },
			//
			//#endif
			{ mtMesh,	"wifispot-01.x", 	43.53f,	1.f, 4.11f,		matScaleWifi,	0.001f, 0.f,	0,		NULL,			0,	NULL },
			{ mtMesh,	"wifispot-01.x", 	43.53f,	1.f, -5.69f,	matScaleWifi,	0.001f, 0.f,	0,		NULL,			0,	NULL },
			{ mtSphere,	NULL, 				34.5f,	1.f, -1.f,		matScaleSphere,	0.f, 0.f,		.75f,	"green.jpg",	0, &position },

			{ mtMesh,	"printer-01.x", 	40.5f, 0.f, -0.5f,		matScalePrinter,0.f, 0.f,		0,		NULL,			VK_NUMLOCK,	NULL },
			{ mtMesh,	"arrow-01.x", 		40.5f, 1.f, -0.5f,		matScaleArror,	0.005f, 0.f,	0,		NULL,			VK_NUMLOCK,	NULL },

			{ mtMesh,	"serverrack-01.x", 	38.75f, 0.f, 3.0f,		matScaleRack,	0.f, 0.f,		0,		NULL,			VK_NUMLOCK,	NULL },
			{ mtMesh,	"arrow-01.x", 		38.75f, 2.5f, 3.0f,		matScaleArror,	0.005f, 0.f,	0,		NULL,			VK_NUMLOCK,	NULL },

			{ mtMesh,	"printer-01.x", 	70.f, 0.f, -0.5f,		matScalePrinter,0.f, 0.f,		0,		NULL,			VK_NUMLOCK,	NULL },
			{ mtMesh,	"arrow-01.x", 		70.f, 1.f, -0.5f,		matScaleArror,	0.005f, 0.f,	0,		NULL,			VK_NUMLOCK,	NULL },

			{ mtMesh,	"pc-01.x", 			41.79f,	0.5f,	-8.98f,				matScalePC,	.0f, .0f,		.0f,	NULL,		VK_NUMLOCK },
			{ mtMesh,	"arrow-01.x", 		41.79f,	1.5f,	-8.98f,				matScalePC,	.005f, .0f,		.0f,	NULL,		VK_NUMLOCK },
			{ mtMesh,	"pc-01.x", 			45.19f,	0.5f,	7.63f,				matScalePC,	.0f, .0f,		.0f,	NULL,		VK_NUMLOCK },
			{ mtMesh,	"arrow-01.x", 		45.19f,	1.5f,	7.63f,				matScalePC,	.005f, .0f,		.0f,	NULL,		VK_NUMLOCK },
			{ mtMesh,	"pc-01.x", 			57.48f,	0.5f,	4.28f,				matScalePC,	.0f, .0f,		.0f,	NULL,		VK_NUMLOCK },
			{ mtMesh,	"arrow-01.x", 		57.48f,	1.5f,	4.28f,				matScalePC,	.005f, .0f,		.0f,	NULL,		VK_NUMLOCK },
			{ mtMesh,	"pc-01.x", 			60.76f,	0.5f,	-4.14f,				matScalePC,	.0f, .0f,		.0f,	NULL,		VK_NUMLOCK },
			{ mtMesh,	"arrow-01.x", 		60.76f,	1.5f,	-4.14f,				matScalePC,	.005f, .0f,		.0f,	NULL,		VK_NUMLOCK },
			{ mtMesh,	"pc-01.x", 			66.15f,	0.5f,	4.31f,				matScalePC,	.0f, .0f,		.0f,	NULL,		VK_NUMLOCK },
			{ mtMesh,	"arrow-01.x", 		66.15f,	1.5f,	4.31f,				matScalePC,	.005f, .0f,		.0f,	NULL,		VK_NUMLOCK },
			{ mtMesh,	"pc-01.x", 			67.67f,	0.5f,	4.31f,				matScalePC,	.0f, .0f,		.0f,	NULL,		VK_NUMLOCK },
			{ mtMesh,	"arrow-01.x", 		67.67f,	1.5f,	4.31f,				matScalePC,	.005f, .0f,		.0f,	NULL,		VK_NUMLOCK },
			{ mtMesh,	"pc-01.x", 			62.57f,	0.5f,	9.25f,				matScalePC,	.0f, .0f,		.0f,	NULL,		VK_NUMLOCK },
			{ mtMesh,	"arrow-01.x",		62.57f,	1.5f,	9.25f,				matScalePC,	.005f, .0f,		.0f,	NULL,		VK_NUMLOCK },
		};
		size_t mdls_cnt = sizeof( mdls ) / sizeof( mdls[0] );
		//
		scene_t scene;
		scene.resize( mdls_cnt );


		for( size_t i = 0; i < mdls_cnt; ++i )
		{
			scene[i].mesh = model::mesh_ptr( new mesh );
			switch( mdls[i].type )
			{
			case mtMesh:
				scene[i].mesh->load( std::cout, device, mdls[i].name );
				break;
			case mtText:
				scene[i].mesh->load_text( std::cout, device, mdls[i].name, mdls[i].texture );
				break;
			case mtSphere:
				scene[i].mesh->load_sphere( std::cout, device, mdls[i].radius, mdls[i].texture );
				break;
			}
			D3DXMatrixTranslation( &(scene[i].wmtx), mdls[i].offs_x, mdls[i].offs_y, mdls[i].offs_z );
			D3DXMatrixMultiply( &(scene[i].wmtx), &(mdls[i].scale), &(scene[i].wmtx) );
			scene[i].rotate_y 	= mdls[i].rotate_y;
			scene[i].offset_y 	= mdls[i].offset_y;
			scene[i].vk 		= mdls[i].vk;
			scene[i].phase_ry 	= 1000.f * (float)rand() / RAND_MAX;
			scene[i].phase_oy 	= 1000.f * (float)rand() / RAND_MAX;
			scene[i].pos_ptr 	= mdls[i].pos_ptr;
		}

		DWORD t = timeGetTime();
		run( boost::bind( _::__, boost::ref( *this ), device, boost::ref( scene ), boost::ref( t ) ), 0 );
	}

	if( device != NULL )
		device->Release();

	if( d3d != NULL )
		d3d->Release();
}

