#pragma once

namespace dev {

	class context :
		public atom::node< LOKI_TYPELIST_1(context2logger) >,
		public boost::enable_shared_from_this < context > {

		typedef atom::node < LOKI_TYPELIST_1(context2logger) >
			base_node_t;

	public:
		///
		static context_ptr create(logger_ptr l, platform_t const& platform) {
			context_ptr result = context_ptr(new context(l, platform));
			return result;
		}
		///
		~context();
		///
		void clear();
		///
		void addEnvVar(std::string const& variable, std::string const& varType, std::string const& value, std::string const& valType);
		///
		void addScriptLines(std::string const& lines);
		///
		void resolveScript(comp_ptr cm);
		///
		std::string getScriptText() { return this->script.str();  }

	protected:
		//
		enum envVarType{
			vtUndefined = 0,
			vtGenerate,
			vtUse,
			vtExtend
		};
		///
		LOGGER_ACCESSOR(context2logger);
		//
		std::string normalizeEnvName(std::string const& name);
		//
		envVarType parseVarType(std::string const& name);
	private:
		struct envVar_t{
			std::string variable;
			envVarType	varType;
			std::string value;
			envVarType	valType;
		};
		typedef std::vector < envVar_t >
			envVars_t;
		envVars_t
			envVars;
		///
		typedef std::vector < std::string >
			lines_t;
		lines_t
			lines;
		///
		std::stringstream
			script;

		///
		context(logger_ptr l, platform_t const& platform);
	};
}