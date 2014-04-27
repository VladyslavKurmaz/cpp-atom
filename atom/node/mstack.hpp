/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2014 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2010.06.23 - prototype design                                  |
|              2010.07.02 - first push/pop logic variant                      |
|              2012.06.10 - LOKI_TYPELIST_0 -> LOKI_TYPELIST_0() hotfix       |
|              2012.11.28 - lib has been moved to github, new namespace: atom |
|-----------------------------------------------------------------------------|
| TODO:		   - add state streams, mstack2                                     |
|              - добавить состояние вызова, как копию списка состояний        |
|              - разнести функциональность на несколько классов, с целью      |
|              уменьшения количества параметров шаблонов                      |
|              - несколько потоков состояний, взаимодействие, блокировка      |
|              пересмотреть дизайн, расширить концепцию                       |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_NODE_MSTACK_HPP
#define ATOM_NODE_MSTACK_HPP

#include <stack>
//
#pragma warning ( push )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4512 )
#	include <boost/thread.hpp>
//#	include <boost/interprocess/sync/scoped_lock.hpp>
#pragma warning ( pop )
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_cv.hpp>
//
#include <atom/node/lock.hpp>
#include <atom/node/node.hpp>
#include <atom/node/gateway.hpp>

namespace atom {
	//-----------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------
	template < 
		typename T,
		typename R,
		typename PU = boost::function< void( T const&, T const& ) >,
		typename PO = boost::function< void( T const&, T const&, R& ) >,
		class M = atom::no_lock,
		bool A = false,
		class L = LOKI_TYPELIST_0(),
		size_t I = 0 >
	class mstack : public gateway< boost::function< T( size_t const ) >, L, I >
	{
		///
		typedef T
			value_type;
		///
		typedef R
			result_type;
		///
		typedef M
			mutex_t;
		///
		typedef boost::lock_guard< typename boost::remove_reference< mutex_t >::type >
			lock_t;
		///
		typedef PU
			push_t;
		///
		typedef PO
			pop_t;
		///
		struct state_t
		{
			value_type
				value;
			push_t
				push;
			pop_t
				pop;
			state_t( value_type v, push_t pu, pop_t po ) : value( v ), push( pu ), pop( po )
				{}
			state_t( state_t const& s ) : value( s.value ), push( s.push ), pop( s.pop )
				{}
		};
		///
		typedef std::vector< state_t >
			states_t;
		///
		states_t
			states;
		///
		mutex_t
			mutex;

	protected:
	public:
		///
		void push( value_type v, push_t push, pop_t pop )
		{
			lock_t lock( this->mutex );
			state_t ns( v, push, pop );
			if ( this->states.size() )
			{
				state_t ps( this->states.back() );
				this->states.push_back( ns );
				ps.push( ps.value, ns.value );
				ns.push( ns.value, ns.value );
			}
			else
			{
				this->states.push_back( ns );
				ns.push( ns.value, ns.value );
			}
		}
		///
		result_type push( value_type v, push_t push, pop_t pop, result_type r )
		{
			lock_t lock( this->mutex );
			state_t ns( v, push, pop );
			if ( this->states.size() )
			{
				state_t ps( this->states.back() );
				this->states.pop_back();
				this->states.push_back( ns );
				ps.pop( ps.value, ns.value, r );
				ns.push( ns.value, ps.value );
			}
			else
			{
				this->states.push_back( ns );
				ns.push( ns.value, ns.value );
			}
			return ( r );
		}
		///
		result_type pop( result_type r )
		{
			lock_t lock( this->mutex );
			if ( this->states.size() )
			{
				state_t ps( this->states.back() );
				this->states.pop_back();
				if ( this->states.size() )
				{
					state_t ns( this->states.back() );
					ps.pop( ps.value, ps.value, r );
					ns.pop( ns.value, ps.value, r );
				}
				else
				{
					ps.pop( ps.value, ps.value, r );
				}
			}
			return ( r );
		}
		///
		result_type pop_all( result_type r )
		{
			lock_t lock( this->mutex );
			while( this->states.size() )
			{
				this->pop( r );
			}
			return ( r );
		}
		///
		value_type top( size_t const i )
		{
			lock_t lock( this->mutex );
			if ( i < states.size() )//???????? use at instead [] for vector size check
				return ( states[ states.size() - i ].value );
			throw std::range_error( "atom::mstack::top invalid state index" );
			// generate exception
			return( value_type() );
		}
		///
		mstack() : states(), mutex()
			{ /*this->set_accessor( boost::bind( &mstack::top, this, _1, 0 ) );*/ }
		///
		mstack( mutex_t m ) : states(), mutex( m )
			{ /*this->set_accessor( boost::bind( &mstack::top, this, _1, 0 ) );*/ }
		///
		size_t size() const
			{ return ( states.size() ); }
	};
}
#endif//ATOM_NODE_MSTACK_HPP
