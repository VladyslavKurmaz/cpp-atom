#pragma once
#include <boost/function.hpp>
#include <boost/smart_ptr.hpp>

//
class logger;
typedef boost::shared_ptr< logger >
	logger_ptr;
//
class pref;
typedef boost::shared_ptr< pref >
	pref_ptr;
//
class process;
typedef boost::shared_ptr< process >
	process_ptr;
//
class frame;
typedef boost::shared_ptr< frame >
	frame_ptr;
//
class window;
typedef boost::shared_ptr< window >
	window_ptr;
//
class appl;
typedef boost::shared_ptr< appl >
	appl_ptr;
