#include "./pch.hpp"
#include "./logger.hpp"
#include "./entity.hpp"

namespace dev {

	boost::filesystem::path const	entity::CONFIG_LOCATION(boost::filesystem::path(".conf") / boost::filesystem::path("catalog.json"));
	std::string const				entity::ENTITY("entity");
	std::string const				entity::ID("id");
	std::string const				entity::ATTR("attr");
	std::string const				entity::DEPENDS("depends");
	std::string const				entity::INHERITS("inherits");


	entity::entity(logger_ptr l, entity_ptr p, boost::filesystem::path const& h, std::string const& i, boost::property_tree::ptree const& a) :
		home(h)
		, id(i)
		, attr(a) {
		atom::mount<entity2logger>(this, l);
		atom::mount<entity2entity>(this, p);
		//*(this->getLogger()) << this->getId().c_str() << std::endl;
	}

	entity::~entity() {
	}

	entity_ptr entity::getRoot(){
		entity_ptr p = this->getParent();
		if (p){
			return p->getRoot();
		}
		return this->shared_from_this();
	}

	void entity::getAbsolutePath(std::string& path){
		entity_ptr p = this->getParent();
		if (p){
			std::string s;
			p->getAbsolutePath(s);
			if (s == CONST_ROOT_SIMBOL){
				path = s + this->getId();
			}
			else{
				path = s + CONST_ROOT_SIMBOL + this->getId();
			}
		}
		else{
			path = this->getId();
		}
	}

	void entity::echo(std::ostream& os, std::string const& offset){
		bool hp = offset.length() > 0;
		os << offset;
		if (hp){
			os << "-";
		}
		os << this->getId() << std::endl;
		std::string off = offset + ((hp) ? (" ") : ("")) + "|";
		//
		struct _{
			static void __(entity_ptr const& e, std::ostream& os, std::string const& o) {
				e->echo(os, o);
			}
		};
		this->get_slot<entity2entities>().for_each(boost::bind(&_::__, _1, boost::ref(os), boost::ref(off)));
	}

	entity_ptr entity::findChild(std::string const& childId){
		struct _{
			static void __(entity_ptr const& e, std::string const& nid, entity_ptr& ne) {
				if (e->getId() == nid){
					ne = e;
				}
			}
		};
		entity_ptr e;
		this->get_slot<entity2entities>().for_each(boost::bind(&_::__, _1, boost::ref(childId), boost::ref(e)));
		return e;
	}

	entity_ptr entity::findEntityUp(std::string const& entityId){
		entity_ptr e = this->findChild(entityId);
		if (!e && this->hasParent()) {
			e = this->getParent()->findEntityUp(entityId);
		}
		return e;

	}

	///
	entity_ptr entity::find(std::string const& path){
		entity_ptr r;
		strings_t parts;
		size_t offset = 0;
		boost::split(parts, path, boost::is_any_of(CONST_ROOT_SIMBOL));
		if (path.size()){
			// relative path
			entity_ptr e = this->shared_from_this();
			if (!parts[0].length()){
				// absolute path
				e = this->getRoot();
				offset++;
			}
			r = e->find(offset, parts);
		}
		return r;
	}

	entity_ptr entity::find(size_t const offset, strings_t const& path){
		if (offset < path.size()){
			std::string const& p = path[offset];
			if (p == CONST_LEVEL_UP){
				entity_ptr p = this->getParent();
				if (p){
					return p->find(offset + 1, path);
				}
			}
			else if (p.length()){
				entity_ptr e = this->findChild(p);
				if (e){
					return e->find(offset + 1, path);
				}
				return e;
			}
		}
		return this->shared_from_this();
	}

	entity_ptr entity::build(std::string const& identity, boost::property_tree::ptree const& attributes){
		entity_ptr e = this->findChild(identity);
		if (e) {
			e->mergeAttr(attributes);
		}
		else{
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
				//
				boost::optional< const boost::property_tree::ptree& > child = c.second.get_child_optional(ATTR);
				this->
					build(nid, ((child) ? (it->second) : (boost::property_tree::ptree())))->
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

	void entity::buildDependsOnList(std::string const& list, entities_t& entities){
		entity::linearizeHierarchy(this->shared_from_this(), DEPENDS, list, entities);
	}

	void entity::buildInheritsFromList(entities_t& entities){
		this->linearizeHierarchy(INHERITS, entities);
	}


	void entity::linearizeHierarchy(entity_ptr parent, std::string const& type, std::string const& list, entities_t& entities){
		strings_t ids;
		boost::split(ids, list, boost::is_any_of(CONST_CMD_DELIMITER));
		strings_t::const_iterator it = ids.begin();
		strings_t::const_iterator eit = ids.end();
		for (; it != eit; ++it){
			if ((*it).length()){
				entity_ptr e = parent->findEntityUp(*it);
				if (e){
					bool add = true;
					entities_t::const_iterator ent_it = entities.begin();
					entities_t::const_iterator ent_eit = entities.end();
					for (; ent_it != ent_eit; ++ent_it){
						if ((*ent_it)->getId() == e->getId()){
							add = false;
							break;
						}
					}
					if (add){
						e->linearizeHierarchy(type, entities);
						entities.push_back(e);
					}
				}
				else{
					throw std::exception(std::string("Component wasn't found: " + (*it)).c_str());
				}
			}
		}
	}

	void entity::linearizeHierarchy(std::string const& type, entities_t& entities){
		boost::optional<std::string> v = this->attr.get_optional<std::string>(type);
		if (v && this->hasParent()){
			entity::linearizeHierarchy(this->getParent(), type, (*v), entities);
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