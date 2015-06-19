#pragma once

template < class T >
entity_ptr create_entity( logger_ptr l, entity_ptr p ) {
	return entity_ptr( new T( l, p ) );
}



class entity :
	public atom::node< LOKI_TYPELIST_3( entity2logger, entity2entity, entity2entities ) > {

	typedef atom::node< LOKI_TYPELIST_3( entity2logger, entity2entity, entity2entities ) >
		base_node_t;

public:
	struct 
	///
	virtual ~entity();
	///
	virtual void
	scan();
	///
	virtual void
	clear();

protected:
	///
	static boost::filesystem::path
		dev_home;
	///
	logger_ptr get_logger() {
		return ( get_slot<filter2logger>().item() );
	}
	///
	entity( logger_ptr l, entity_ptr p, string_t const& sid, boost::property_tree::ptree const& a );
	///
	boost::filesystem::path
	get_id() const;
	///
	boost::filesystem::path
	get_home() const;


private:
	///
	boost::filesystem::path
		id;
	///
	boost::property_tree::ptree
		attr;

};
