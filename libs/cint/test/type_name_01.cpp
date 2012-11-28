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
#include <z3d/type_name.hpp>

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
			<< z3d::type_name::type_< my_struct >::name() << std::endl
			<< z3d::type_name::type_< my_struct const >::name() << std::endl
			<< z3d::type_name::type_< my_struct volatile >::name() << std::endl
			<< z3d::type_name::type_< my_struct const volatile >::name() << std::endl
			<< std::endl
			<< z3d::type_name::type_< int my_struct::* >::name() << std::endl
			<< z3d::type_name::type_< unsigned char( my_struct::*)() >::name() << std::endl
			<< z3d::type_name::type_< unsigned char( my_struct::*)( int, int*, int const&, float*&, void(*)(), bool const, long[5], char*[], float my_struct::* const &, void(my_struct::*)() ) >::name() << std::endl
			<< std::endl
			<< z3d::type_name::type_< int >::name() << std::endl
			<< z3d::type_name::type_< int const >::name() << std::endl
			<< z3d::type_name::type_< int volatile >::name() << std::endl
			<< z3d::type_name::type_< int const volatile >::name() << std::endl
			<< std::endl
			<< z3d::type_name::type_< int& >::name() << std::endl
			<< z3d::type_name::type_< int const& >::name() << std::endl
			<< z3d::type_name::type_< int volatile& >::name() << std::endl
			<< z3d::type_name::type_< int const volatile& >::name() << std::endl
			<< std::endl
			<< z3d::type_name::type_< int* >::name() << std::endl
			<< z3d::type_name::type_< int const* >::name() << std::endl
			<< z3d::type_name::type_< int volatile* >::name() << std::endl
			<< z3d::type_name::type_< int const volatile* >::name() << std::endl
			<< std::endl
			<< z3d::type_name::type_< int*& >::name() << std::endl
			<< z3d::type_name::type_< int const*& >::name() << std::endl
			<< z3d::type_name::type_< int volatile*& >::name() << std::endl
			<< z3d::type_name::type_< int const volatile*& >::name() << std::endl
			<< std::endl
			<< z3d::type_name::type_< volatile long >::name() << std::endl
			<< z3d::type_name::type_< volatile long * >::name() << std::endl
			<< z3d::type_name::type_< volatile long *& >::name() << std::endl
			<< z3d::type_name::type_< long volatile >::name() << std::endl
			<< z3d::type_name::type_< long volatile * >::name() << std::endl
			<< z3d::type_name::type_< long volatile *& >::name() << std::endl
			<< std::endl
			<< z3d::type_name::type_< void(*)(void) >::name() << std::endl
			<< z3d::type_name::type_< int**&(*)(char) >::name() << std::endl
			<< z3d::type_name::type_< unsigned char(*)( void*(*)(int&) ) >::name() << std::endl
			<< z3d::type_name::type_< int(*)(int) >::name() << std::endl
			<< z3d::type_name::type_< int const* const * const *& >::name() << std::endl
			<< z3d::type_name::type_< int**const****const********(*)(int, void*) >::name() << std::endl
			<< z3d::type_name::type_< int const volatile& >::name() << std::endl
			<< z3d::type_name::type_< int** [10] >::name() << std::endl
			<< z3d::type_name::type_< unsigned long const volatile&>::name() << std::endl
			<< z3d::type_name::type_< void(*)( int const[], float const[256] ) >::name() << std::endl
			<< z3d::type_name::type_< float[] >::name() << std::endl
			<< std::endl
			;
		unsigned int ui = 10;
		long l = 20;
		long long ll = 100;
		std::cout
			<< z3d::type_name::param_( 1 ) << std::endl
			<< z3d::type_name::param_( -1 ) << std::endl
			<< z3d::type_name::param_( 0x24 ) << std::endl
			<< z3d::type_name::param_( 0777 ) << std::endl
			<< z3d::type_name::param_( ui ) << std::endl
			<< z3d::type_name::param_( l ) << std::endl
			<< z3d::type_name::param_( ll ) << std::endl
			<< z3d::type_name::param_( &ui ) << std::endl
			<< z3d::type_name::param_( &l ) << std::endl
			<< z3d::type_name::param_( &ll ) << std::endl
			<< z3d::type_name::param_< unsigned int& >( ui ) << std::endl
			<< z3d::type_name::param_< long& >( l ) << std::endl
			<< z3d::type_name::param_< long long& >( ll ) << std::endl
			<< z3d::type_name::param_( 1.f ) << std::endl
			<< z3d::type_name::param_( 1.0 ) << std::endl
			<< z3d::type_name::param_( "abcdef" ) << std::endl
			<< z3d::type_name::param_( L"abcdef" ) << std::endl
			;
}
