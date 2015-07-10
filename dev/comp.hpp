#pragma once

namespace dev {

  class comp :
    public atom::node < LOKI_TYPELIST_3(comp2logger, comp2comp, comp2comps) >,
    public boost::enable_shared_from_this < comp > {

    typedef atom::node < LOKI_TYPELIST_3(comp2logger, comp2comp, comp2comps) >
      base_node_t;

  public:
    ///
    static boost::filesystem::path const CONFIG_LOCATION;
    static std::string const COMP;
    static std::string const ID;
    static std::string const ATTR;
    static std::string const DEPENDS;
    static std::string const INHERITS;
    ///
    static comp_ptr create(logger_ptr l, comp_ptr p, boost::filesystem::path const& h, std::string const& i, boost::property_tree::ptree const& a) {
      return comp_ptr(new comp(l, p, h, i, a));
    }
    ///
    ~comp();
    ///
    std::string const& getId() const { return this->id; }
    ///
    comp_ptr getParent(){ return ((get_value(boost::mpl::identity< comp2comp >()).item())); }
    ///
    bool hasParent(){ return ((get_value(boost::mpl::identity< comp2comp >()).item().get() != NULL )); }
    ///
    bool hasChildren(){ return ((get_value(boost::mpl::identity< comp2comps >()).size() > 0)); }
    //
    comp_ptr getRoot();
    ///
    void getAbsolutePath(std::string& path);
    ///
    void echo(std::ostream& os, std::string const& offset);
    ///
    comp_ptr findcompUp(std::string const& compId);
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
    void build();
    ///
    void buildDependsOnList(std::string const& list, comps_t& comps);
    ///
    void buildInheritsFromList(comps_t& comps);
    ///
    void clear();

  protected:
    LOGGER_ACCESSOR(comp2logger);
    ///
    comp(logger_ptr l, comp_ptr p, boost::filesystem::path const& h, std::string const& i, boost::property_tree::ptree const& a);
    ///
    boost::filesystem::path getHome() const { return this->home; };
    ///
    void mergeAttr(boost::property_tree::ptree const& a);
    ///
    static void linearizeHierarchy(comp_ptr parent, std::string const& type, std::string const& list, comps_t& comps);
    ///
    void linearizeHierarchy(std::string const& type, comps_t& comps);

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