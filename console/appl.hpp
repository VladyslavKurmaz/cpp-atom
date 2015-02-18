#pragma once
#include "./classes.hpp"

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > appl2logger;
typedef atom::nstorage< pref, boost::shared_ptr, atom::narray1 > appl2pref;
typedef atom::nstorage< window, boost::shared_ptr, atom::narray1 > appl2window;

class appl :
	public atom::node< LOKI_TYPELIST_3( appl2logger, appl2pref, appl2window ) >,
	public boost::enable_shared_from_this< appl > {
public:
	///
	static appl_ptr create( logger_ptr l, pref_ptr p ) {
		return appl_ptr( new appl( l, p ) );
	}
	///
	~appl();
	///
	bool
		init();
	///
	appl&
		run();
	///
	void
		clear();

protected:
	LOGGER_ACCESSOR( appl2logger )
	PREF_ACCESSOR( appl2pref )
	WINDOW_ACCESSOR( appl2window )

private:
	///
	appl( logger_ptr l, pref_ptr p );
};