#pragma once
#include "./classes.hpp"

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > pref2logger;

class pref :
	public atom::node< LOKI_TYPELIST_1( pref2logger ) >,
	public boost::enable_shared_from_this< pref > {
	typedef atom::node< LOKI_TYPELIST_1( pref2logger ) >
		base_t;
public:
	enum pref_group_t {
		pgEmpty,
		pgAutostart,
		pgHotkeys,
		pgFonts,
		pgWindow,
		pgUI
	};
	typedef boost::function< void() >
		callback_t;
	///
	static pref_ptr create( logger_ptr l ) {
		return pref_ptr( new pref( l ) );
	}
	///
	~pref();
	///
	bool
	init( int argc, char const * const argv[] );
	///
	void
	register_process_callback( pref_group_t const g, callback_t c );
	///
	bool
	parse( std::string const& s );
	///
	template <typename T >
	T get( atom::po::id_t const id ) {
		return ( this->po.as<T>( id ) );
	}
	//
	void
		calculateDocks( placement& windowPlacement );
	//
	bool
		parseHotkey( atom::po::id_t const id, hotkey& hk );

	///
	struct opt2cmd_t {
		atom::po::id_t	opt;
		WORD			cmd;
	};
	template < size_t N >
	void parseAccel( opt2cmd_t const (&tags)[N], atom::accel& accel ) {

		static atom::parse_tag< TCHAR, BYTE > const accel_tags[] = {
			{ "alt",		FALT },
			{ "ctrl",		FCONTROL },
			{ "shift",		FSHIFT }
		};
		static size_t const accel_tags_count = sizeof( accel_tags ) / sizeof( accel_tags[0] );

		static atom::parse_tag< TCHAR, WORD > const vk_tags[] = {
			{ "tab",		VK_TAB },
			{ "break",		VK_CANCEL },
			{ "return",		VK_RETURN },
			{ "enter",		VK_RETURN },
			{ "f1",			VK_F1 },
			{ "f2",			VK_F2 },
			{ "f3",			VK_F3 },
			{ "f4",			VK_F4 },
			{ "f5",			VK_F5 },
			{ "f6",			VK_F6 },
			{ "f7",			VK_F7 },
			{ "f8",			VK_F8 },
			{ "f9",			VK_F9 },
			{ "f10",		VK_F10 }
		};
		static size_t const vk_tags_count = sizeof( vk_tags ) / sizeof( vk_tags[0] );
		//
		BOOST_FOREACH( opt2cmd_t const& tag, tags )
		{
			std::string s = this->get< std::string >( tag.opt );
			atom::parse_result< TCHAR, BYTE > mods = atom::parse_tags( s, accel_tags, accel_tags_count, std::string( "+" ) );
			//
			if ( ( mods.total_found > 1 ) && ( mods.unparsed.size() == 1 ) ) {
				atom::parse_result< TCHAR, WORD > vk = atom::parse_tags( s, vk_tags, vk_tags_count, mods.unparsed );
				if ( ( vk.total_found = 1 ) && ( vk.unparsed.size() == 0 ) ) {
				} else {
					vk.result = (WORD)mods.unparsed[0].c_str()[0];
				}
				accel.add_accel( tag.cmd, mods.result | FVIRTKEY, vk.result );

			} else {
				// throw exception
				this->getLogger() << "Invalid accelerator format format " << s << std::endl;
			}
		}
	}
protected:
	//
	logger& getLogger() {
		return ( *( get_value( boost::mpl::identity< pref2logger >() ).item() ) );
	}
	//
	void id2gr( atom::po::id_t const id, pref_group_t const g ) {
		this->prefGroups.insert( std::pair<atom::po::id_t, pref_group_t >( id, g ) );
	}

private:
	///
	typedef std::map< atom::po::id_t, pref_group_t >
		prefGroupsMap;
	typedef std::map< pref_group_t, std::pair< bool, callback_t > >
		processCallbacksMap;
	///
	pref( logger_ptr l );
	///
	atom::po
		po;
	///
	prefGroupsMap
		prefGroups;
	///
	processCallbacksMap
		processCallbacks;
};