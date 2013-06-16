/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2010.06.22 - move code from node.hpp                           |
|              2012.11.28 - lib has been moved to github, new namespace: atom |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_NODE_NLIST_HPP
#define ATOM_NODE_NLIST_HPP

#include <list>
#include <boost/function.hpp>

namespace atom {
	//-----------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------
	template < class T >
	class nlist : protected std::list< T >
	{
		///
		typedef std::list< T >
			base_type;
		///
		typedef typename base_type::value_type
			value_type;

	public:
		///
		template < class N >
		void insert( value_type v, N const& ) {
			base_type::insert( base_type::end(), v ); }
		///
		template < class N >
		void erase( value_type v, N const& ) {
			base_type::erase( std::find( base_type::begin(), base_type::end(), v ) ); }
		///
		template < class N >
		void clear( N const& ) {
			base_type::clear(); }
		///
		bool for_each( boost::function< bool( T const& ) > pred  ) const {
			base_type::const_iterator it 	= base_type::begin();
			base_type::const_iterator eit 	= base_type::end();
			for( ; it != eit; ++it ) {
				if ( pred( (*it) ) ) {
					return true;
				}
			}
			return false;
		}
		///
		bool for_each( boost::function< bool( T& ) > pred  ) {
			base_type::iterator it 	= base_type::begin();
			base_type::iterator eit = base_type::end();
			for( ; it != eit; ++it ) {
				if ( pred( (*it) ) ) {
					return true;
				}
			}
			return false;
		}
		///
		size_t size() const {
			return ( base_type::size() );
		}

	};
}
#endif//ATOM_NODE_NLIST_HPP
