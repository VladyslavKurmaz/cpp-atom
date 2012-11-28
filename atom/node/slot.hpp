/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2010.06.17 - z3d::node generalization                          |
|              2010.06.28 - added boost::add_references, simplify namespace   |
|              2010.07.26 - added some typedef                                |
|              2012.11.28 - lib has been moved to github, new namespace: atom |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_NODE_SLOT_HPP
#define ATOM_NODE_SLOT_HPP

#include <boost/mpl/int.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/ref.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/add_reference.hpp>
//
#include <loki/Typelist.h>
//
#include <atom/node/tldefs.hpp>
//

namespace atom {
	//-----------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------
	template < class, size_t >
	struct slot;

	template < size_t I >
	struct slot< Loki::NullType, I >
	{
		///
		typedef Loki::NullType
			value_type;
		///
		typedef slot< value_type, I >
			this_type;
		/// indexed access key
		typedef boost::mpl::int_< I >
			indexed_key_t;
		/// typed access key
		typedef boost::mpl::identity< value_type >
			typed_key_t;
		///
		value_type
			value;
		/// default cons
		slot< value_type, I >()
			{}
		/// init cons
		explicit slot< value_type, I >( value_type const & p ) : value( p )
			{}
		/// indexed get
		value_type& get_value( indexed_key_t const & );
		/// typed get
		value_type& get_value( typed_key_t const & );
		/// indexed set
		void set_value( indexed_key_t const &, value_type const & );
		/// typed set
		void set_value( typed_key_t const &, value_type const & );
	};

	template < typename T, typename U, size_t I >
	struct slot< Loki::Typelist< T, U >, I > : public slot< U, I + 1 >
	{
		///
		typedef T
			value_type;
		///
		typedef slot< Loki::Typelist< value_type, U >, I >
			this_type;
		///
		typedef slot< U, I + 1 >
			base_type;
		///
		typedef typename boost::remove_reference< value_type >::type
			unref_value_type;
		/// indexed access key
		typedef typename boost::mpl::int_< I >
			indexed_key_t;
		/// typed access key
		typedef typename boost::mpl::identity< value_type >
			typed_key_t;
		/// base class methods
		using base_type::get_value;
		using base_type::set_value;
		///
		value_type
			value;
		/// default cons
		slot< Loki::Typelist< value_type, U >, I >() : base_type(), value()
			{ }
		/// reference init cons
		template< class P >
		explicit slot< Loki::Typelist< value_type, U >, I >( std::pair< boost::reference_wrapper< unref_value_type >, P > const& p ) : base_type( p.second ), value( p.first )
			{ }
		/// init cons
		template< class P >
		explicit slot< Loki::Typelist< value_type, U >, I >( std::pair< T, P > const& p ) : base_type( p.second ), value( p.first )
			{ }
		/// skip init cons
		template< class P >
		explicit slot< Loki::Typelist< value_type, U >, I >( std::pair< Loki::NullType, P > const& p ) : base_type( p.second ), value()
			{ }
		///
		this_type& operator=( this_type const& );
		/// indexed get
		unref_value_type & get_value( indexed_key_t const & )
			{ return ( this_type::value ); }
		/// typed get
		unref_value_type & get_value( typed_key_t const & )
			{ return ( this_type::value ); }
		/// indexed set
		void set_value( indexed_key_t const &, unref_value_type const & v )
			{ this_type::value = v; }
		/// typed set
		void set_value( typed_key_t const &, unref_value_type const & v )
			{ this_type::value = v; }
	};
}

#endif//ATOM_NODE_SLOT_HPP
