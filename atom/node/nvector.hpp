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

#ifndef ATOM_NODE_NVECTOR_HPP
#define ATOM_NODE_NVECTOR_HPP

#include <vector>

namespace atom {
	//-----------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------
	template < class T >
	class nvector : protected std::vector< T >
	{
		///
		typedef std::vector< T >
			base_type;
		///
		typedef typename base_type::value_type
			value_type;

	public:
		///
		template < class N >
		void insert( value_type v, N const& )
		{
			base_type::push_back( v );
		}
		///
		template < class N >
		void erase( value_type v, N const& )
		{
			base_type::erase( std::find( base_type::begin(), base_type::end(), v ) );
		}
		///
		template < class N >
		void clear( N const& )
		{
			base_type::clear();
		}
	};
}
#endif//ATOM_NODE_NVECTOR_HPP
