#pragma once
#include <atom/util/log.hpp>

class logger : public atom::log, public boost::enable_shared_from_this< logger >
{
public:
	///
	typedef boost::shared_ptr<logger> shared_ptr;
	///
	static shared_ptr create() {
		return shared_ptr( new logger );
	}

	~logger() {
		*this << "free log" << std::endl;
	}


private:
	///
	logger() : atom::log() {
		*this << "create log" << std::endl;
	}
};