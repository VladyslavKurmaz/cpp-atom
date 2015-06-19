#pragma once

class repo :
	public entity,
	public boost::enable_shared_from_this< repo > {

public:
	///
	virtual ~repo();
	///
	virtual void
	clear();

protected:
	///
	repo( logger_ptr l, entity_ptr p, string_t const& sid, boost::property_tree::ptree const& a );

private:

	template < class T >
	friend entity_ptr create_entity( logger_ptr, entity_ptr, string_t const& );
};
