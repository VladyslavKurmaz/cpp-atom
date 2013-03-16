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

#endif


pref::pref( logger_ptr l ) : base_t(), po() {
	atom::mount<pref2logger>( this, l );
	atom::po::options_description_t& desc = this->po.add_desc( 0, "program options" );
	//
	po.
		add_option( po_help,					"help",											"Show this help", desc ).
		//[hk.*]
		add_option( po_hk_appear,				"hk.appear",
		boost::program_options::value<std::string>()->default_value("ctrl+0xC0"),				"[win+][ctrl+][alt+][shift+][VK]", desc ).
		add_option( po_hk_split,				"hk.split",
		boost::program_options::value<std::string>()->default_value("alt+S"),					"", desc ).
		add_option( po_hk_expand,				"hk.expand",
		boost::program_options::value<std::string>()->default_value("alt+W"),					"", desc ).
		add_option( po_hk_rotate,				"hk.rotate",
		boost::program_options::value<std::string>()->default_value("alt+R"),					"", desc ).
		add_option( po_hk_next,					"hk.next",
		boost::program_options::value<std::string>()->default_value("ctrl+TAB"),				"", desc ).
		add_option( po_hk_prev,					"hk.prev",
		boost::program_options::value<std::string>()->default_value("ctrl+shift+TAB"),			"", desc ).
		add_option( po_hk_close,				"hk.close",
		boost::program_options::value<std::string>()->default_value("ctrl+F4"),					"", desc ).
		add_option( po_hk_tty1,					"hk.tty1",
		boost::program_options::value<std::string>()->default_value("alt+ctrl+F1"),				"", desc ).
		add_option( po_hk_tty2,					"hk.tty2",
		boost::program_options::value<std::string>()->default_value("alt+ctrl+F2"),				"", desc ).
		add_option( po_hk_tty3,					"hk.tty3",
		boost::program_options::value<std::string>()->default_value("alt+ctrl+F3"),				"", desc ).
		add_option( po_hk_tty4,					"hk.tty4",
		boost::program_options::value<std::string>()->default_value("alt+ctrl+F4"),				"", desc ).
		add_option( po_hk_tty5,					"hk.tty5",
		boost::program_options::value<std::string>()->default_value("alt+ctrl+F5"),				"", desc ).
		add_option( po_hk_tty6,					"hk.tty6",
		boost::program_options::value<std::string>()->default_value("alt+ctrl+F6"),				"", desc ).
		//[ui.*]
		add_option( po_ui_timeout,				"ui.timeout",
		boost::program_options::value<unsigned int>()->default_value( 250 ),					"", desc ).
		add_option( po_ui_alignment,			"ui.alignment",
		boost::program_options::value<std::string>()->default_value("top"),						"top|bottom|left|right|center|vcenter|client", desc ).
		add_option( po_ui_width,				"ui.width",
		boost::program_options::value<unsigned int>()->default_value( 50 ),						"", desc ).
		add_option( po_ui_height,				"ui.height",
		boost::program_options::value<unsigned int>()->default_value( 50 ),						"", desc ).
		add_option( po_ui_clip,					"ui.clip",
		boost::program_options::value<bool>()->default_value( true ),							"", desc ).
		add_option( po_ui_alpha,				"ui.alpha",
		boost::program_options::value<unsigned int>()->default_value( 0xF0 ),					"", desc ).
		add_option( po_ui_bk_color,				"ui.bk.color",
		boost::program_options::value<unsigned int>()->default_value( 0x0F0F0F ),				"", desc ).
		//[ui.font.*]
		add_option( po_ui_font_name,			"ui.font.name",
		boost::program_options::value<std::string>()->default_value( "Consolas" ),				"", desc ).
		add_option( po_ui_font_height,			"ui.font.height",
		boost::program_options::value< unsigned int >()->default_value( 16 ),					"", desc ).
		add_option( po_ui_font_color,			"ui.font.color",
		boost::program_options::value< unsigned int >()->default_value( 0xFFFFFF ),				"", desc ).
		//[ui.margin.*]
		add_option( po_ui_margin_size,			"ui.margin.size",
		boost::program_options::value< unsigned int >()->default_value( 0 ),					"", desc ).
		//[ui.border.*]
		add_option( po_ui_border_size,			"ui.border.size",
		boost::program_options::value< unsigned int >()->default_value( 1 ),					"", desc ).
		add_option( po_ui_border_color,			"ui.border.color",
		boost::program_options::value< unsigned int >()->default_value( 0xFFFFFF ),				"", desc ).
		//[ui.padding.*]
		add_option( po_ui_padding_size,			"ui.padding.size",
		boost::program_options::value< unsigned int >()->default_value( 2 ),					"", desc ).
		//[ui.scroll.*]
		add_option( po_ui_scroll_size,			"ui.scroll.size",
		boost::program_options::value<unsigned int>()->default_value( 2 ),						"", desc ).
		add_option( po_ui_scroll_color,			"ui.scroll.color",
		boost::program_options::value<unsigned int>()->default_value( 0x00FF00 ),				"", desc );
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


