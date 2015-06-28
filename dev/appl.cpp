#include "./pch.hpp"
#include "./logger.hpp"
#include "./entity.hpp"
#include "./appl.hpp"

// TODO
// substitute for_each with for_each2
namespace dev {

	appl::appl( logger_ptr l ) : 
		po()
	,	cursor()
		/*
		po()
	,	msbuild()*/ {
		atom::mount<appl2logger>( this, l );
		//
		char const* root = getenv(CONST_DEV_HOME_ENV);
		std::string home;
		if (root) {
			home = std::string(root);
		}
		char const* arch = getenv(CONST_PA_ENV);
		std::string am((arch != NULL) ? ((!strcmp(arch, CONST_PLATFORM_X86.c_str())) ? ("32") : ("64")) : (""));
		//
		atom::po::options_description_t& initial_desc = this->po.add_desc( po_initial_desc, "" );
		this->po.
			add_option(po_help,				"help,h", "show (h)elp", initial_desc).
			add_option(po_shell,			"shell,s", "shell mode", initial_desc).
			add_option(po_home,				"home,o", "define h(o)me directory, override %DEV_HOME% environment variable", initial_desc, boost::program_options::value<std::string>()->default_value(home));
		//
		atom::po::options_description_t& conf_desc = this->po.add_desc( po_conf_desc, "" );
		this->po.
			add_option(po_init_entity,		"entity,e",				"define current (e)nvironment", conf_desc, boost::program_options::value<std::string>()->default_value(CONST_ROOT_SIMBOL)).
			add_option(po_recursive,		"recursive,r",			"(r)ecursive mode", conf_desc).
			add_option(po_user,				"user,u",				"(u)ser", conf_desc, boost::program_options::value<std::string>()).
			add_option(po_password,			"password,p",			"(p)assword", conf_desc, boost::program_options::value<std::string>()).
			add_option(po_email,			"email,m",				"e(m)ail", conf_desc, boost::program_options::value<std::string>()).
			add_option(po_origin,			"ori(g)in,g",			"repository ori(g)in", conf_desc, boost::program_options::value<std::string>()).
			add_option(po_branch,			"branc(h),h",			"branc(h)", conf_desc, boost::program_options::value<std::string>()).
			add_option(po_osystem,			"operating-system,y",	"operating s(y)stem", conf_desc, boost::program_options::value<std::string>()->default_value(CONST_DEF_OS)).
			add_option(po_instruction_set,	"instruction-set,n",	"i(n)struction set", conf_desc, boost::program_options::value<std::string>()->default_value(CONST_DEF_IS)).
			add_option(po_address_model,	"address-model,a",		"(a)ddress-model", conf_desc, boost::program_options::value<std::string>()->default_value(am)).
			add_option(po_toolset,			"toolset,t",			"build (t)oolset", conf_desc, boost::program_options::value<std::string>()->default_value(CONST_DEF_TOOLSET)).
			add_option(po_configuration,	"configuration,c",		"(c)onfiguration", conf_desc, boost::program_options::value<std::string>()->default_value(CONST_DEF_CONFIGURATION));
		// environment-type
		//
		atom::po::options_description_t& subcommands_desc = this->po.add_desc( po_subcommands_desc, "" );
		this->po.
			add_option( po_subcommand1,		"sc1",				"<stage>    | cd  |help|ls|exit", subcommands_desc, boost::program_options::value<std::string>()->default_value( "" ) ).
			add_option( po_subcommand2,		"sc2",				"<component>|<ent>|    |  |", subcommands_desc, boost::program_options::value<std::string>()->default_value( "" ) );

		atom::po::positional_options_description_t& subcommands_posdesc = this->po.add_pdesc( po_subcommands_posdesc, "" );
		this->po.
			add_option( po_subcommand1,		"sc1",				"", subcommands_posdesc, 1 ).
			add_option( po_subcommand2,		"sc2",				"", subcommands_posdesc, 2 );
		//
		atom::po::options_description_t& cmdline_desc = this->po.add_desc( po_cmdline_desc, "" );
		cmdline_desc.add(initial_desc).add(conf_desc).add(subcommands_desc);
		//
		atom::po::options_description_t& shell_desc = this->po.add_desc( po_shell_desc, "" );
		shell_desc.add( conf_desc ).add( subcommands_desc );
}

appl::~appl() {
}

bool appl::init(int argc, char const * const argv[], std::ostream& os) {
	//
	atom::po::options_description_t& desc = this->po.get_desc( po_cmdline_desc );
	atom::po::positional_options_description_t& pdesc = this->po.get_pdesc( po_subcommands_posdesc );
	try {
		this->po.parse_arg( argc, argv, desc, pdesc, true );
		//
		if ( !this->po.as< std::string >( po_home ).length() ) {
			throw std::exception( make_crit_msg( "Dev home wasn't defined, set environment variable DEV_HOME or use command line argument --home" ) );
		}
		//
		this->cursor = entity::create(this->getLogger(), dev::entity_ptr(), this->po.as<std::string>(po_home), CONST_ROOT_SIMBOL, boost::property_tree::ptree());
		atom::mount<appl2entity>(this, cursor);
		// ... manually create .conf repo DEV_CATALOG and attach it to the dev root
		// 1. get param from command line
		// if .conf already exists - nothing to do
		// if it doesn't - add ".git"."url|user|branch" property
		//
		// ????////
		// find --entity and set it as cursor
		cursor->build();
		this->processCommand(os);
	}
	catch (std::exception& exc) {
		this->printError( desc, exc );
		std::cin.ignore();
		return false;
	}
	return true;
}

void appl::run(std::ostream& os, std::istream& is) {
	atom::po::options_description_t& shell_desc = this->po.get_desc(po_shell_desc);
	atom::po::positional_options_description_t& subcommands_posdesc = this->po.get_pdesc(po_subcommands_posdesc);
	while (true) {
		std::string s, cpath;
		this->cursor->getAbsolutePath(cpath);
		os <<  cpath << "> ";
		std::getline(is, s);
		//
		try {
			this->po.parse_cmd_line(s, shell_desc, subcommands_posdesc, true);
			if (!this->processCommand(os)) {
				break;
			}
		}
		catch (std::exception& exc) {
			this->printError(shell_desc, exc);
		}
	}
}

void appl::clear(){
	this->cursor.reset();
	this->getRoot()->clear();
	base_node_t::clear();
}


void appl::printError(atom::po::options_description_t const& desc, std::exception& exc) {
	std::stringstream ss;
	desc.print( ss );
	*(this->getLogger()) << boost::lexical_cast<dev::string_t>(std::string(exc.what())) << std::endl;
	*(this->getLogger()) << boost::lexical_cast<dev::string_t>(ss.str()) << std::endl;
}

bool appl::processCommand(std::ostream& os) {
	std::string const pos1 = this->po.as< std::string >(po_subcommand1);
	std::string const pos2 = this->po.as< std::string >(po_subcommand2);
	//
	if (pos1 == CONST_CMD_HELP) {
		throw std::exception("command line parameters:");
	} else if (pos1 == CONST_CMD_CHANGE_ENTITY) {
		path_t path;
		size_t offset = 0;
		boost::split(path, pos2, boost::is_any_of(CONST_ROOT_SIMBOL));
		if (path.size()){
			entity_ptr nc;
			// relative path
			entity_ptr e = this->cursor;
			if (!path[0].length()){
				// absolute path
				e = this->getRoot();
				offset++;
			}
			if (nc = e->find(offset, path)){
				this->cursor = nc;
			}
		}
	} else if (pos1 == CONST_CMD_LIST) {
		this->cursor->echo(os, "");
	} else if (pos1 == CONST_CMD_EXIT) {
		return false;
	} else if (pos1.length()){
		if (pos2.length()){
			// entities
		} else {
			// reposotory with catalogs
		}
	}
	return true;
}


#if 0
bool appl::process_command() {


	
	context_ptr cont = context::create( this->get_logger() );
		//
		//
		*(this->get_logger()) << std::endl;
		return true;
}

#endif
}