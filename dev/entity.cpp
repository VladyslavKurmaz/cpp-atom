#include "./pch.hpp"
#include "./logger.hpp"
#include "./entity.hpp"

namespace dev {

	boost::filesystem::path const	entity::CONFIG_LOCATION(boost::filesystem::path(".conf") / boost::filesystem::path("catalog.json"));
	std::string const				entity::ENTITY("entity");
	std::string const				entity::ID("id");
	std::string const				entity::ATTR("attr");

	entity::entity(logger_ptr l, entity_ptr p, boost::filesystem::path const& h, std::string const& i, boost::property_tree::ptree const& a) :
			home(h)
			, id(i)
			, attr(a) {
		atom::mount<entity2logger>(this, l);
		atom::mount<entity2entity>(this, p);
	}

	entity::~entity() {
	}

	void entity::echo(std::ostream& os, std::string const& offset){
		os << offset << "-" << this->getId() << std::endl;
		std::string off = offset + " |";
		//
		struct _{
			static void __(entity_ptr const& e, std::ostream& os, std::string const& o) {
				e->echo(os, o);
			}
		};
		this->get_slot<entity2entities>().for_each(boost::bind(&_::__, _1, boost::ref(os), boost::ref(off)));
	}

	entity_ptr entity::build(std::string const& identity, boost::property_tree::ptree const& attributes){
		struct _{
			static void __(entity_ptr const& e, std::string const& nid, entity_ptr& ne) {
				if (e->getId() == nid){
					ne = e;
				}
			}
		};
		entity_ptr e;
		this->get_slot<entity2entities>().for_each(boost::bind(&_::__, _1, boost::ref(identity), boost::ref(e)));
		if (e) {
			e->mergeAttr(attributes);
		}else{
			e = entity::create(this->getLogger(), this->shared_from_this(), this->getHome() / boost::filesystem::path(identity), identity, attributes);
			atom::mount<entity2entities>(this, e);
		}
		return (e);
	}

	void entity::build(boost::property_tree::ptree const& config){
		boost::property_tree::ptree::const_assoc_iterator it = config.find(ENTITY);
		if (it != config.not_found()) {
			BOOST_FOREACH(boost::property_tree::ptree::value_type const & c, config.get_child(ENTITY)) {
				boost::property_tree::ptree::const_assoc_iterator it = c.second.find(ATTR);
				std::string nid = c.second.get<std::string>(ID);
				this->
					build(nid, ((it != config.not_found()) ? (it->second) : (boost::property_tree::ptree())))->
					// recursive entities scan
					build(c.second);
			}
		}
	}

	void entity::build() {
		boost::filesystem::path config_file = this->home / CONFIG_LOCATION;
		if (boost::filesystem::exists(config_file)) {
			//
			boost::property_tree::ptree config;
			boost::property_tree::read_json(config_file.string(), config);
			//
			// merger this->attr and "attr" section from config
			boost::property_tree::ptree::const_assoc_iterator it = config.find(ATTR);
			if (it != config.not_found()) {
				this->mergeAttr(it->second);
			}
			//
			// create hierarchy of entities based on json file
			this->build(config);
			//
			// scan folders to find sub-environments
			boost::filesystem::directory_iterator eit;
			for (boost::filesystem::directory_iterator it(this->home); it != eit; ++it) {
				if (boost::filesystem::is_directory(it->status())) {
					boost::filesystem::path config_file = it->path() / CONFIG_LOCATION;
					if (boost::filesystem::exists(config_file)) {
						this->
							build(it->path().leaf().generic_string(), boost::property_tree::ptree())->
							build();
					}
				}
			}
		}
	}

	void entity::clear() {
		atom::clear_slot<entity_ptr, entity2entities, entity_ptr >(this->shared_from_this());
		base_node_t::clear();
	}

	void entity::mergeAttr(boost::property_tree::ptree const& a){

	}

	//std::cout << c.second.get<std::string>("id") << " " << c.second.get<std::string>("type") << " " << c.second.get<std::string>("attr.git") << std::endl;

}