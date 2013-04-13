#pragma once
#pragma warning(disable : 4996)
//#pragma warning(disable : 4355)

#include <windows.h>

#include <boost/function.hpp>
#include <boost/smart_ptr.hpp>

#include <atom/util/po.hpp>
#include <atom/util/log.hpp>
#include <atom/node/tldefs.hpp>
#include <atom/node/node.hpp>
#include <atom/util/dbg.hpp>
#include <atom/util/po.hpp>
#include <atom/util/ptr.hpp>
#include <atom/util/cast.hpp>


class logger;
typedef boost::shared_ptr< logger >
	logger_ptr;

class appl;
typedef boost::shared_ptr< appl >
	appl_ptr;

class env;
typedef boost::shared_ptr< env >
	env_ptr;

class comp;
typedef boost::shared_ptr< comp >
	comp_ptr;

typedef std::basic_string< TCHAR >
	string_t;

static atom::po::id_t const po_none					=	0;
static atom::po::id_t const po_help					=	po_none + 1;
static atom::po::id_t const po_interactive			=	po_help + 1;
static atom::po::id_t const po_exit					=	po_interactive + 1;
static atom::po::id_t const po_home					=	po_exit + 1;
static atom::po::id_t const po_stages				=	po_home + 1;
static atom::po::id_t const po_components			=	po_stages + 1;
static atom::po::id_t const po_recursive			=	po_components + 1;




