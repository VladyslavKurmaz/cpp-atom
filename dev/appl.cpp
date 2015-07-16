#include "./pch.hpp"
#include "./logger.hpp"
#include "./comp.hpp"
#include "./appl.hpp"

// TODO
// substitute for_each with for_each2
namespace dev {

	appl::appl(logger_ptr l) :
		po()
		, platform()
		, cursor()
		/*
		po()
		, msbuild()*/ {
			atom::mount<appl2logger>(this, l);
			//
			char const* root = getenv(CONST_DEV_HOME_ENV);
			std::string home;
			if (root) {
				home = std::string(root);
			}
			//
#ifdef _WIN32
			SYSTEM_INFO si = { 0 };
			GetSystemInfo(&si);
			std::stringstream ss;
			ss << "i" << si.dwProcessorType;
			//
			BOOL bIsWow64 = FALSE;
#ifndef _WIN64
			typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
			LPFN_ISWOW64PROCESS fnIsWow64Process;
			fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
				GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

			if (NULL != fnIsWow64Process) {
				if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64)) {
					//handle error
				}
			}
#endif
			//
			this->platform.operatingSystem	= "windows";
			this->platform.instractionSet = ss.str();
			this->platform.memoryModel = (bIsWow64) ? ("32") : ("64");
			this->platform.toolset = "msvc-12.0";
			this->platform.configuration = "debug";
#endif
			//
			atom::po::options_description_t& initial_desc = this->po.add_desc(po_initial_desc, "");
			this->po.
				add_option(po_help, "help,h", "show (h)elp", initial_desc).
				add_option(po_shell, "shell,s", "shell mode", initial_desc).
				add_option(po_home, "home,o", "define h(o)me directory, override %DEV_HOME% environment variable", initial_desc, boost::program_options::value<std::string>()->default_value(home));
			//
			atom::po::options_description_t& conf_desc = this->po.add_desc(po_conf_desc, "");
			this->po.
				add_option(po_init_comp, "comp,e", "define current (e)nvironment", conf_desc, boost::program_options::value<std::string>()->default_value(CONST_ROOT_SIMBOL)).
				add_option(po_recursive, "recursive,r", "(r)ecursive mode", conf_desc).
				add_option(po_user, "user,u", "(u)ser", conf_desc, boost::program_options::value<std::string>()).
				add_option(po_password, "password,p", "(p)assword", conf_desc, boost::program_options::value<std::string>()).
				add_option(po_email, "email,m", "e(m)ail", conf_desc, boost::program_options::value<std::string>()).
				add_option(po_origin, "ori(g)in,g", "repository ori(g)in", conf_desc, boost::program_options::value<std::string>()).
				add_option(po_branch, "branc(h),h", "branc(h)", conf_desc, boost::program_options::value<std::string>()).
				add_option(po_osystem, "operating-system,y", "operating s(y)stem", conf_desc, boost::program_options::value<std::string>(&this->platform.operatingSystem)->default_value(this->platform.operatingSystem)).
				add_option(po_instruction_set, "instruction-set,n", "i(n)struction set", conf_desc, boost::program_options::value<std::string>(&this->platform.instractionSet)->default_value(this->platform.instractionSet)).
				add_option(po_address_model, "address-model,a", "(a)ddress-model", conf_desc, boost::program_options::value<std::string>(&this->platform.memoryModel)->default_value(this->platform.memoryModel)).
				add_option(po_toolset, "toolset,t", "build (t)oolset", conf_desc, boost::program_options::value<std::string>(&this->platform.toolset)->default_value(this->platform.toolset)).
				add_option(po_configuration, "configuration,c", "(c)onfiguration", conf_desc, boost::program_options::value<std::string>(&this->platform.configuration)->default_value(this->platform.configuration));
			// environment-type
			//
			atom::po::options_description_t& subcommands_desc = this->po.add_desc(po_subcommands_desc, "");
			this->po.
				add_option(po_subcommand1, "sc1", "<stage>    | cd  |help|ls|exit", subcommands_desc, boost::program_options::value<std::string>()->default_value("")).
				add_option(po_subcommand2, "sc2", "<component>|<ent>|    |  |", subcommands_desc, boost::program_options::value<std::string>()->default_value(""));

			atom::po::positional_options_description_t& subcommands_posdesc = this->po.add_pdesc(po_subcommands_posdesc, "");
			this->po.
				add_option(po_subcommand1, "sc1", "", subcommands_posdesc, 1).
				add_option(po_subcommand2, "sc2", "", subcommands_posdesc, 2);
			//
			atom::po::options_description_t& cmdline_desc = this->po.add_desc(po_cmdline_desc, "");
			cmdline_desc.add(initial_desc).add(conf_desc).add(subcommands_desc);
			//
			atom::po::options_description_t& shell_desc = this->po.add_desc(po_shell_desc, "");
			shell_desc.add(conf_desc).add(subcommands_desc);
	}

	appl::~appl() {
	}

	bool appl::init(int argc, char const * const argv[], std::ostream& os) {
		//
		atom::po::options_description_t& desc = this->po.get_desc(po_cmdline_desc);
		atom::po::positional_options_description_t& pdesc = this->po.get_pdesc(po_subcommands_posdesc);
		try {
			this->po.parse_arg(argc, argv, desc, pdesc, true);
			//
			if (!this->po.as< std::string >(po_home).length()) {
				throw std::exception(make_crit_msg("Dev home wasn't defined, set environment variable DEV_HOME or use command line argument --home"));
			}
			//
			this->cursor = comp::create(this->getLogger(), dev::comp_ptr(), this->po.as<std::string>(po_home), CONST_ROOT_SIMBOL, boost::property_tree::ptree());
			atom::mount<appl2comp>(this, cursor);
			// ... manually create .conf repo DEV_CATALOG and attach it to the dev root
			// 1. get param from command line
			// if .conf already exists - nothing to do
			// if it doesn't - add ".git"."url|user|branch" property
			//
			// ????////
			// find --comp and set it as cursor
			cursor->build();
			this->processCommand(os);
		}
		catch (std::exception& exc) {
			this->printError(desc, exc);
			std::cin.ignore();
			return false;
		}
		return true;
	}

	void appl::run(std::ostream& os, std::istream& is) {
		atom::po::options_description_t& shell_desc = this->po.get_desc(po_shell_desc);
		atom::po::positional_options_description_t& subcommands_posdesc = this->po.get_pdesc(po_subcommands_posdesc);
		while (true) {
			std::string s, mname;
			this->cursor->getMnemonicName(mname);
			os << mname << "> ";
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
		desc.print(ss);
		*(this->getLogger()) << boost::lexical_cast<dev::string_t>(std::string(exc.what())) << std::endl;
		*(this->getLogger()) << boost::lexical_cast<dev::string_t>(ss.str()) << std::endl;
	}

	bool appl::processCommand(std::ostream& os) {
		std::string const pos1 = this->po.as< std::string >(po_subcommand1);
		std::string const pos2 = this->po.as< std::string >(po_subcommand2);
		bool const recursive = ( this->po.count(po_recursive) > 0 );
		//
		if (pos1 == CONST_CMD_HELP) {
			throw std::exception("command line parameters:");
		}
		else if (pos1 == CONST_CMD_CHANGE_COMP) {
			comp_ptr nc;
			if (nc = this->cursor->find(pos2)){
				this->cursor = nc;
			}
		}
		else if (pos1 == CONST_CMD_LIST) {
			this->cursor->echo(os, "");
		}
		else if (pos1 == CONST_CMD_EXIT) {
			return false;
		}
		else if (pos1.length()){
			if (pos2.length()){
				std::string p1 = pos1;// "make";
				std::string p2 = pos2;// "cpp-atom.agd";
				//
				// split commands
				strings_t stages;
				boost::split(stages, p1, boost::is_any_of(CONST_CMD_DELIMITER));
				//
				// split components and resolve names
				strings_t cmpns;
				boost::split(cmpns, p2, boost::is_any_of(CONST_CMD_DELIMITER));
				comps_t components;
				BOOST_FOREACH(std::string const& cm, cmpns) {
					comp_ptr c = this->cursor->findChild(cm);
					if (c){
						components.push_back(c);
					}
					else{
						throw std::exception("component doesn't find");
					}
				}
				//
				// execute commands
				BOOST_FOREACH(std::string const& stage, stages) {
					BOOST_FOREACH(comp_ptr cp, components) {
						cp->execute(this->platform, stage, recursive);
					}
				}
			}
			else {
				// reposotory with catalogs
			}
		}
		return true;
	}
}