#pragma once

namespace dev {

	class comp :
		public atom::node < LOKI_TYPELIST_3(comp2logger, comp2comp, comp2comps) >,
		public boost::enable_shared_from_this < comp > {

		typedef atom::node < LOKI_TYPELIST_3(comp2logger, comp2comp, comp2comps) >
			base_node_t;

	public:
		///
		static boost::filesystem::path const CONFIG_FOLDER;
		static boost::filesystem::path const CONFIG_FILE_NAME;
		static std::string const INCLUDE;
		static std::string const ID;
		static std::string const COMP;
		static std::string const PREREQUISITES;
		static std::string const INHERITS;
		static std::string const ATTR;
		static std::string const VARS;
		static std::string const STAGES;
		///
		static comp_ptr create(logger_ptr l, comp_ptr p, boost::filesystem::path const& h, std::string const& i, boost::property_tree::ptree const& a) {
			return comp_ptr(new comp(l, p, h, i, a));
		}
		///
		~comp();
		///
		std::string const& getId() const { return this->id; }
		///
		boost::filesystem::path getHome() const { return this->home; };
		///
		comp_ptr getParent(){ return ((get_value(boost::mpl::identity< comp2comp >()).item())); }
		///
		bool hasParent(){ return ((get_value(boost::mpl::identity< comp2comp >()).item().get() != NULL)); }
		///
		bool hasChildren(){ return ((get_value(boost::mpl::identity< comp2comps >()).size() > 0)); }
		//
		comp_ptr getRootComponent();
		///
		std::string getQualifiedId();
		///
		void getMnemonicName(std::string& name);
		///
		void echo(std::ostream& os, std::string const& offset, std::string const& regex, bool const recursive);
		///
		comp_ptr findCompUp(std::string const& compId);
		///
		comp_ptr findChild(std::string const& childId);
		///
		comp_ptr find(std::string const& path);
		///
		comp_ptr find(size_t const offset, strings_t const& path);
		///
		comp_ptr build(std::string const& identity, boost::property_tree::ptree const& attributes);
		///
		void build(boost::property_tree::ptree const& config);
		///
		void build(boost::filesystem::path const& fileName);
		///
		void build();
		///
		void buildPrerequisitesList(platform_t const& platform, comps_t& comps);
		///
		void buildInheritsFromList(platform_t const& platform, comps_t& comps);
		///
		void execute(platform_t const& platform, std::string const& stage, bool const recursive, bool const idle);
		///
		void clear();

	protected:
		typedef unsigned int script_mask_t;
		static script_mask_t const SCRIPT_MASK_ENVIRONMENT = 0x01;
		static script_mask_t const SCRIPT_MASK_PACKAGES = 0x02;
		static script_mask_t const SCRIPT_MASK_STAGES = 0x04;
		static script_mask_t const SCRIPT_MASK_ALL = SCRIPT_MASK_ENVIRONMENT | SCRIPT_MASK_PACKAGES | SCRIPT_MASK_STAGES;
		///
		LOGGER_ACCESSOR(comp2logger);
		///
		comp(logger_ptr l, comp_ptr p, boost::filesystem::path const& h, std::string const& i, boost::property_tree::ptree const& a);
		///
		void mergeAttr(boost::property_tree::ptree const& a);
		///
		bool checkPlatform(platform_t const& platform) const;
		///
		static void linearizeHierarchy(platform_t const& platform, comp_ptr parent, std::string const& type, std::string const& list, comps_t& comps);
		///
		void linearizeHierarchy(platform_t const& platform, std::string const& type, comps_t& comps);
		///
		void constructScript(std::string const& stage, script_mask_t const mask, context_ptr cnxt) const;
		///
		void buildScript(platform_t const& platform, std::string const& stage, script_mask_t const mask, context_ptr cnxt);

	private:
		///
		boost::filesystem::path const
			home;
		///
		std::string
			id;
		///
		boost::property_tree::ptree
			attr;
	};
}