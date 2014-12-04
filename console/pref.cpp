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
	,	pref_groups()
	,	process_callbacks()
{
	atom::mount<pref2logger>( this, l );
	atom::po::options_description_t& desc = this->po.add_desc( 0, "program options" );
	//
	po.
		add_option( po_help,			"help",				"Show this help",							desc )( ( id2gr( po_help, pgEmpty ), 0 ) ).
		add_option( po_autostart,		"autostart",		"Start on Windows startup",					desc,	boost::program_options::value<bool>()->default_value( false ) )( ( id2gr( po_autostart, pgAutostart ), 0 ) ).
		//[hk.*]
		add_option( po_hk_appear,		"hk.appear",		"[win+][ctrl+][alt+][shift+]vk",			desc,	boost::program_options::value<std::string>()->default_value("ctrl+0xC0") )( ( id2gr( po_hk_appear, pgHotkeys ), 0 ) ).
		add_option( po_hk_split,		"hk.split",			"",											desc,	boost::program_options::value<std::string>()->default_value("ctrl+S") )( ( id2gr( po_hk_split, pgHotkeys ), 0 ) ).
		add_option( po_hk_expand,		"hk.expand",		"",											desc,	boost::program_options::value<std::string>()->default_value("ctrl+W") )( ( id2gr( po_hk_expand, pgHotkeys ), 0 ) ).
		add_option( po_hk_rotate,		"hk.rotate",		"",											desc,	boost::program_options::value<std::string>()->default_value("ctrl+A") )( ( id2gr( po_hk_rotate, pgHotkeys ), 0 ) ).
		add_option( po_hk_next,			"hk.next",			"",											desc,	boost::program_options::value<std::string>()->default_value("ctrl+tab") )( ( id2gr( po_hk_next, pgHotkeys ), 0 ) ).
		add_option( po_hk_prev,			"hk.prev",			"",											desc,	boost::program_options::value<std::string>()->default_value("ctrl+shift+tab") )( ( id2gr( po_hk_prev, pgHotkeys ), 0 ) ).
		add_option( po_hk_ctrl_break,	"hk.ctrl-break",	"",											desc,	boost::program_options::value<std::string>()->default_value("ctrl+break") )( ( id2gr( po_hk_ctrl_break, pgHotkeys ), 0 ) ).
		add_option( po_hk_ctrl_c,		"hk.ctrl-c",		"",											desc,	boost::program_options::value<std::string>()->default_value("ctrl+C") )( ( id2gr( po_hk_ctrl_c, pgHotkeys ), 0 ) ).
		add_option( po_hk_close,		"hk.close",			"",											desc,	boost::program_options::value<std::string>()->default_value("ctrl+f4") )( ( id2gr( po_hk_close, pgHotkeys ), 0 ) ).
		add_option( po_hk_tty1,			"hk.tty1",			"",											desc,	boost::program_options::value<std::string>()->default_value("alt+ctrl+f1") )( ( id2gr( po_hk_tty1, pgHotkeys ), 0 ) ).
		add_option( po_hk_tty2,			"hk.tty2",			"",											desc,	boost::program_options::value<std::string>()->default_value("alt+ctrl+f2") )( ( id2gr( po_hk_tty2, pgHotkeys ), 0 ) ).
		add_option( po_hk_tty3,			"hk.tty3",			"",											desc,	boost::program_options::value<std::string>()->default_value("alt+ctrl+f3") )( ( id2gr( po_hk_tty3, pgHotkeys ), 0 ) ).
		add_option( po_hk_tty4,			"hk.tty4",			"",											desc,	boost::program_options::value<std::string>()->default_value("alt+ctrl+f4") )( ( id2gr( po_hk_tty4, pgHotkeys ), 0 ) ).
		add_option( po_hk_tty5,			"hk.tty5",			"",											desc,	boost::program_options::value<std::string>()->default_value("alt+ctrl+f5") )( ( id2gr( po_hk_tty5, pgHotkeys ), 0 ) ).
		add_option( po_hk_tty6,			"hk.tty6",			"",											desc,	boost::program_options::value<std::string>()->default_value("alt+ctrl+f6") )( ( id2gr( po_hk_tty6, pgHotkeys ), 0 ) ).
		//[ui.*]
		add_option( po_ui_timeout,		"ui.timeout",		"",											desc,	boost::program_options::value<unsigned int>()->default_value( 250 ) )( ( id2gr( po_ui_timeout, pgEmpty ), 0 ) ).
		add_option( po_ui_alignment,	"ui.alignment",		"[left|right|hcenter|hclient+][top|bottom|vcenter|vclient+][client][center]",	desc,	boost::program_options::value<std::string>()->default_value("hclient+top") )( ( id2gr( po_ui_alignment, pgWindow ), 0 ) ).
		add_option( po_ui_width,		"ui.width",			"",											desc,	boost::program_options::value<unsigned int>()->default_value( 50 ))( ( id2gr( po_ui_width, pgWindow ), 0 ) ).
		add_option( po_ui_height,		"ui.height",		"",											desc,	boost::program_options::value<unsigned int>()->default_value( 50 ))( ( id2gr( po_ui_height, pgWindow ), 0 ) ).
		add_option( po_ui_clip,			"ui.clip",			"",											desc,	boost::program_options::value<bool>()->default_value( true ))( ( id2gr( po_ui_clip, pgWindow ), 0 ) ).
		add_option( po_ui_alpha,		"ui.alpha",			"",											desc,	boost::program_options::value<unsigned int>()->default_value( 0xF0 ))( ( id2gr( po_ui_alpha, pgUI ), 0 ) ).
		add_option( po_ui_bk_color,		"ui.bk-color",		"",											desc,	boost::program_options::value<unsigned int>()->default_value( 0x0F0F0F ) )( ( id2gr( po_ui_bk_color, pgUI ), 0 ) ).
		add_option( po_ui_lines_count,	"ui.lines-count",	"",											desc,	boost::program_options::value<unsigned int>()->default_value( 500 ) )( ( id2gr( po_ui_lines_count, pgUI ), 0 ) ).
		//[ui.font.*]
		add_option( po_ui_font_text,	"ui.font.text",		"",											desc,	boost::program_options::value<std::string>()->default_value( "name:Consolas;height:16;color:FFFFFF" ))( ( id2gr( po_ui_font_text, pgFonts ), 0 ) ).
		add_option( po_ui_font_sys,		"ui.font.sys",		"",											desc,	boost::program_options::value<std::string>()->default_value( "name:Consolas;height:12;color:008000" ) )( ( id2gr( po_ui_font_sys, pgFonts ), 0 ) ).
		//[ui.margin.*]
		add_option( po_ui_margin,		"ui.margin",		"",											desc,	boost::program_options::value<std::string>()->default_value( "size:0" ) )( ( id2gr( po_ui_margin, pgUI ), 0 ) ).
		//[ui.border.*]
		add_option( po_ui_border,		"ui.border",		"",											desc,	boost::program_options::value<std::string>()->default_value( "size:1;color:FFFFFF;inactive:404040" ) )( ( id2gr( po_ui_border, pgUI ), 0 ) ).
		//[ui.padding.*]
		add_option( po_ui_padding,		"ui.padding",		"",											desc,	boost::program_options::value<std::string>()->default_value( "size:1" ) )( ( id2gr( po_ui_padding, pgUI ), 0 ) ).
		//[ui.scroll.*]
		add_option( po_ui_scroll,		"ui.scroll",		"",											desc,	boost::program_options::value<std::string>()->default_value( "size:2;color:008000" ) )( ( id2gr( po_ui_scroll, pgUI ), 0 ) );
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
		this->get_logger() << exc.what() << std::endl;
		this->get_logger() << ss.str() << std::endl;
		return false;
	}
	return true;
}

void pref::register_process_callback( pref_group_t const g, callback_t c ){
	this->process_callbacks[ g ] = std::make_pair( false, c );
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

bool pref::parse( string_t const& s ) {
	atom::po npo = this->po;
	//
	atom::po::options_description_t& desc = npo.get_desc( 0 );
	try {

		BOOST_FOREACH(process_callbacks_t::value_type& pair, this->process_callbacks ) {
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
				(*(this->process_callbacks.find( (*(this->pref_groups.find( this->po.get_id( pair.first ) ))).second ))).second.first = true;
			}
			//
			++it;
		}
		//
		this->po = npo;
		//
		BOOST_FOREACH(process_callbacks_t::value_type& pair, this->process_callbacks ) {
			if ( pair.second.first ) {
				pair.second.second();
			}
		}

	} catch( std::exception& e ) {
		this->get_logger() << e.what() << std::endl;
		return false;
	}
	return true;
}

