/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2012 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladyslav.kurmaz@rozoom-group.com                                           |
|-----------------------------------------------------------------------------|
| FILE:        z3d/script/cint/dict.hpp                                       |
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2010.08.24 - Created                                           |
|              2012.03.23 - gcc fix for missed typename(s)                    |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
|-----------------------------------------------------------------------------|
| TAGS{ SDK                                                                 } |
\-----------------------------------------------------------------------------/
*/

#ifndef Z3D_SCRIPT_CINT_DICT_HPP
#define Z3D_SCRIPT_CINT_DICT_HPP

#include <set>

namespace z3d { namespace script { namespace cint {

	template < typename H, typename C >
	class basic_dict
	{
		typedef H
			basic_dict_handle_t;
		typedef C
			char_t;
		typedef std::basic_string< char_t >
			string_t;
		typedef char_t const *
			string_ptr_t;
		typedef void*
			func_ptr_t;
		///
		typedef std::set< basic_dict_handle_t >
			handles_t;
		typedef std::map< string_t, func_ptr_t >
			func_ptrs_t;
		///
		handles_t
			handles;
		///
		func_ptrs_t
			func_ptrs;

	protected:
	public:
		typedef basic_dict_handle_t
			handle_t;
		///
		basic_dict() : handles(), func_ptrs()
			{}
		///
		~basic_dict()
			{}
		///
		void add_entry( handle_t h )
			{ this->handles.insert( h ); }
		///
		void delete_entry( handle_t h )
			{ this->handles.erase( h ); }
		///
		template < typename T >
		T get( string_ptr_t name )
		{
			string_t key = name;
			func_ptr_t func = 0;
			typename func_ptrs_t::const_iterator fit = this->func_ptrs.find( key );
			if ( fit != this->func_ptrs.end() )
			{
				func = (*fit).second;
			}
			else
			{
				typename handles_t::const_iterator hit = this->handles.begin();
				typename handles_t::const_iterator ehit = this->handles.end();
				for( ; hit != ehit; ++hit )
				{
					if ( ( func = GetProcAddress( (*hit), name ) ) != NULL )
					{
						this->func_ptrs.insert( std::make_pair( key, func ) );
						break;
					}
				}
			}
			return ( reinterpret_cast< T >( func ) );
		}
	};

#ifdef Z3D_WINDOWS
	typedef basic_dict< HMODULE, char >
		dict;
#endif
} } }
#endif //Z3D_SCRIPT_CINT_DICT_HPP
