#pragma once

#include "./log.hpp"
#include "./frame.hpp"

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > process2logger;
typedef atom::nstorage< frame, boost::shared_ptr, atom::narray1 > process2frame;


class process : public atom::node< LOKI_TYPELIST_2( process2logger, process2frame ) > {
public:
	///
	typedef boost::shared_ptr< process >
		shared_ptr;
	///
	static shared_ptr create( logger::shared_ptr l, frame::shared_ptr f ) {
		return shared_ptr( new process( l, f ) );
	}
	///
	~process();
	///
	void
		run( std::basic_string<TCHAR> const& cmd );
	///
	void 
		write( std::string const& str );
	///
	void 
		close();
	//
	logger& get_logger() {
		return ( *( get_value( boost::mpl::identity< process2logger >() ).item() ) );
	}

protected:
private:
	///
	process( logger::shared_ptr l, frame::shared_ptr f );
};