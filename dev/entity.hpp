#pragma once

namespace dev {

	class entity :
		public atom::node < LOKI_TYPELIST_3(entity2logger, entity2entity, entity2entities) >,
		public boost::enable_shared_from_this< entity > {

		typedef atom::node < LOKI_TYPELIST_3(entity2logger, entity2entity, entity2entities) >
			base_node_t;

	public:
		///
		static boost::filesystem::path const
			CONFIG_LOCATION;
		static std::string const
			ENTITY;
		static std::string const
			ID;
		static std::string const
			ATTR;
		///
		static entity_ptr create(logger_ptr l, entity_ptr p, boost::filesystem::path const& h, std::string const& i, boost::property_tree::ptree const& a) {
			return entity_ptr(new entity(l, p, h, i, a));
		}
		///
		~entity();
		///
		std::string const& getId() const { return this->id; }
		///
		void echo(std::ostream& os, std::string const& offset);
		///
		entity_ptr build(std::string const& identity, boost::property_tree::ptree const& attributes);
		///
		void build(boost::property_tree::ptree const& config);
		///
		void build();
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