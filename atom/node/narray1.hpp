/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2012 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladyslav.kurmaz@rozoom-group.com                                           |
|-----------------------------------------------------------------------------|
| FILE:        z3d/narray1.hpp                                                |
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2010.07.07 - move class from node.hpp                          |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
|-----------------------------------------------------------------------------|
| TAGS{ SDK                                                                 } |
\-----------------------------------------------------------------------------/
*/

#ifndef Z3D_NARRAY1_HPP
#define Z3D_NARRAY1_HPP

#include <boost/array.hpp>

namespace z3d {
	//-------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------
	template < class T >
	class narray1 : protected boost::array< T, 1 >
	{
		///
		typedef typename boost::array< T, 1 >::value_type
			value_type;

	public:
		///
		template < class N >
		void insert( value_type v, N const& )
		{
			this->elems[0] = v;
		}
		///
		template < class N >
		void erase( value_type v, N const& )
		{
			assert( this->elems[0] == v );
			this->elems[0] = value_type();
		}
		///
		template < class N >
		void clear( N const& )
		{
			this->elems[0] = value_type();
		}
		///
		value_type item() const
		{
			return ( this->elems[0] );
		}
	};
}
#endif//Z3D_NARRAY1_HPP
