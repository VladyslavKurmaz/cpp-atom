/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2012 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladyslav.kurmaz@rozoom-group.com                                           |
|-----------------------------------------------------------------------------|
| FILE:        z3d/script/cint.hpp                                            |
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2010.05.21 - Created                                           |
|              2012.03.23 - gcc fix for missed cint main include file         |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
|-----------------------------------------------------------------------------|
| TAGS{ SDK                                                                 } |
\-----------------------------------------------------------------------------/
*/

#ifndef Z3D_SCRIPT_CINT_HPP
#define Z3D_SCRIPT_CINT_HPP

#include <boost/thread/mutex.hpp>
//
#ifdef NDEBUG
#	pragma comment ( lib, "libCint" ) /*libCint_static*/
#else
#	pragma comment ( lib, "libCint" ) /*libCint_staticD*/
#endif
//
#include <z3d/util/log.hpp>
#include <z3d/type_name.hpp>
#include <G__ci.h>
#include <z3d/script/cint/dict.hpp>
#include <z3d/script/cint/cast.hpp>

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
namespace z3d { namespace script { namespace cint {
	class context;
	class language;
} } }

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
struct cint_traits
{
	///
	typedef z3d::util::log
		log_t;
	///
	typedef boost::mutex
		mutex_t;
	///
	typedef z3d::script::cint::dict
		dict_t;
	///
	typedef z3d::script::cint::context
		context_t;
	typedef boost::enable_shared_from_this< context_t >
		context_base_t;
	typedef unsigned int
		context_id_t;
	///
	typedef z3d::script::cint::language
		language_t;
	typedef boost::enable_shared_from_this< language_t >
		language_base_t;
};


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#include <z3d/script/cint/language.hpp>
#include <z3d/script/cint/context.hpp>

#endif//Z3D_SCRIPT_CINT_HPP
