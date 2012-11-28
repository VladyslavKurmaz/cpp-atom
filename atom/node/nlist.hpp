/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2012 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladyslav.kurmaz@rozoom-group.com                                           |
|-----------------------------------------------------------------------------|
| FILE:        z3d/nlist.hpp                                                  |
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2010.06.22 - move code from node.hpp                           |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
|-----------------------------------------------------------------------------|
| TAGS{ SDK                                                                 } |
\-----------------------------------------------------------------------------/
*/

#ifndef Z3D_NLIST_HPP
#define Z3D_NLIST_HPP

#include <list>

namespace z3d {
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
		void insert( value_type v, N const& )
		{
			base_type::insert( base_type::end(), v );
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
#endif//Z3D_NLIST_HPP
