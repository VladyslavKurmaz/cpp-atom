/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2012 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladyslav.kurmaz@rozoom-group.com                                           |
|-----------------------------------------------------------------------------|
| FILE:        libs/chain_of_resp/test/chain_of_rest_03.cpp                   |
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| HISTORY:     2010.08.26                                                     |
|              2012.03.16 move to boost::test                                 |
|-----------------------------------------------------------------------------|
| TODO:		                                                                    |
|-----------------------------------------------------------------------------|
| TAGS{ SDK                                                                 } |
\-----------------------------------------------------------------------------/
*/
#include <boost/test/included/unit_test.hpp>
#include <boost/function.hpp>
//
#include <z3d/chain_of_resp.hpp>

boost::unit_test::test_suite * init_unit_test_suite(int,char * * const)
{
	return 0;
}

namespace {

	// chain function
	typedef boost::function< void( int const ) >
		chain_t;
	//
	class layer : protected z3d::chain_of_resp< LOKI_TYPELIST_1( chain_t ) >,
		public boost::enable_shared_from_this< layer >,
		public boost::noncopyable
	{
		typedef z3d::chain_of_resp< LOKI_TYPELIST_1( chain_t ) >
			base_t;
		///
		std::string
			name;
	protected:
		/// init cons
		layer( std::string const& n ) : name( n )
		{}
	public:
		///
		typedef boost::shared_ptr< layer >
			layer_ptr_t;
		/// working function
		void chain( int const arg )
		{
			std::cout << name << "::chain(" << arg << ")" << std::endl;
			if ( !( base_t::empty< chain_t >() ) )
				base_t::call< chain_t, void >( arg + 1 );
		}
		/// making chain between layers
		layer_ptr_t attach( layer_ptr_t l )
		{
			base_t::bind<chain_t>( boost::bind( &layer::chain, l, _1 ) );
			return ( shared_from_this() );
		}
		///
		static layer_ptr_t create( std::string const& name )
			{ return layer_ptr_t( new layer( name ) ); }
	};
}
BOOST_AUTO_TEST_CASE( chain_of_resp_03 )
{
	std::cout << std::endl << "z3d::chain_of_resp_03" << std::endl;
	//
	layer::layer_ptr_t lA2 = layer::create( "A2" );
	layer::layer_ptr_t lA1 = layer::create( "A1" )->attach( lA2->attach( layer::create( "A3" ) ) );
	std::cout << std::endl << "A1 -> A2 -> A3 chain" << std::endl;
	lA1->chain( 0 );
	layer::layer_ptr_t lB1 = layer::create( "B1" );
	lB1->attach( lA2 );
	std::cout << std::endl << "B1 -> A2 -> A3 chain" << std::endl;
	lB1->chain( 0 );
}

