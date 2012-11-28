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

#ifndef ATOM_NODE_NSET_HPP
#define ATOM_NODE_NSET_HPP

#include <set>

namespace atom {
	//-----------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------
	template < class T >
	class nset : protected std::set< T >
	{
		///
		typedef std::set< T >
			base_type;
		///
		typedef typename base_type::value_type
			value_type;

	public:
		///
		template < class N >
		void insert( value_type v, N const& )
			{ base_type::insert( v ); }
		///
		template < class N >
		void erase( value_type v, N const& )
			{ base_type::erase( v ); }
		///
		template < class N >
		void clear( N const& )
			{ base_type::clear(); }
	};
}
#endif//ATOM_NODE_NSET_HPP
