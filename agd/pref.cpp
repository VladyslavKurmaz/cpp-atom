#include "./pch.hpp"
#include "./log.hpp"
#include "./pref.hpp" 


pref::pref( logger_ptr l ) :
		base_t()
	,	config()
	,	accel()
	,	langsPair()
	,	langs()
{
	atom::mount<pref2logger>( this, l );
	//
	// load config json user config or from resource
	// check local file
	// ????????????
	std::stringstream ss;
	{
		HMODULE handle = ::GetModuleHandle(NULL);
		HRSRC rc = ::FindResource(handle, MAKEINTRESOURCE(IDR_CONFIG),MAKEINTRESOURCE(TEXTFILE));
		HGLOBAL rcData = ::LoadResource(handle, rc);
		DWORD size = ::SizeofResource(handle, rc);
		char const* data = static_cast<const char*>(::LockResource(rcData));
		ss << data;
	}
	boost::property_tree::read_json(ss, this->config);
	//
	// configure available languages
	lang_t const en = boost::make_tuple( _T("eng"), _T("en"), _T("English"), AD_PANEL_IMAGE_LANG_EN );
	lang_t const ua = boost::make_tuple( _T("ukr"), _T("uk"), _T("Український"), AD_PANEL_IMAGE_LANG_UA );
	lang_t const ru = boost::make_tuple( _T("rus"), _T("ru"), _T("Русский"), AD_PANEL_IMAGE_LANG_RU );

	this->langs.push_back( en );
	this->langs.push_back( ua );
	this->langs.push_back( ru );
	//
	this->langSetPair( std::make_pair( en, ru ) );
}

pref::~pref() {
}

bool pref::init( int argc, char const * const argv[] ) {
	//
	this->parseAccel(commands, this->accel);
	this->accel.build();
#if 0
	// prepare paint objects
	//
	atom::string_t const d1( DELIM1 );
	atom::string_t const d2( DELIM2 );
	//
	static const struct font_t {
		atom::po::id_t	opt;
		paint_font_t&	font;
} fonts[] = {
	{ po_ui_font_text, this->paintParam.textFont },
	{ po_ui_font_sys, this->paintParam.sysFont }
};
//
BOOST_FOREACH(font_t const& font, fonts)
{
	atom::string_t s = boost::lexical_cast<atom::string_t>(this->getPref()->get< atom::po::string_t >(font.opt));
	atom::attributes< atom::char_t > a(s, d1, d2);
	unsigned int height = a.as<unsigned int>(_T("height"));
	unsigned int color = a.as_color(_T("color"));
	//
	HFONT f = CreateFont(
		height,
		0,
		0,
		0,
		FW_NORMAL,//FW_HEAVY,
		FALSE,//TRUE
		FALSE,
		FALSE,
		OEM_CHARSET,
		OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		FIXED_PITCH,
		a.as<atom::string_t>(_T("name")).c_str()
		);
	//
	if (f != NULL) {
		font.font.height = height;
		font.font.font = f;
		font.font.color = color;
	}
	else {
		*(this->getLogger()) << "Text font creation error: " << s << std::endl;
	}
}
//

atom::attributes< atom::char_t > padding(boost::lexical_cast<atom::string_t>(this->getPref()->get< atom::po::string_t >(po_ui_padding)), d1, d2);
unsigned int const pv = padding.as<unsigned int>(_T("size"));
SetRect(&this->paintParam.padding, pv, pv, pv, pv);

atom::attributes< atom::char_t > border(boost::lexical_cast<atom::string_t>(this->getPref()->get< atom::po::string_t >(po_ui_border)), d1, d2);
this->paintParam.borderActive = CreateSolidBrush(border.as_color(_T("color")));
this->paintParam.borderInactive = CreateSolidBrush(border.as_color(_T("inactive")));

#endif
#if 0
	BOOST_FOREACH(boost::property_tree::ptree::value_type const & c, r.get_child("state")) {
		*(this->getLogger()) << c.first.c_str() << std::endl;
		//std::wstring dest = boost::locale::conv::to_utf<wchar_t>( c.second.get<std::string>("translatedText"), "UTF-8" );
	}
#endif
	return true;
}

boost::property_tree::ptree const&  pref::getModeConfig(std::string const& key){
	return this->config.get_child(CONFIG_MODE + key);
}

void pref::getView(bool const clip, RECT& r) {
	SetRect(&r, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	if (clip) {
		SystemParametersInfo(SPI_GETWORKAREA, 0, &r, 0);
	}
}

alignment_t::type pref::getAlignment(){
	//
	static atom::parse_tag< char, alignment_t::type > alignment_tags[] = {
		{ "hcenter", alignment_t::hcenter },
		{ "left", alignment_t::left },
		{ "right", alignment_t::right },
		{ "hclient", alignment_t::hclient },
		{ "hmask", alignment_t::hmask },
		{ "vcenter", alignment_t::vcenter },
		{ "top", alignment_t::top },
		{ "bottom", alignment_t::bottom },
		{ "vclient", alignment_t::vclient },
		{ "vmask", alignment_t::vmask },
		{ "client", alignment_t::client },
		{ "center", alignment_t::center }
	};
	static size_t alignment_tags_count = sizeof(alignment_tags) / sizeof(alignment_tags[0]);
	//
	std::string const alig_str = this->get< std::string >(CONFIG_ALIGNMENT);
	//
	alignment_t::type align = alignment_t::client;
	atom::parse_result< char, alignment_t::type > result = atom::parse_tags(alig_str, alignment_tags, alignment_tags_count, std::string("+"));
	//
	if ((result.total_found > 0) && (result.unparsed.size() == 0)) {
		align = result.result;
	}
	else {
		//*(this->getLogger()) << "Invalid alignment format " << alig_str << std::endl;
		align = alignment_t::hcenter | alignment_t::top;
	}
	//
	return align;
}

void pref::calculateDocks(unsigned int const alpha, placement_t& windowPlacement) {

	unsigned int const width = (windowPlacement.fullScreen) ? (100) : (windowPlacement.width);
	unsigned int const height = (windowPlacement.fullScreen) ? (100) : (windowPlacement.height);
	alignment_t::type align = windowPlacement.alignment;

	RECT rt;
	this->getView(windowPlacement.clip, rt);
	//
	alignment_t::type const h_align = align & alignment_t::hmask;
	alignment_t::type const v_align = align & alignment_t::vmask;
	//
	int const rt_w = RECT_WIDTH( rt );
	int const rt_h = RECT_HEIGHT( rt );
	int const wnd_rt_w = ((h_align == alignment_t::hclient )?(rt_w):(rt_w * width / 100));
	int const wnd_rt_h = ((v_align == alignment_t::vclient) ? (rt_h) : (rt_h * height / 100));
	//
	RECT rrt;
	SetRect( &rrt, 0, 0, wnd_rt_w, wnd_rt_h );
	OffsetRect( &rrt, rt.left, rt.top );
	//
	LONG offset_x = 0;
	LONG offset_y = 0;
	LONG offset_x_inv = 0;
	LONG offset_y_inv = 0;
	if (h_align == alignment_t::hcenter) {
		offset_x = ( rt_w - RECT_WIDTH( rrt ) ) / 2;
	} else if (h_align == alignment_t::left) {
		offset_x_inv = -RECT_WIDTH( rrt );
	} else if (h_align == alignment_t::right) {
		offset_x = ( rt_w - RECT_WIDTH( rrt ) );
		offset_x_inv = RECT_WIDTH( rrt );
	}
	if (v_align == alignment_t::vcenter) {
		offset_y = ( rt_h - RECT_HEIGHT( rrt ) ) / 2;
	}
	else if (v_align == alignment_t::top) {
		offset_y_inv = -RECT_HEIGHT( rrt );
	} else if (v_align == alignment_t::bottom) {
		offset_y = ( rt_h - RECT_HEIGHT( rrt ) );
		offset_y_inv = RECT_HEIGHT( rrt );
	}
	OffsetRect( &rrt, offset_x, offset_y );
	//
	windowPlacement.alpha = alpha;
	if ( !windowPlacement.visible ) {
		windowPlacement.alpha = 0;
		OffsetRect( &rrt, offset_x_inv, offset_y_inv );
	}
	//
	windowPlacement.destination = rrt;
}

bool pref::parseHotkey(std::string const& id, hotkey& hk) {
	static atom::parse_tag< char, UINT > const hotkey_tags[] = {
		{ "win",	MOD_WIN },
		{ "ctrl",	MOD_CONTROL },
		{ "alt",	MOD_ALT },
		{ "shift",	MOD_SHIFT }
	};
	static size_t const hotkey_tags_count = sizeof( hotkey_tags ) / sizeof( hotkey_tags[0] );
	//
	atom::parse_result< char, UINT > result = atom::parse_tags( this->get< std::string >( id ), hotkey_tags, hotkey_tags_count, std::string( "+" ) );
	if ( ( result.total_found > 1 ) && ( result.unparsed.size() == 1 ) ) {
		hk.mods = MOD_NOREPEAT | result.result;
		try {
			std::stringstream ss;
			ss << result.unparsed[0];
			ss >> hk.vk;
			if ( !hk.vk ) {
				std::stringstream ss;
				ss << result.unparsed[0];
				ss >> std::hex >> hk.vk;
			}
			return ( hk.vk > 0 );
		}
		catch( std::exception& e ){
			//*(this->getLogger()) << "Invalid hotkey format " << e.what()<< std::endl;
		}
	}

	return false;
}

bool pref::translateAccel(HWND hWnd, MSG* msg){
	return this->accel.translate(hWnd, msg);
}


void pref::langSetPair( langspair_t const& lngs ) {
	this->langsPair = lngs;
}

pref::langspair_t pref::langGetPair() const {
	return this->langsPair;
}

void pref::langSetLang( bool const from, size_t const ind ) {
	if ( ( 0 <= ind ) && ( ind < this->langs.size() ) ) {
		lang_t l = this->langs[ind];
		if ( from ) {
			this->langsPair.first = l;
		} else {
			this->langsPair.second = l;
		}
	}
}

void pref::langEnum( boost::function< bool( lang_t const& ) > func ) const {
	BOOST_FOREACH( lang_t const& l, langs ) {
		if ( !func( l ) ) {
			break;
		}
	}
}




/*
bool operator==( const boost::any & lhs, const boost::any & rhs ){
const type_info& lt = lhs.type();
const type_info& rt = rhs.type();
//
if ( lt == rt ) {
if ( lt == typeid( bool ) ) {
return ( ( boost::any_cast< bool >( lhs ) ) == ( boost::any_cast< bool >( rhs ) ) );
} else if ( lt == typeid( int ) ) {
return ( ( boost::any_cast< int >( lhs ) ) == ( boost::any_cast< int >( rhs ) ) );
} else if ( lt == typeid( unsigned int ) ) {
return ( ( boost::any_cast< unsigned int >( lhs ) ) == ( boost::any_cast< unsigned int >( rhs ) ) );
} else if ( lt == typeid( std::string ) ) {
return ( ( boost::any_cast< std::string >( lhs ) ) == ( boost::any_cast< std::string >( rhs ) ) );
}
}
return false;
}

bool operator!=( const boost::any & lhs, const boost::any & rhs ){
return ( !( lhs == rhs ) );
}
*/



#if 0

///
static atom::po::id_t const po_none					=	0;
static atom::po::id_t const po_help					=	po_none + 1;
static atom::po::id_t const po_autostart			=	po_help + 1;
//[hk.*]
static atom::po::id_t const po_hk_appear			=	po_autostart + 1;
static atom::po::id_t const po_hk_entire_screen		=	po_hk_appear + 1;
static atom::po::id_t const po_hk_frame_split		=	po_hk_entire_screen + 1;
static atom::po::id_t const po_hk_frame_minmax		=	po_hk_frame_split + 1;
static atom::po::id_t const po_hk_frame_rotate		=	po_hk_frame_minmax + 1;
static atom::po::id_t const po_hk_frame_next		=	po_hk_frame_rotate + 1;
static atom::po::id_t const po_hk_frame_prev		=	po_hk_frame_next + 1;
static atom::po::id_t const po_hk_frame_close		=	po_hk_frame_prev + 1;
static atom::po::id_t const po_hk_ctrl_break		=	po_hk_frame_close + 1;
static atom::po::id_t const po_hk_ctrl_c			=	po_hk_ctrl_break + 1;
static atom::po::id_t const po_hk_tty1				=	po_hk_ctrl_c + 1;
static atom::po::id_t const po_hk_tty2				=	po_hk_tty1 + 1;
static atom::po::id_t const po_hk_tty3				=	po_hk_tty2 + 1;
static atom::po::id_t const po_hk_tty4				=	po_hk_tty3 + 1;
static atom::po::id_t const po_hk_tty5				=	po_hk_tty4 + 1;
static atom::po::id_t const po_hk_tty6				=	po_hk_tty5 + 1;
//[ui.*]
static atom::po::id_t const po_ui_timeout			=	po_hk_tty6 + 1;
static atom::po::id_t const po_ui_alignment			=	po_ui_timeout + 1;
static atom::po::id_t const po_ui_width				=	po_ui_alignment + 1;
static atom::po::id_t const po_ui_height			=	po_ui_width + 1;
static atom::po::id_t const po_ui_clip				=	po_ui_height + 1;
static atom::po::id_t const po_ui_alpha				=	po_ui_clip + 1;
static atom::po::id_t const po_ui_bk_color			=	po_ui_alpha + 1;
static atom::po::id_t const po_ui_lines_count		=	po_ui_bk_color + 1;
//[ui.font.*]
static atom::po::id_t const po_ui_font_text			=	po_ui_lines_count + 1;
static atom::po::id_t const po_ui_font_sys			=	po_ui_font_text + 1;
//[ui.margin.*]
static atom::po::id_t const po_ui_margin			=	po_ui_font_sys + 1;
//[ui.border.*]
static atom::po::id_t const po_ui_border			=	po_ui_margin + 1;
//[ui.padding.*]
static atom::po::id_t const po_ui_padding			=	po_ui_border + 1;
//[ui.scroll.*]
static atom::po::id_t const po_ui_scroll			=	po_ui_padding + 1;

//
po.
add_option(po_help, "help", "Show this help",
desc)((id2gr(po_help, pgEmpty), 0)).
add_option(po_autostart, "autostart", "Start on Windows startup",
desc, boost::program_options::value<bool>()->default_value(false))((id2gr(po_autostart, pgAutostart), 0)).
//[hk.*]
add_option(po_hk_appear, "hk.appear", "[win+][ctrl+][alt+][shift+]vk",
desc, boost::program_options::value<std::string>()->default_value("ctrl+0xC0"))((id2gr(po_hk_appear, pgHotkeys), 0)).
add_option(po_hk_entire_screen, "hk.entire-screen", "[win+][ctrl+][alt+][shift+]vk",
desc, boost::program_options::value<std::string>()->default_value("alt+enter"))((id2gr(po_hk_entire_screen, pgHotkeys), 0)).
add_option(po_hk_frame_split, "hk.split", "",
desc, boost::program_options::value<std::string>()->default_value("ctrl+S"))((id2gr(po_hk_frame_split, pgHotkeys), 0)).
add_option(po_hk_frame_minmax, "hk.expand", "",
desc, boost::program_options::value<std::string>()->default_value("ctrl+W"))((id2gr(po_hk_frame_minmax, pgHotkeys), 0)).
add_option(po_hk_frame_rotate, "hk.rotate", "",
desc, boost::program_options::value<std::string>()->default_value("ctrl+A"))((id2gr(po_hk_frame_rotate, pgHotkeys), 0)).
add_option(po_hk_frame_next, "hk.next", "",
desc, boost::program_options::value<std::string>()->default_value("ctrl+tab"))((id2gr(po_hk_frame_next, pgHotkeys), 0)).
add_option(po_hk_frame_prev, "hk.prev", "",
desc, boost::program_options::value<std::string>()->default_value("ctrl+shift+tab"))((id2gr(po_hk_frame_prev, pgHotkeys), 0)).
add_option(po_hk_frame_close, "hk.close", "",
desc, boost::program_options::value<std::string>()->default_value("ctrl+f4"))((id2gr(po_hk_frame_close, pgHotkeys), 0)).
add_option(po_hk_ctrl_break, "hk.ctrl-break", "",
desc, boost::program_options::value<std::string>()->default_value("ctrl+break"))((id2gr(po_hk_ctrl_break, pgHotkeys), 0)).
add_option(po_hk_ctrl_c, "hk.ctrl-c", "",
desc, boost::program_options::value<std::string>()->default_value("ctrl+C"))((id2gr(po_hk_ctrl_c, pgHotkeys), 0)).
add_option(po_hk_tty1, "hk.tty1", "",
desc, boost::program_options::value<std::string>()->default_value("alt+ctrl+f1"))((id2gr(po_hk_tty1, pgHotkeys), 0)).
add_option(po_hk_tty2, "hk.tty2", "",
desc, boost::program_options::value<std::string>()->default_value("alt+ctrl+f2"))((id2gr(po_hk_tty2, pgHotkeys), 0)).
add_option(po_hk_tty3, "hk.tty3", "",
desc, boost::program_options::value<std::string>()->default_value("alt+ctrl+f3"))((id2gr(po_hk_tty3, pgHotkeys), 0)).
add_option(po_hk_tty4, "hk.tty4", "",
desc, boost::program_options::value<std::string>()->default_value("alt+ctrl+f4"))((id2gr(po_hk_tty4, pgHotkeys), 0)).
add_option(po_hk_tty5, "hk.tty5", "",
desc, boost::program_options::value<std::string>()->default_value("alt+ctrl+f5"))((id2gr(po_hk_tty5, pgHotkeys), 0)).
add_option(po_hk_tty6, "hk.tty6", "",
desc, boost::program_options::value<std::string>()->default_value("alt+ctrl+f6"))((id2gr(po_hk_tty6, pgHotkeys), 0)).
//[ui.*]
add_option(po_ui_timeout, "ui.timeout", "",
desc, boost::program_options::value<unsigned int>()->default_value(250))((id2gr(po_ui_timeout, pgEmpty), 0)).
add_option(po_ui_alignment, "ui.alignment", "[left|right|hcenter|hclient+][top|bottom|vcenter|vclient+][client][center]",
desc, boost::program_options::value<std::string>()->default_value("hclient+top"))((id2gr(po_ui_alignment, pgWindow), 0)).
add_option(po_ui_width, "ui.width", "",
desc, boost::program_options::value<unsigned int>()->default_value(50))((id2gr(po_ui_width, pgWindow), 0)).
add_option(po_ui_height, "ui.height", "",
desc, boost::program_options::value<unsigned int>()->default_value(50))((id2gr(po_ui_height, pgWindow), 0)).
add_option(po_ui_clip, "ui.clip", "",
desc, boost::program_options::value<bool>()->default_value(false))((id2gr(po_ui_clip, pgWindow), 0)).
add_option(po_ui_alpha, "ui.alpha", "",
desc, boost::program_options::value<unsigned int>()->default_value(0x40))((id2gr(po_ui_alpha, pgUI), 0)).
//desc,	boost::program_options::value<unsigned int>()->default_value( 0xF0 ))( ( id2gr( po_ui_alpha, pgUI ), 0 ) ).
add_option(po_ui_bk_color, "ui.bk-color", "",
desc, boost::program_options::value<unsigned int>()->default_value(0x0F0F0F))((id2gr(po_ui_bk_color, pgUI), 0)).
add_option(po_ui_lines_count, "ui.lines-count", "",
desc, boost::program_options::value<unsigned int>()->default_value(500))((id2gr(po_ui_lines_count, pgUI), 0)).
//[ui.font.*]
add_option(po_ui_font_text, "ui.font.text", "",
desc, boost::program_options::value<std::string>()->default_value("name:Consolas;height:16;color:FFFFFF"))((id2gr(po_ui_font_text, pgFonts), 0)).
add_option(po_ui_font_sys, "ui.font.sys", "",
desc, boost::program_options::value<std::string>()->default_value("name:Consolas;height:12;color:008000"))((id2gr(po_ui_font_sys, pgFonts), 0)).
//[ui.margin.*]
add_option(po_ui_margin, "ui.margin", "",
desc, boost::program_options::value<std::string>()->default_value("size:0"))((id2gr(po_ui_margin, pgUI), 0)).
//[ui.border.*]
add_option(po_ui_border, "ui.border", "",
desc, boost::program_options::value<std::string>()->default_value("color:FFFFFF;inactive:404040"))((id2gr(po_ui_border, pgUI), 0)).
//[ui.padding.*]
add_option(po_ui_padding, "ui.padding", "",
desc, boost::program_options::value<std::string>()->default_value("size:1"))((id2gr(po_ui_padding, pgUI), 0)).
//[ui.scroll.*]
add_option(po_ui_scroll, "ui.scroll", "",
desc, boost::program_options::value<std::string>()->default_value("size:2;color:008000"))((id2gr(po_ui_scroll, pgUI), 0));

#endif