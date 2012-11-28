/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| HISTORY:     2010.07.06                                                     |
|              2012.03.16 move to boost::test                                 |
|              2012.11.28 - lib has been moved to github, new namespace: atom |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
\-----------------------------------------------------------------------------/
*/

#include <boost/smart_ptr.hpp>
#include <boost/test/included/unit_test.hpp>
//
#include <atom/node/node.hpp>
#include <atom/node/mstack.hpp>
#include <atom/node/gateway.hpp>

boost::unit_test::test_suite * init_unit_test_suite(int,char * * const)
{
	return 0;
}

namespace {

	class manager;
	class state;
	//
	typedef atom::nstorage< manager, boost::shared_ptr, atom::narray1 > manager_t;
	typedef boost::shared_ptr< state > state_ptr_t;
	typedef boost::shared_ptr< manager > manager_ptr_t;
	//
	class state :	public atom::node< LOKI_TYPELIST_1( manager_t ) >,
		public boost::enable_shared_from_this< state >,
		public boost::noncopyable
	{
		std::string const
			id;
	protected:
		///
		state( std::string const i ) : id( i )
		{}
		///
		manager_ptr_t get_manager_ptr()
		{
			manager_t const& ma = atom::node< LOKI_TYPELIST_1( manager_t ) >::get_slot< 0 >();
			manager_ptr_t m = ma.item();
			return ( m );
		}

	public:
		virtual void push( state_ptr_t const&, state_ptr_t const& ){ std::cout << "push:" << id << std::endl; }
		virtual void pop( state_ptr_t const&, state_ptr_t const&, bool& ){ std::cout << "pop:" << id << std::endl; }
		virtual void input(){}
		virtual void update(){}
		virtual void render(){}
		///
		template < class T >
		static state_ptr_t create( manager_ptr_t manager )
		{
			state_ptr_t result = state_ptr_t( new T() );
			mount< 0 >( result, manager );
			manager->push( result, boost::bind( &T::push, result, _1, _2 ), boost::bind( &T::pop, result, _1, _2, _3 ) );
			return ( result );
		}

	};
	//
	class boot_state : public state
	{
	protected:
		boot_state() : state( "boot_state" )
		{}
		friend state;
	};
	//
	class game_state : public state
	{
	protected:
		game_state() : state( "game_state" )
		{}
		friend state;
	};
	//
	class msg_state : public state
	{
	protected:
		msg_state() : state( "msg_state" )
		{}
		friend state;
	};
	//
	class manager : protected atom::mstack< state_ptr_t, bool >,
		public boost::enable_shared_from_this< manager >
	{
	public:
		///
		using atom::mstack< state_ptr_t, bool >::pop;
		using atom::mstack< state_ptr_t, bool >::pop_all;

		friend state;
	};
}
BOOST_AUTO_TEST_CASE( mstack_01 )
{
	std::cout << std::endl << "atom::utests::mstack_ut02::test()" << std::endl;
	manager_ptr_t m = manager_ptr_t( new manager() );
	state::create< boot_state >( m );
	state::create< game_state >( m );
	m->pop_all( false );
}

