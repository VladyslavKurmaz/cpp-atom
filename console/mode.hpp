#pragma once

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > mode2logger;
typedef atom::nstorage< pref, boost::shared_ptr, atom::narray1 > mode2pref;

class mode :
	public atom::node< LOKI_TYPELIST_2( mode2logger, mode2pref ) >,
	public boost::enable_shared_from_this< mode > {
public:
	typedef atom::node< LOKI_TYPELIST_2( mode2logger, mode2pref ) >
		base_t;
	///
	template < class T >
	static mode_ptr create( logger_ptr l, pref_ptr p ) {
		return mode_ptr( new T( l, p ) );
	}
	///
	virtual ~mode(){}
	///
	virtual void activate( bool const state ) = 0;
	///
	virtual void show( bool const state ) = 0;
	///
	virtual bool command( int const id ) = 0;
	//
	virtual void key( KEY_EVENT_RECORD const& k ) = 0;
	///
	virtual void paint( paint_param_t& paintParam, RECT const& rect ) = 0;
	///
	virtual void clear() = 0;

protected:
	//
	logger& getLogger() {
		return ( *( get_value( boost::mpl::identity< mode2logger >() ).item() ) );
	}
	//
	pref& getPref() {
		return ( *( get_value( boost::mpl::identity< mode2pref >() ).item() ) );
	}
	///
	mode( logger_ptr l, pref_ptr p ) {
		atom::mount<mode2logger>( this, l );
		atom::mount<mode2pref>( this, p );
	}

private:
};
