/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2012 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladyslav.kurmaz@rozoom-group.com                                           |
|-----------------------------------------------------------------------------|
| FILE:        z3d/nset.hpp                                                   |
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2010.06.22 - move code from node.hpp                           |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
|-----------------------------------------------------------------------------|
| TAGS{ SDK                                                                 } |
\-----------------------------------------------------------------------------/
*/

#ifndef Z3D_NSET_HPP
#define Z3D_NSET_HPP

#include <set>

namespace z3d {
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
#endif//Z3D_NSET_HPP
