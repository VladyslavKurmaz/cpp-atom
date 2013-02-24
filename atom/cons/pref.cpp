#include "./pch.hpp"
#include "./pref.hpp"

#if 0

default -> shared -> global -> user -> local 
*[global]-computer/dropbox, [user]-computer-user, [instance|local] dpm-console instance settings

profile.location.shared
profile.location.global
profile.location.user
profile.location.local


ui.alpha
ui.color.bk
ui.color.command.bk
ui.color.command.text
ui.color.command.font
ui.color.log.bk
ui.color.log.text
ui.color.log.font
ui.color.vertical.scroll
ui.color.horizontal.scroll

ui.margin
ui.border
ui.spacing

ui.alignment top|left|bottom|right
ui.full-screen true|false

hk.edit.exec VK_RETURN
hk.edit.ml.enter VK_CONTROL + VK_RETURN
hk.edit.ml.new-line VK_RETURN
hk.edit.ml.exec VK_CONTROL + VK_RETURN

#endif


pref::pref( logger::shared_ptr l ) : base_t(), po() {
	atom::mount<pref2logger>( this, l );
	atom::po::options_description_t& desc = this->po.add_desc( 0, "program options" );
	//
	po.
		add_option( po_help,					"help",											"Show this help", desc );
	/*
		//[hk.*]
		add_option( po_help,					"hk.appear",
		boost::program_options::value<std::string>()->default_value("win+`"),					"[win][ctrl][shift][alt]+[char|0xCODE]", desc ).
		add_option( po_help,					"hk.split",
		boost::program_options::value<std::string>()->default_value("alt+S"),					"", desc ).
		add_option( po_help,					"hk.maxmin",
		boost::program_options::value<std::string>()->default_value("alt+W"),					"", desc ).
		add_option( po_help,					"hk.rotate",
		boost::program_options::value<std::string>()->default_value("alt+R"),					"", desc ).
		add_option( po_help,					"hk.next",
		boost::program_options::value<std::string>()->default_value("ctrl+TAB"),				"", desc ).
		add_option( po_help,					"hk.prev",
		boost::program_options::value<std::string>()->default_value("ctrl+shift+TAB"),			"", desc ).
		add_option( po_help,					"hk.close",
		boost::program_options::value<std::string>()->default_value("ctrl+F4"),					"", desc ).
		//[ui.*]
		add_option( po_help,					"ui.alignment",
		boost::program_options::value<std::string>()->default_value("top"),						"[top|bottom]|[left|right]", desc ).
		add_option( po_help,					"ui.width",
		boost::program_options::value<unsigned int>()->default_value( 100 ),					"", desc ).
		add_option( po_help,					"ui.height",
		boost::program_options::value<unsigned int>()->default_value( 50 ),						"", desc ).
		add_option( po_help,					"ui.clip",
		boost::program_options::value<bool>()->default_value( true ),							"", desc ).


		//[ui.margin.*]
		//[ui.border.*]
		//[ui.padding.*]

		//[ui.font.*]
		add_option( po_help,					"ui.font.name",
		boost::program_options::value<std::string>()->default_value("Lucida Console"),			"", desc ).
		add_option( po_help,					"ui.font.color",
		boost::program_options::value<std::string>()->default_value("00FF00"),					"", desc );
																														/*add_option( po_host,					"host",
																														boost::program_options::value<std::string>()->default_value("172.22.90.101"),		"Map service", desc ).
																														add_option( po_wifi_scan,				"scan",
																														boost::program_options::value<int>()->default_value(1000),							"Wifi scan timeout (<=0 disable)", desc ).
																														add_option( po_gui,						"gui",
																														boost::program_options::value<std::string>()->default_value(""),					"Display GUI: '' | '2d' | '3d'", desc ).
																														add_option( po_gui_width,				"width",
																														boost::program_options::value<unsigned int>()->default_value(800),					"GUI window width", desc ).
																														add_option( po_gui_height,				"height",
																														boost::program_options::value<unsigned int>()->default_value(600),					"GUI window height", desc ).
																														add_option( po_move,					"move",											"Move object", desc )*/;

	this->get_logger() << "create pref" << std::endl;
	std::stringstream ss;
	desc.print( ss );
	this->get_logger() << ss.str() << std::endl;
}

pref::~pref() {
	this->get_logger() << "free pref" << std::endl;
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


