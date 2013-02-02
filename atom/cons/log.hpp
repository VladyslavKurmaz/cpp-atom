#pragma once
#include <atom/util/log.hpp>

class logger : public atom::log
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
	}
};