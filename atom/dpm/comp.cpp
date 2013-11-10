#include "./pch.hpp"
#include "./logger.hpp"
#include "./comp.hpp"
#include "./env.hpp"

comp::comp( logger_ptr l, appl_ptr a, boost::property_tree::ptree const& pt ) :
		props( pt )
{
	atom::mount<comp2logger>( this, l );
	atom::mount<comp2appl>( this, a );
			//boost::filesystem::path ncp = this->get_paths().get_env();
			//string_t id = child.second.get<string_t>("id");
			//ncp /= boost::filesystem::path( id );
}

comp::~comp() {
}

void
comp::clear() {
	base_node_t::clear();
}

void
comp::update() {
}

void
comp::print( logger_ptr l, string_t const& offs ) {
	*l << offs << this->get_id() << std::endl;
}

void
comp::execute( string_t const& cmd ) {
	// run msbuild
	stringstream_t ss;
	ss	<< "msbuild.exe";
		//<< " /p:component=\"" << pos1 << "\""
		//<< " /p:stage=\"" << pos2 << "\""
		//<< " /p:osystem=\"" << this->po.as< string_t >( po_osystem ) << "\""
		//<< " /p:instruction_set=\"" << this->po.as< string_t >( po_instruction_set ) << "\""
		//<< " /p:address_model=\"" << this->po.as< string_t >( po_address_model ) << "\""
		//<< " /p:configuration=\"" << this->po.as< string_t >( po_configuration ) << "\""
		//<< " /p:toolset=\"" << this->po.as< string_t >( po_toolset ) << "\""
		//<< " /p:recursive=" << (( this->po.count( po_recursive ) )?( "true" ):( "false" ))
		//<< std::endl;
	//
	*(this->get_logger()) << ss.str() << std::endl;
	atom::exec( ss.str(), this->get_env()->get_paths().get_dpm().string() );
}
