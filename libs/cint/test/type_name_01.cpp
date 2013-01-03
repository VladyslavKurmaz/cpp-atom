/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2012 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladyslav.kurmaz@rozoom-group.com                                           |
|-----------------------------------------------------------------------------|
| FILE:        libs/type_name/test/type_name_01.cpp                           |
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| HISTORY:     2010.09.22                                                     |
|              2012.03.16 move to boost::test                                 |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
|-----------------------------------------------------------------------------|
| TAGS{ SDK                                                                 } |
\-----------------------------------------------------------------------------/
*/

#include <iostream>
//
#include<boost/test/included/unit_test.hpp>
//
#include <atom/cint/type_name.hpp>

boost::unit_test::test_suite * init_unit_test_suite(int,char * * const)
{
	return 0;
}

/*
/-----------------------------------------------------------------------------\
|                                                                             |
\-----------------------------------------------------------------------------/
*/
struct my_struct
{
	int member;
	void function() {}
};
Z3D_TYPE_NAME_DEF_TERM_USER( my_struct );

BOOST_AUTO_TEST_CASE( type_name_01 )
{
		std::cout
			<< atom::cint::type_< my_struct >::name() << std::endl
			<< atom::cint::type_< my_struct const >::name() << std::endl
			<< atom::cint::type_< my_struct volatile >::name() << std::endl
			<< atom::cint::type_< my_struct const volatile >::name() << std::endl
			<< std::endl
			<< atom::cint::type_< int my_struct::* >::name() << std::endl
			<< atom::cint::type_< unsigned char( my_struct::*)() >::name() << std::endl
			<< atom::cint::type_< unsigned char( my_struct::*)( int, int*, int const&, float*&, void(*)(), bool const, long[5], char*[], float my_struct::* const &, void(my_struct::*)() ) >::name() << std::endl
			<< std::endl
			<< atom::cint::type_< int >::name() << std::endl
			<< atom::cint::type_< int const >::name() << std::endl
			<< atom::cint::type_< int volatile >::name() << std::endl
			<< atom::cint::type_< int const volatile >::name() << std::endl
			<< std::endl
			<< atom::cint::type_< int& >::name() << std::endl
			<< atom::cint::type_< int const& >::name() << std::endl
			<< atom::cint::type_< int volatile& >::name() << std::endl
			<< atom::cint::type_< int const volatile& >::name() << std::endl
			<< std::endl
			<< atom::cint::type_< int* >::name() << std::endl
			<< atom::cint::type_< int const* >::name() << std::endl
			<< atom::cint::type_< int volatile* >::name() << std::endl
			<< atom::cint::type_< int const volatile* >::name() << std::endl
			<< std::endl
			<< atom::cint::type_< int*& >::name() << std::endl
			<< atom::cint::type_< int const*& >::name() << std::endl
			<< atom::cint::type_< int volatile*& >::name() << std::endl
			<< atom::cint::type_< int const volatile*& >::name() << std::endl
			<< std::endl
			<< atom::cint::type_< volatile long >::name() << std::endl
			<< atom::cint::type_< volatile long * >::name() << std::endl
			<< atom::cint::type_< volatile long *& >::name() << std::endl
			<< atom::cint::type_< long volatile >::name() << std::endl
			<< atom::cint::type_< long volatile * >::name() << std::endl
			<< atom::cint::type_< long volatile *& >::name() << std::endl
			<< std::endl
			<< atom::cint::type_< void(*)(void) >::name() << std::endl
			<< atom::cint::type_< int**&(*)(char) >::name() << std::endl
			<< atom::cint::type_< unsigned char(*)( void*(*)(int&) ) >::name() << std::endl
			<< atom::cint::type_< int(*)(int) >::name() << std::endl
			<< atom::cint::type_< int const* const * const *& >::name() << std::endl
			<< atom::cint::type_< int**const****const********(*)(int, void*) >::name() << std::endl
			<< atom::cint::type_< int const volatile& >::name() << std::endl
			<< atom::cint::type_< int** [10] >::name() << std::endl
			<< atom::cint::type_< unsigned long const volatile&>::name() << std::endl
			<< atom::cint::type_< void(*)( int const[], float const[256] ) >::name() << std::endl
			<< atom::cint::type_< float[] >::name() << std::endl
			<< std::endl
			;
		unsigned int ui = 10;
		long l = 20;
		long long ll = 100;
		std::cout
			<< atom::cint::param_( 1 ) << std::endl
			<< atom::cint::param_( -1 ) << std::endl
			<< atom::cint::param_( 0x24 ) << std::endl
			<< atom::cint::param_( 0777 ) << std::endl
			<< atom::cint::param_( ui ) << std::endl
			<< atom::cint::param_( l ) << std::endl
			<< atom::cint::param_( ll ) << std::endl
			<< atom::cint::param_( &ui ) << std::endl
			<< atom::cint::param_( &l ) << std::endl
			<< atom::cint::param_( &ll ) << std::endl
			<< atom::cint::param_< unsigned int& >( ui ) << std::endl
			<< atom::cint::param_< long& >( l ) << std::endl
			<< atom::cint::param_< long long& >( ll ) << std::endl
			<< atom::cint::param_( 1.f ) << std::endl
			<< atom::cint::param_( 1.0 ) << std::endl
			<< atom::cint::param_( "abcdef" ) << std::endl
			<< atom::cint::param_( L"abcdef" ) << std::endl
			;
}
