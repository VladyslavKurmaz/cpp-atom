#include "./pch.hpp"
#include "./logger.hpp"
#include "./comp.hpp"
#include "./context.hpp"

namespace dev {

	context::context(logger_ptr l, platform_t const& platform) :
			envVars()
			, lines()
			, script() {
		atom::mount<context2logger>(this, l);
		//
		this->script
			<< "echo off" << std::endl
			<< "set OPERATING_SYSTEM=" << platform.operatingSystem << std::endl
			<< "set INSTRACTION_SET=" << platform.instractionSet << std::endl
			<< "set MEMORY_MODEL=" << platform.memoryModel << std::endl
			<< "set TOOLSET=" << platform.toolset << std::endl
			<< "set CONFIGURATION=" << platform.configuration << std::endl;
	}

	context::~context() {
	}

	void context::clear() {
		base_node_t::clear();
	}

	void context::addEnvVar(std::string const& name, std::string const& value){
		this->envVars.push_back(std::make_pair(name, value));
	}

	void context::addScriptLines(std::string const& lines){
		this->lines.push_back(lines);
	}
	
	void context::resolveScript(comp_ptr cm) {
		if (this->envVars.size() || this->lines.size()){
			std::ostream& os = this->script;
			std::string mname;
			cm->getMnemonicName(mname);
			//
			os << std::endl;
			os << "rem " << mname << std::endl;
			BOOST_FOREACH(envVar_t const& v, this->envVars) {
				os << "set ";
				std::string name = v.first;
				boost::replace_all(name, "{name}", normalizeEnvName("DEV2" + cm->getQualifiedId() + CONST_CMD_UNDERSCORE));
				//
				os << name;
				os << "=";
				std::string value = v.second;
				boost::replace_all(value, "{home}", cm->getHome().string());
				if (cm->hasParent()){
					boost::replace_all(value, "{parent-home}", cm->getParent()->getHome().string());
				}
				//
				os << value;
				os << std::endl;
			}
			this->envVars.clear();
			BOOST_FOREACH(std::string const& s, this->lines) {
				os << s << std::endl;
			}
		}
	}

	std::string context::normalizeEnvName(std::string const& name){
		std::string s = name;
		boost::replace_all(s, CONST_CMD_DOT, CONST_CMD_UNDERSCORE);
		boost::replace_all(s, CONST_CMD_DASH, CONST_CMD_UNDERSCORE);
		boost::to_upper(s);
		return s;
	}
}
