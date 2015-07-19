#include "./pch.hpp"
#include "./logger.hpp"
#include "./context.hpp"
#include "./comp.hpp"

namespace dev {

	boost::filesystem::path const comp::CONFIG_LOCATION(boost::filesystem::path(".conf") / boost::filesystem::path("catalog.json"));
	std::string const       comp::COMP("comp");
	std::string const       comp::ID("id");
	std::string const       comp::PREREQUISITES("prerequisites");
	std::string const       comp::INHERITS("inherits");
	std::string const       comp::ATTR("attr");
	std::string const       comp::VARS("var");
	std::string const       comp::STAGES("stage");

	comp::comp(logger_ptr l, comp_ptr p, boost::filesystem::path const& h, std::string const& i, boost::property_tree::ptree const& a) :
		home(h)
		, id(i)
		, attr(a) {
		atom::mount<comp2logger>(this, l);
		atom::mount<comp2comp>(this, p);
	}

	comp::~comp() {
	}

	comp_ptr comp::getRootComponent(){
		comp_ptr p = this->getParent();
		if (p){
			return p->getRootComponent();
		}
		return this->shared_from_this();
	}

	///
	std::string comp::getQualifiedId() {
		std::string qn;
		this->getMnemonicName(qn);
		boost::replace_all(qn, CONST_ROOT_SIMBOL, CONST_CMD_DASH);
		return qn;
	}

	void comp::getMnemonicName(std::string& name) {
		comp_ptr p = this->getParent();
		if (p){
			std::string s;
			p->getMnemonicName(s);
			if (s == CONST_ROOT_SIMBOL){
				name = s + this->getId();
			}
			else{
				name = s + CONST_ROOT_SIMBOL + this->getId();
			}
		}
		else{
			name = this->getId();
		}
	}


	const boost::regex e("\\A(\\d{3,4})[- ]?(\\d{4})[- ]?(\\d{4})[- ]?(\\d{4})\\z");
	const std::string machine_format("\\1\\2\\3\\4");
	const std::string human_format("\\1-\\2-\\3-\\4");

	std::string machine_readable_card_number(const std::string s)
	{
		return regex_replace(s, e, machine_format, boost::match_default | boost::format_sed);
	}

	std::string human_readable_card_number(const std::string s)
	{
		return regex_replace(s, e, human_format, boost::match_default | boost::format_sed);
	}

	void comp::echo(std::ostream& os, std::string const& offset, std::string const& regex, bool const recursive){
		//
		struct _{
			static void __(comp_ptr const& e, std::ostream& os, std::string const& o, std::string const& re, bool const r) {
				std::string const& id = e->getId();
				const boost::regex ex(re);
				if (!re.length() || regex_match(id, ex)){
					os << o << id << std::endl;
					if (r){
						e->echo(os, o + " ", re, r);
					}
				}
			};
		};
		this->get_slot<comp2comps>().for_each(boost::bind(&_::__, _1, boost::ref(os), boost::ref(offset), boost::ref(regex), recursive));
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

	comp_ptr comp::findCompUp(std::string const& compId){
		comp_ptr e = this->findChild(compId);
		if (!e && this->hasParent()) {
			e = this->getParent()->findCompUp(compId);
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
				e = this->getRootComponent();
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

	void comp::buildPrerequisitesList(platform_t const& platform, comps_t& comps) {
		this->linearizeHierarchy(platform, PREREQUISITES, comps);
	}

	void comp::buildInheritsFromList(platform_t const& platform, comps_t& comps) {
		this->linearizeHierarchy(platform, INHERITS, comps);
	}
	
	void comp::linearizeHierarchy(platform_t const& platform, comp_ptr parent, std::string const& type, std::string const& list, comps_t& comps){
		strings_t ids;
		boost::split(ids, list, boost::is_any_of(CONST_CMD_DELIMITER));
		strings_t::const_iterator it = ids.begin();
		strings_t::const_iterator eit = ids.end();
		for (; it != eit; ++it){
			if ((*it).length()){
				comp_ptr e = parent->findCompUp(*it);
				if (e){
					if (e->checkPlatform(platform)){
						bool add = true;
						BOOST_FOREACH(comp_ptr c, comps) {
							if (c->getId() == e->getId()){
								add = false;
								break;
							}
						}
						if (add){
							e->linearizeHierarchy(platform, type, comps);
							comps.push_back(e);
						}
					}
				}
				else{
					throw std::exception(std::string("Component wasn't found: " + (*it)).c_str());
				}
			}
		}
	}

	void comp::linearizeHierarchy(platform_t const& platform, std::string const& type, comps_t& comps) {
		boost::optional<std::string> v = this->attr.get_optional<std::string>(type);
		if (v && this->hasParent()){
			comp::linearizeHierarchy(platform, this->getParent(), type, (*v), comps);
		}
	}

	void comp::execute(platform_t const& platform, std::string const& stage, bool const recursive, bool const idle){
		if (recursive) {
			// send command down to the children components
			struct _{
				static void __(comp_ptr e, platform_t const& p, std::string const& s, bool const r, bool const i) {
					e->execute(p, s, r, i);
				}
			};
			comp_ptr e;
			this->get_slot<comp2comps>().for_each(boost::bind(&_::__, _1, boost::ref(platform), boost::ref(stage), recursive, idle));
		}
		//
		context_ptr cnxt = context::create(this->getLogger(), platform);
		//
		comps_t prers;
		this->buildPrerequisitesList(platform, prers);
		BOOST_FOREACH(comp_ptr prs, prers) {
			prs->buildScript(platform, stage, SCRIPT_MASK_ENVIRONMENT, cnxt);
		}
		//
		this->buildScript(platform, stage, SCRIPT_MASK_ALL, cnxt);
		//
		std::ofstream script;
		//??? copy cmd file into home directory during make stage
		//boost::filesystem::path fname = this->getHome() / boost::filesystem::path(this->getId() + CONST_CMD_FILE_EXT);
		boost::filesystem::path fname = boost::filesystem::path(getenv("TEMP")) / boost::filesystem::path(stage + this->getQualifiedId() + CONST_CMD_FILE_EXT);
		script.open(fname.c_str());
		script << cnxt->getScriptText().c_str();
		script.flush();
		script.close();
		//
		if (idle){
			*(this->getLogger()) << cnxt->getScriptText().c_str() << std::endl;
		} else {
			atom::proc exec;
			if (exec.run(fname.string(), true)){
				exec.join();
			}
		}
	}

	void comp::clear() {
		atom::clear_slot<comp_ptr, comp2comps, comp_ptr >(this->shared_from_this());
		base_node_t::clear();
	}

	void comp::mergeAttr(boost::property_tree::ptree const& a){

	}

	bool comp::checkPlatform(platform_t const& platform) const{
		struct ptag_t{
			std::string const& value;
			std::string const name;
		} const ptags[] = {
			{ platform.operatingSystem, "mask.operating-system" },
			{ platform.instractionSet, "mask.instruction" },
			{ platform.memoryModel, "mask.address" },
			{ platform.toolset, "mask.toolset" },
			{ platform.configuration, "mask.confguration" }
		};
		BOOST_FOREACH(ptag_t const& t, ptags) {
			boost::optional< const boost::property_tree::ptree& > a = this->attr.get_child_optional(t.name);
			if (a && ( a->data() != t.value)){
				return false;
			}
		}
		return true;
	}

	void comp::constructScript(std::string const& stage, script_mask_t const mask, context_ptr cnxt) const {
		// environment variables
		if (mask & SCRIPT_MASK_ENVIRONMENT) {
			boost::optional< const boost::property_tree::ptree& > vars = this->attr.get_child_optional(VARS);
			if (vars) {
				BOOST_FOREACH(boost::property_tree::ptree::value_type const & v, *vars) {
					cnxt->addEnvVar(v.first, v.second.data());
				}
			}
		}
		// packages
		if (mask & SCRIPT_MASK_PACKAGES) {
		}
			// stages
		if (mask & SCRIPT_MASK_STAGES) {
			boost::optional< const boost::property_tree::ptree& > stages = this->attr.get_child_optional(STAGES);
			if (stages) {
				BOOST_FOREACH(boost::property_tree::ptree::value_type const & s, *stages) {
					if (s.first == stage) {
						cnxt->addScriptLines(s.second.data());
					}
				}
			}
		}
	}

	void comp::buildScript(platform_t const& platform, std::string const& stage, script_mask_t const mask, context_ptr cnxt) {
		comps_t inhs;
		this->buildInheritsFromList(platform, inhs);
		BOOST_FOREACH(comp_ptr cp, inhs) {
			cp->constructScript(stage, mask, cnxt);
		}
		this->constructScript(stage, mask, cnxt);
		cnxt->resolveScript(this->shared_from_this());
	}
}