#pragma once

class logger :
	public atom::log,
	public boost::enable_shared_from_this< logger >
{
public:
	///
	typedef boost::shared_ptr<logger> shared_ptr;
	///
	static shared_ptr create() {
		return shared_ptr( new logger );
	}

	~logger() {
	}


private:
	///
	logger() : atom::log() {
	}
};