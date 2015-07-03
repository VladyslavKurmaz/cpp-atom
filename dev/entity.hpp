#pragma once

namespace dev {

	class entity :
		public atom::node < LOKI_TYPELIST_3(entity2logger, entity2entity, entity2entities) >,
		public boost::enable_shared_from_this < entity > {

		typedef atom::node < LOKI_TYPELIST_3(entity2logger, entity2entity, entity2entities) >
			base_node_t;

	public:
		///
		static boost::filesystem::path const CONFIG_LOCATION;
		static std::string const ENTITY;
		static std::string const ID;
		static std::string const ATTR;
		static std::string const DEPENDS;
		static std::string const INHERITS;
		///
		static entity_ptr create(logger_ptr l, entity_ptr p, boost::filesystem::path const& h, std::string const& i, boost::property_tree::ptree const& a) {
			return entity_ptr(new entity(l, p, h, i, a));
		}
		///
		~entity();
		///
		std::string const& getId() const { return this->id; }
		///
		entity_ptr getParent(){ return ((get_value(boost::mpl::identity< entity2entity >()).item())); }
		///
		bool hasParent(){ return ((get_value(boost::mpl::identity< entity2entity >()).item().operator bool())); }
		///
		bool hasChildren(){ return ((get_value(boost::mpl::identity< entity2entities >()).size() > 0)); }
		//
		entity_ptr getRoot();
		///
		void getAbsolutePath(std::string& path);
		///
		void echo(std::ostream& os, std::string const& offset);
		///
		entity_ptr findChild(std::string const& childId);
		///
		entity_ptr find(std::string const& path);
		///
		entity_ptr find(size_t const offset, strings_t const& path);
		///
		entity_ptr build(std::string const& identity, boost::property_tree::ptree const& attributes);
		///
		void build(boost::property_tree::ptree const& config);
		///
		void build();
		///
		void buildDependsOnList(std::string const& list, entities_t& entities);
		///
		void buildDependsOnList(entities_t& entities);
		///
		void clear();

	protected:
		LOGGER_ACCESSOR(entity2logger);
		///
		entity(logger_ptr l, entity_ptr p, boost::filesystem::path const& h, std::string const& i, boost::property_tree::ptree const& a);
		///
		boost::filesystem::path getHome() const { return this->home; };
		///
		void mergeAttr(boost::property_tree::ptree const& a);

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