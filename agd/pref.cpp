#include "./pch.hpp"
#include "./log.hpp"
#include "./pref.hpp"

#if 0

default -> shared -> global -> user -> local 
	*[global]-computer/dropbox, [user]-computer-user, [instance|local] dpm-console instance settings

	profile.location.shared
	profile.location.global
	profile.location.user
	profile.location.local

	hk.edit.exec VK_RETURN
	hk.edit.ml.enter VK_CONTROL + VK_RETURN
	hk.edit.ml.new-line VK_RETURN
	hk.edit.ml.exec VK_CONTROL + VK_RETURN

	distance between lines
	background image
	enable/bisable bell
	child process interaption hotket ctrl+c|ctrl+break

#endif


pref::pref( logger_ptr l ) :
		base_t()
	,	po()
	,	langsPair()
	,	langs()
	,	prefGroups()
	,	processCallbacks()
{
	atom::mount<pref2logger>( this, l );
	atom::po::options_description_t& desc = this->po.add_desc( 0, "program options" );
	//
	std::stringstream ss;
	ss	<<	"{"
		<<	" \"data\": {"
		<<	"  \"translations\": ["
		<<	"   {"
		<<	"    \"translatedText\": \"Find\""
		<<	"   }"
		<<	"  ]"
		<<	" }"
		<<	"}";

	boost::property_tree::ptree r;
	boost::property_tree::read_json( ss, r );


	BOOST_FOREACH( boost::property_tree::ptree::value_type const & c, r.get_child( "data.translations" )) {
		std::wstring dest = boost::locale::conv::to_utf<wchar_t>( c.second.get<std::string>("translatedText"), "UTF-8" );
		break;
	}

	//
	po.
		add_option( po_help,			"help",				"Show this help",
		desc )( ( id2gr( po_help, pgEmpty ), 0 ) ).
		add_option( po_autostart,		"autostart",		"Start on Windows startup",
		desc,	boost::program_options::value<bool>()->default_value( false ) )( ( id2gr( po_autostart, pgAutostart ), 0 ) ).
		//[hk.*]
		add_option( po_hk_appear,		"hk.appear",		"[win+][ctrl+][alt+][shift+]vk",
		desc,	boost::program_options::value<std::string>()->default_value("ctrl+0xC0") )( ( id2gr( po_hk_appear, pgHotkeys ), 0 ) ).
		add_option( po_hk_entire_screen,"hk.entire-screen",	"[win+][ctrl+][alt+][shift+]vk",
		desc,	boost::program_options::value<std::string>()->default_value("alt+enter") )( ( id2gr( po_hk_entire_screen, pgHotkeys ), 0 ) ).
		add_option( po_hk_frame_split,	"hk.split",			"",	
		desc,	boost::program_options::value<std::string>()->default_value("ctrl+S") )( ( id2gr( po_hk_frame_split, pgHotkeys ), 0 ) ).
		add_option( po_hk_frame_minmax,	"hk.expand",		"",
		desc,	boost::program_options::value<std::string>()->default_value("ctrl+W") )( ( id2gr( po_hk_frame_minmax, pgHotkeys ), 0 ) ).
		add_option( po_hk_frame_rotate,	"hk.rotate",		"",
		desc,	boost::program_options::value<std::string>()->default_value("ctrl+A") )( ( id2gr( po_hk_frame_rotate, pgHotkeys ), 0 ) ).
		add_option( po_hk_frame_next,	"hk.next",			"",
		desc,	boost::program_options::value<std::string>()->default_value("ctrl+tab") )( ( id2gr( po_hk_frame_next, pgHotkeys ), 0 ) ).
		add_option( po_hk_frame_prev,	"hk.prev",			"",
		desc,	boost::program_options::value<std::string>()->default_value("ctrl+shift+tab") )( ( id2gr( po_hk_frame_prev, pgHotkeys ), 0 ) ).
		add_option( po_hk_frame_close,	"hk.close",			"",
		desc,	boost::program_options::value<std::string>()->default_value("ctrl+f4") )( ( id2gr( po_hk_frame_close, pgHotkeys ), 0 ) ).
		add_option( po_hk_ctrl_break,	"hk.ctrl-break",	"",
		desc,	boost::program_options::value<std::string>()->default_value("ctrl+break") )( ( id2gr( po_hk_ctrl_break, pgHotkeys ), 0 ) ).
		add_option( po_hk_ctrl_c,		"hk.ctrl-c",		"",
		desc,	boost::program_options::value<std::string>()->default_value("ctrl+C") )( ( id2gr( po_hk_ctrl_c, pgHotkeys ), 0 ) ).
		add_option( po_hk_tty1,			"hk.tty1",			"",
		desc,	boost::program_options::value<std::string>()->default_value("alt+ctrl+f1") )( ( id2gr( po_hk_tty1, pgHotkeys ), 0 ) ).
		add_option( po_hk_tty2,			"hk.tty2",			"",
		desc,	boost::program_options::value<std::string>()->default_value("alt+ctrl+f2") )( ( id2gr( po_hk_tty2, pgHotkeys ), 0 ) ).
		add_option( po_hk_tty3,			"hk.tty3",			"",
		desc,	boost::program_options::value<std::string>()->default_value("alt+ctrl+f3") )( ( id2gr( po_hk_tty3, pgHotkeys ), 0 ) ).
		add_option( po_hk_tty4,			"hk.tty4",			"",
		desc,	boost::program_options::value<std::string>()->default_value("alt+ctrl+f4") )( ( id2gr( po_hk_tty4, pgHotkeys ), 0 ) ).
		add_option( po_hk_tty5,			"hk.tty5",			"",
		desc,	boost::program_options::value<std::string>()->default_value("alt+ctrl+f5") )( ( id2gr( po_hk_tty5, pgHotkeys ), 0 ) ).
		add_option( po_hk_tty6,			"hk.tty6",			"",
		desc,	boost::program_options::value<std::string>()->default_value("alt+ctrl+f6") )( ( id2gr( po_hk_tty6, pgHotkeys ), 0 ) ).
		//[ui.*]
		add_option( po_ui_timeout,		"ui.timeout",		"",
		desc,	boost::program_options::value<unsigned int>()->default_value( 250 ) )( ( id2gr( po_ui_timeout, pgEmpty ), 0 ) ).
		add_option( po_ui_alignment,	"ui.alignment",		"[left|right|hcenter|hclient+][top|bottom|vcenter|vclient+][client][center]",
		desc,	boost::program_options::value<std::string>()->default_value( "hclient+top" ) )( ( id2gr( po_ui_alignment, pgWindow ), 0 ) ).
		add_option( po_ui_width,		"ui.width",			"",
		desc,	boost::program_options::value<unsigned int>()->default_value( 50 ))( ( id2gr( po_ui_width, pgWindow ), 0 ) ).
		add_option( po_ui_height,		"ui.height",		"",
		desc,	boost::program_options::value<unsigned int>()->default_value( 100 ))( ( id2gr( po_ui_height, pgWindow ), 0 ) ).
		add_option( po_ui_clip,			"ui.clip",			"",
		desc,	boost::program_options::value<bool>()->default_value( false ))( ( id2gr( po_ui_clip, pgWindow ), 0 ) ).
		add_option( po_ui_alpha,		"ui.alpha",			"",
		desc,	boost::program_options::value<unsigned int>()->default_value( 0x40 ))( ( id2gr( po_ui_alpha, pgUI ), 0 ) ).
		//desc,	boost::program_options::value<unsigned int>()->default_value( 0xF0 ))( ( id2gr( po_ui_alpha, pgUI ), 0 ) ).
		add_option( po_ui_bk_color,		"ui.bk-color",		"",
		desc,	boost::program_options::value<unsigned int>()->default_value( 0x0F0F0F ) )( ( id2gr( po_ui_bk_color, pgUI ), 0 ) ).
		add_option( po_ui_lines_count,	"ui.lines-count",	"",
		desc,	boost::program_options::value<unsigned int>()->default_value( 500 ) )( ( id2gr( po_ui_lines_count, pgUI ), 0 ) ).
		//[ui.font.*]
		add_option( po_ui_font_text,	"ui.font.text",		"",
		desc,	boost::program_options::value<std::string>()->default_value( "name:Consolas;height:16;color:FFFFFF" ))( ( id2gr( po_ui_font_text, pgFonts ), 0 ) ).
		add_option( po_ui_font_sys,		"ui.font.sys",		"",
		desc,	boost::program_options::value<std::string>()->default_value( "name:Consolas;height:12;color:008000" ) )( ( id2gr( po_ui_font_sys, pgFonts ), 0 ) ).
		//[ui.margin.*]
		add_option( po_ui_margin,		"ui.margin",		"",	
		desc,	boost::program_options::value<std::string>()->default_value( "size:0" ) )( ( id2gr( po_ui_margin, pgUI ), 0 ) ).
		//[ui.border.*]
		add_option( po_ui_border,		"ui.border",		"",
		desc,	boost::program_options::value<std::string>()->default_value( "color:FFFFFF;inactive:404040" ) )( ( id2gr( po_ui_border, pgUI ), 0 ) ).
		//[ui.padding.*]
		add_option( po_ui_padding,		"ui.padding",		"",
		desc,	boost::program_options::value<std::string>()->default_value( "size:1" ) )( ( id2gr( po_ui_padding, pgUI ), 0 ) ).
		//[ui.scroll.*]
		add_option( po_ui_scroll,		"ui.scroll",		"",
		desc,	boost::program_options::value<std::string>()->default_value( "size:2;color:008000" ) )( ( id2gr( po_ui_scroll, pgUI ), 0 ) );
	///
	///
	lang_t const en = boost::make_tuple( _T("eng"), _T("en"), _T("English"), AD_PANEL_IMAGE_LANG_EN );
	lang_t const ua = boost::make_tuple( _T("ukr"), _T("uk"), _T("����������"), AD_PANEL_IMAGE_LANG_UA );
	lang_t const ru = boost::make_tuple( _T("rus"), _T("ru"), _T("�������"), AD_PANEL_IMAGE_LANG_RU );

	this->langs.push_back( en );
	this->langs.push_back( ua );
	this->langs.push_back( ru );
	//
	this->langSetPair( std::make_pair( en, ru ) );
}

pref::~pref() {
}

bool pref::init( int argc, char const * const argv[] ) {
	atom::po::options_description_t& desc = this->po.get_desc( 0 );
	try {
		this->po.parse_arg( argc, argv, desc, true );
		//
		if ( po.count( po_help ) )
			throw std::exception( "cons command line parameters are:" );
		// load preferences
		// merge preferences with command line parameters
		//
	} catch( std::exception& exc ) {
		std::stringstream ss;
		desc.print( ss );
		//????????
		*(this->getLogger()) << boost::lexical_cast<atom::string_t>( exc.what() ) << std::endl;
		*(this->getLogger()) << boost::lexical_cast<atom::string_t>(ss.str()) << std::endl;
		return false;
	}
	return true;
}

void pref::register_process_callback( pref_group_t const g, callback_t c ){
	this->processCallbacks[ g ] = std::make_pair( false, c );
}

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

bool pref::parse( atom::string_t const& s ) {
	atom::po npo = this->po;
	//
	atom::po::options_description_t& desc = npo.get_desc( 0 );
	try {

		BOOST_FOREACH(processCallbacksMap::value_type& pair, this->processCallbacks ) {
			pair.second.first = false;
		}

		typedef std::map<std::string, boost::program_options::variable_value>
			map_t;
		npo.parse_cmd_line( s, desc, true );
		//
		atom::po::variables_map_t& vm = this->po.get_vm();
		atom::po::variables_map_t& nvm = npo.get_vm();
		//
		map_t::iterator it = nvm.begin();
		//
		BOOST_FOREACH(map_t::value_type& pair, vm )
		{
			if ( pair.second.value() != (*it).second.value() ) {
				(*(this->processCallbacks.find( (*(this->prefGroups.find( this->po.get_id( pair.first ) ))).second ))).second.first = true;
			}
			//
			++it;
		}
		//
		this->po = npo;
		//
		BOOST_FOREACH(processCallbacksMap::value_type& pair, this->processCallbacks ) {
			if ( pair.second.first ) {
				pair.second.second();
			}
		}

	} catch( std::exception& e ) {
		*(this->getLogger()) << e.what() << std::endl;
		return false;
	}
	return true;
}

void pref::getView( RECT& r ) {
	SetRect( &r, 0, 0, GetSystemMetrics( SM_CXSCREEN ), GetSystemMetrics( SM_CYSCREEN ) );
	if ( this->get< bool >( po_ui_clip ) ) {
		SystemParametersInfo( SPI_GETWORKAREA, 0, &r, 0 );
	}
}


void pref::calculateDocks( placement& windowPlacement ) {
	//
	static atom::parse_tag< atom::char_t, alignment::type > alignment_tags[] = {
		{ _T( "hcenter" ),		alignment::hcenter },
		{ _T( "left" ),			alignment::left },
		{ _T( "right" ),		alignment::right },
		{ _T( "hclient" ),		alignment::hclient },
		{ _T( "hmask" ),		alignment::hmask },
		{ _T( "vcenter" ),		alignment::vcenter },
		{ _T( "top" ),			alignment::top },
		{ _T( "bottom" ),		alignment::bottom },
		{ _T( "vclient" ),		alignment::vclient },
		{ _T( "vmask" ),		alignment::vmask },
		{ _T( "client" ),		alignment::client },
		{ _T( "center" ),		alignment::center }
	};
	static size_t alignment_tags_count = sizeof( alignment_tags ) / sizeof( alignment_tags[0] );
	//
	atom::string_t const alig_str	= boost::lexical_cast<atom::string_t>( this->get< atom::po::string_t >( po_ui_alignment ) );

	unsigned int const width	= ( windowPlacement.fullScreen )?( 100 ):( this->get< unsigned int >( po_ui_width ) );
	unsigned int const height	= ( windowPlacement.fullScreen )?( 100 ):( this->get< unsigned int >( po_ui_height ) );

	windowPlacement.timeout = this->get< unsigned int>( po_ui_timeout );
	//
	alignment::type align = alignment::client;
	atom::parse_result< atom::char_t, alignment::type > result = atom::parse_tags( alig_str, alignment_tags, alignment_tags_count, atom::string_t( _T( "+" ) ) );
	//
	if ( ( result.total_found > 0 ) && ( result.unparsed.size() == 0 ) ) {
		align = result.result;
	} else {
		*(this->getLogger()) << "Invalid alignment format " << alig_str << std::endl;
		align = alignment::hcenter | alignment::top;
	}
	//

	RECT rt;
	this->getView( rt );
	//
	alignment::type const h_align = align & alignment::hmask;
	alignment::type const v_align = align & alignment::vmask;
	//
	int const rt_w = RECT_WIDTH( rt );
	int const rt_h = RECT_HEIGHT( rt );
	int const wnd_rt_w = ((h_align == alignment::hclient )?(rt_w):(rt_w * width / 100));
	int const wnd_rt_h = ((v_align == alignment::vclient )?(rt_h):(rt_h * height / 100));
	//
	RECT rrt;
	SetRect( &rrt, 0, 0, wnd_rt_w, wnd_rt_h );
	OffsetRect( &rrt, rt.left, rt.top );
	//
	LONG offset_x = 0;
	LONG offset_y = 0;
	LONG offset_x_inv = 0;
	LONG offset_y_inv = 0;
	if ( h_align == alignment::hcenter ) {
		offset_x = ( rt_w - RECT_WIDTH( rrt ) ) / 2;
	} else if ( h_align == alignment::left ) {
		offset_x_inv = -RECT_WIDTH( rrt );
	} else if ( h_align == alignment::right ) {
		offset_x = ( rt_w - RECT_WIDTH( rrt ) );
		offset_x_inv = RECT_WIDTH( rrt );
	}
	if ( v_align == alignment::vcenter ) {
		offset_y = ( rt_h - RECT_HEIGHT( rrt ) ) / 2;
	} else if ( v_align == alignment::top ) {
		offset_y_inv = -RECT_HEIGHT( rrt );
	} else if ( v_align == alignment::bottom ) {
		offset_y = ( rt_h - RECT_HEIGHT( rrt ) );
		offset_y_inv = RECT_HEIGHT( rrt );
	}
	OffsetRect( &rrt, offset_x, offset_y );
	//
	windowPlacement.alpha = this->get< unsigned int >( po_ui_alpha );
	if ( !windowPlacement.visible ) {
		windowPlacement.alpha = 0;
		OffsetRect( &rrt, offset_x_inv, offset_y_inv );
	}
	//
	windowPlacement.destination = rrt;
}

bool pref::parseHotkey( atom::po::id_t const id, hotkey& hk ) {
	static atom::parse_tag< atom::char_t, UINT > const hotkey_tags[] = {
		{ _T( "win" ),		MOD_WIN },
		{ _T( "ctrl" ),		MOD_CONTROL },
		{ _T( "alt" ),		MOD_ALT },
		{ _T( "shift" ),	MOD_SHIFT }
	};
	static size_t const hotkey_tags_count = sizeof( hotkey_tags ) / sizeof( hotkey_tags[0] );
	//
	atom::parse_result< atom::char_t, UINT > result = atom::parse_tags( boost::lexical_cast<atom::string_t>( this->get< atom::po::string_t >( id ) ), hotkey_tags, hotkey_tags_count, atom::string_t( _T( "+" ) ) );
	if ( ( result.total_found > 1 ) && ( result.unparsed.size() == 1 ) ) {
		hk.mods = MOD_NOREPEAT | result.result;
		try {
			atom::stringstream_t ss;
			ss << result.unparsed[0];
			ss >> hk.vk;
			if ( !hk.vk ) {
				atom::stringstream_t ss;
				ss << result.unparsed[0];
				ss >> std::hex >> hk.vk;
			}
			return ( hk.vk > 0 );
		}
		catch( std::exception& e ){
			*(this->getLogger()) << "Invalid hotkey format " << e.what()<< std::endl;
		}
	}

	return false;
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
