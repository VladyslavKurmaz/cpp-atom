#pragma once
#include "./classes.hpp"

template < typename T >
class cons_buffer : public boost::noncopyable {
	typename T
		elem_t;
public:
protected:
private:
	///
	size_t
		width;
	///
	size_t
		lines_count;
	///
	size_t
		effective_width;

};