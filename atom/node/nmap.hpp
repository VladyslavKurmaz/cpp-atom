/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2012 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladyslav.kurmaz@rozoom-group.com                                           |
|-----------------------------------------------------------------------------|
| FILE:        z3d/nmap.hpp                                                   |
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2010.08.23 - move code from node.hpp                           |
|              2010.10.19 - added map key compare parameter                   |
|              2012.03.23 - gcc fix for missing typename(s)                   |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
|-----------------------------------------------------------------------------|
| TAGS{ SDK                                                                 } |
\-----------------------------------------------------------------------------/
*/

#ifndef Z3D_NMAP_HPP
#define Z3D_NMAP_HPP

#include <map>

namespace z3d {
	//-----------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------
	template < class T >
	class nmap : protected std::map< typename T::value_type::id_t, T, typename T::value_type::id_cmp_t >
	{
		typedef typename T::value_type::id_t
			id_t;
		///
		typedef std::map< id_t, T, typename T::value_type::id_cmp_t >
			base_type;
		///
		typedef T
			value_type;

	public:
		///
		template < class N >
		void insert( value_type v, N const& )
			{ base_type::insert( std::make_pair( v->get_id(), v ) ); }
		///
		template < class N >
		void erase( value_type v, N const& )
			{ base_type::erase( v->get_id() ); }
		///
		template < class N >
		void clear( N const& )
			{ base_type::clear(); }
		///
		value_type find( id_t const id ) const
		{
			typename base_type::const_iterator it = base_type::find( id );
			if ( it != base_type::end() )
			{
				return ( (*it).second );
			}
			return ( value_type() );
		}
	};
}
#endif//Z3D_NMAP_HPP
