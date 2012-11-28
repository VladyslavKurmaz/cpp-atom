/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2012 by Vladislav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladyslav.kurmaz@rozoom-group.com                                           |
|-----------------------------------------------------------------------------|
| FILE:        libs/script/scope/test/cint_01.cpp                             |
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| HISTORY:     2012.03.25                                                     |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
|-----------------------------------------------------------------------------|
| TAGS{                                                                     } |
\-----------------------------------------------------------------------------/
*/

#include <iostream>
//
#include <boost/test/included/unit_test.hpp>
//
#include <z3d/platform.hpp>
#define Z3D_SCRIPT_CINT_INTERPRETER
#include <z3d/script/cint.hpp>

boost::unit_test::test_suite * init_unit_test_suite(int,char * * const)
{
	return 0;
}

BOOST_AUTO_TEST_CASE( cint_01 )
{
	z3d::util::log l;
	l.add_std_cout();
	z3d::script::cint::language::language_ptr_t lang = z3d::script::cint::language::create( l );
	if ( lang->init( "", "" ) )
	{
		z3d::script::cint::context::context_ptr_t cont = lang->make_context();
	}
}
