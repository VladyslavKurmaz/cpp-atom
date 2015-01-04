#pragma once

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > shell2logger;
typedef atom::nstorage< pref, boost::shared_ptr, atom::narray1 > shell2pref;

class shell :
	public atom::node< LOKI_TYPELIST_2( shell2logger, shell2pref ) >,
	public boost::enable_shared_from_this< shell > {
public:
	typedef atom::node< LOKI_TYPELIST_2( shell2logger, shell2pref ) >
		base_t;
	///
	static shell_ptr create( logger_ptr l, pref_ptr p ) {
		return shell_ptr( new shell( l, p ) );
	}
	///
	~shell();
	///
	bool
		cmd( int const id );
	///
	void
		paint( paint_param_t& paintParam, RECT const& rect );
	///
	void
		clear();

protected:
	//
	logger& getLogger() {
		return ( *( get_value( boost::mpl::identity< shell2logger >() ).item() ) );
	}
	//
	pref& getPref() {
		return ( *( get_value( boost::mpl::identity< shell2pref >() ).item() ) );
	}

private:
	///
	shell( logger_ptr l, pref_ptr p );
	//
	area_ptr
		headArea;
	//
	frame_ptr
		currentFrame;
	//
	bool
		expandMode;
};
