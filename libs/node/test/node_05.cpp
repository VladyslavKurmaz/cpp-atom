/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2012 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladyslav.kurmaz@rozoom-group.com                                           |
|-----------------------------------------------------------------------------|
| FILE:        libs/node/test/node_05.cpp                                     |
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| HISTORY:     2010.08.27                                                     |
|              2012.03.15 - move to boost::test                               |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
|-----------------------------------------------------------------------------|
| TAGS{                                                                     } |
\-----------------------------------------------------------------------------/
*/

#include <boost/test/included/unit_test.hpp>
//
#include <z3d/node.hpp>

boost::unit_test::test_suite * init_unit_test_suite(int,char * * const)
{
	return 0;
}

class ctrl;

BOOST_AUTO_TEST_CASE( node_05 )
{
	std::cout << std::endl << "z3d::utests::node_ut05::test()" << std::endl;
	//
}
