#include "./pch.hpp"
#include "./logger.hpp"
#include "./comp.hpp"

namespace dev {

  boost::filesystem::path const comp::CONFIG_LOCATION(boost::filesystem::path(".conf") / boost::filesystem::path("catalog.json"));
  std::string const       comp::COMP("comp");
  std::string const       comp::ID("id");
  std::string const       comp::ATTR("attr");
  std::string const       comp::DEPENDS("depends");
  std::string const       comp::INHERITS("inherits");


  comp::comp(logger_ptr l, comp_ptr p, boost::filesystem::path const& h, std::string const& i, boost::property_tree::ptree const& a) :
    home(h)
    , id(i)
    , attr(a) {
    atom::mount<comp2logger>(this, l);
    atom::mount<comp2comp>(this, p);
    //*(this->getLogger()) << this->getId().c_str() << std::endl;
  }

  comp::~comp() {
  }

  comp_ptr comp::getRoot(){
    comp_ptr p = this->getParent();
    if (p){
      return p->getRoot();
    }
    return this->shared_from_this();
  }

  void comp::getAbsolutePath(std::string& path){
    comp_ptr p = this->getParent();
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

  void comp::echo(std::ostream& os, std::string const& offset){
    bool hp = offset.length() > 0;
    os << offset;
    if (hp){
      os << "-";
    }
    os << this->getId() << std::endl;
    std::string off = offset + ((hp) ? (" ") : ("")) + "|";
    //
    struct _{
      static void __(comp_ptr const& e, std::ostream& os, std::string const& o) {
        e->echo(os, o);
      }
    };
    this->get_slot<comp2comps>().for_each(boost::bind(&_::__, _1, boost::ref(os), boost::ref(off)));
  }

  comp_ptr comp::findChild(std::string const& childId){
    struct _{
      static void __(comp_ptr const& e, std::string const& nid, comp_ptr& ne) {
        if (e->getId() == nid){
          ne = e;
        }
      }
    };
    comp_ptr e;
    this->get_slot<comp2comps>().for_each(boost::bind(&_::__, _1, boost::ref(childId), boost::ref(e)));
    return e;
  }

  comp_ptr comp::findcompUp(std::string const& compId){
    comp_ptr e = this->findChild(compId);
    if (!e && this->hasParent()) {
      e = this->getParent()->findcompUp(compId);
    }
    return e;

  }

  ///
  comp_ptr comp::find(std::string const& path){
    comp_ptr r;
    strings_t parts;
    size_t offset = 0;
    boost::split(parts, path, boost::is_any_of(CONST_ROOT_SIMBOL));
    if (path.size()){
      // relative path
      comp_ptr e = this->shared_from_this();
      if (!parts[0].length()){
        // absolute path
        e = this->getRoot();
        offset++;
      }
      r = e->find(offset, parts);
    }
    return r;
  }

  comp_ptr comp::find(size_t const offset, strings_t const& path){
    if (offset < path.size()){
      std::string const& p = path[offset];
      if (p == CONST_LEVEL_UP){
        comp_ptr p = this->getParent();
        if (p){
          return p->find(offset + 1, path);
        }
      }
      else if (p.length()){
        comp_ptr e = this->findChild(p);
        if (e){
          return e->find(offset + 1, path);
        }
        return e;
      }
    }
    return this->shared_from_this();
  }

  comp_ptr comp::build(std::string const& idcomp, boost::property_tree::ptree const& attributes){
    comp_ptr e = this->findChild(idcomp);
    if (e) {
      e->mergeAttr(attributes);
    }
    else{
      e = comp::create(this->getLogger(), this->shared_from_this(), this->getHome() / boost::filesystem::path(idcomp), idcomp, attributes);
      atom::mount<comp2comps>(this, e);
    }
    return (e);
  }

  void comp::build(boost::property_tree::ptree const& config){
    boost::property_tree::ptree::const_assoc_iterator it = config.find(COMP);
    if (it != config.not_found()) {
      BOOST_FOREACH(boost::property_tree::ptree::value_type const & c, config.get_child(COMP)) {
        boost::property_tree::ptree::const_assoc_iterator it = c.second.find(ATTR);
        std::string nid = c.second.get<std::string>(ID);
        //
        boost::optional< const boost::property_tree::ptree& > child = c.second.get_child_optional(ATTR);
        this->
          build(nid, ((child) ? (it->second) : (boost::property_tree::ptree())))->
          // recursive comps scan
          build(c.second);
      }
    }
  }

  void comp::build() {
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
      // create hierarchy of comps based on json file
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

  void comp::buildDependsOnList(std::string const& list, comps_t& comps){
    comp::linearizeHierarchy(this->shared_from_this(), DEPENDS, list, comps);
  }

  void comp::buildInheritsFromList(comps_t& comps){
    this->linearizeHierarchy(INHERITS, comps);
  }


  void comp::linearizeHierarchy(comp_ptr parent, std::string const& type, std::string const& list, comps_t& comps){
    strings_t ids;
    boost::split(ids, list, boost::is_any_of(CONST_CMD_DELIMITER));
    strings_t::const_iterator it = ids.begin();
    strings_t::const_iterator eit = ids.end();
    for (; it != eit; ++it){
      if ((*it).length()){
        comp_ptr e = parent->findcompUp(*it);
        if (e){
          bool add = true;
          comps_t::const_iterator ent_it = comps.begin();
          comps_t::const_iterator ent_eit = comps.end();
          for (; ent_it != ent_eit; ++ent_it){
            if ((*ent_it)->getId() == e->getId()){
              add = false;
              break;
            }
          }
          if (add){
            e->linearizeHierarchy(type, comps);
            comps.push_back(e);
          }
        }
        else{
          throw std::exception(std::string("Component wasn't found: " + (*it)).c_str());
        }
      }
    }
  }

  void comp::linearizeHierarchy(std::string const& type, comps_t& comps){
    boost::optional<std::string> v = this->attr.get_optional<std::string>(type);
    if (v && this->hasParent()){
      comp::linearizeHierarchy(this->getParent(), type, (*v), comps);
    }
  }

  void comp::clear() {
    atom::clear_slot<comp_ptr, comp2comps, comp_ptr >(this->shared_from_this());
    base_node_t::clear();
  }

  void comp::mergeAttr(boost::property_tree::ptree const& a){

  }

  //std::cout << c.second.get<std::string>("id") << " " << c.second.get<std::string>("type") << " " << c.second.get<std::string>("attr.git") << std::endl;

}