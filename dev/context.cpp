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

	void context::addEnvVar(std::string const& variable, std::string const& varType, std::string const& value, std::string const& valType){
		envVar_t v;
		v.variable = variable;
		v.varType = parseVarType(varType);
		v.value = value;
		v.valType = parseVarType(valType);
		this->envVars.push_back(v);
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
				std::string name = v.variable;
				if (v.varType == vtGenerate){
					name = normalizeEnvName(/*"DEV" + CONST_CMD_UNDERSCORE + */cm->getQualifiedId() + CONST_CMD_UNDERSCORE + name);
				}
				os << name;
				os << "=";
				std::string value = v.value;
				if (v.valType == vtGenerate){
					boost::filesystem::path p = cm->getHome();
					p /= boost::filesystem::path(value);
					value = p.string();
				}
				os << value;
				if (v.varType == vtExtend){
					os << ";%" << name << "%";
				}
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

	context::envVarType context::parseVarType(std::string const& name){
		///
		static struct {
			char*		id;
			envVarType	type;
		} const types[] = {
			{ "gen", context::vtGenerate },
			{ "use", context::vtUse },
			{ "ext", context::vtExtend }
		};
		size_t const typesCount = sizeof(types) / sizeof(types[0]);
		//
		envVarType r = vtUndefined;
		//
		for (size_t i = 0; i < typesCount; ++i){
			if (name == types[i].id){
				r = types[i].type;
				break;
			}
		}
		return r;
	}

}
