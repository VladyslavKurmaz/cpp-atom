#pragma once

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > langs2logger;

class langs :
  public atom::node< LOKI_TYPELIST_1( langs2logger ) >,
  public boost::enable_shared_from_this< langs > {
public:
  typedef atom::node< LOKI_TYPELIST_1( langs2logger ) >
    base_t;
  ///
  static langs_ptr create(boost::property_tree::ptree const& c, logger_ptr l) {
    return langs_ptr( new langs( c, l) );
  }
  ///
  virtual ~langs();
  ///
  void clear();
  //
  void setPair(langspair_t const& lngs);
  //
  langspair_t getPair() const;
  //
  void setLang(bool const from, size_t const ind);
  //
  size_t count() const;
  //
  void foreach(boost::function< bool(lang_t const&) > func) const;

protected:
  //
  LOGGER_ACCESSOR( langs2logger )
  ///
  langs(boost::property_tree::ptree const& c, logger_ptr l);

private:
	///
	boost::property_tree::ptree const&
		config;
	typedef std::vector < lang_t >
		langs_t;
	langspair_t langsPair;
	langs_t languages;
};
