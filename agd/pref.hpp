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
	bool init( int argc, char const * const argv[] );
	///
	void register_process_callback( pref_group_t const g, callback_t c );
	///
	bool parse( atom::string_t const& s );
	///
	template <typename T >
	T get( atom::po::id_t const id ) {
		return ( this->po.as<T>( id ) );
	}
	//
	void getView( RECT& r );
	//
	void calculateDocks( placement& windowPlacement );
	//
	bool parseHotkey( atom::po::id_t const id, hotkey& hk );

	///
	struct opt2cmd_t {
		atom::po::id_t	opt;
		WORD			cmd;
	};
	template < size_t N >
	void parseAccel( opt2cmd_t const (&tags)[N], atom::accel& accel ) {

		static atom::parse_tag< atom::char_t, BYTE > const accel_tags[] = {
			{ _T( "alt" ),			FALT },
			{ _T( "ctrl" ),			FCONTROL },
			{ _T( "shift" ),		FSHIFT }
		};
		static size_t const accel_tags_count = sizeof( accel_tags ) / sizeof( accel_tags[0] );

		static atom::parse_tag< atom::char_t, WORD > const vk_tags[] = {
			{ _T( "tab" ),			VK_TAB },
			{ _T( "break" ),		VK_CANCEL },
			{ _T( "return" ),		VK_RETURN },
			{ _T( "enter" ),		VK_RETURN },
			{ _T( "f1" ),			VK_F1 },
			{ _T( "f2" ),			VK_F2 },
			{ _T( "f3" ),			VK_F3 },
			{ _T( "f4" ),			VK_F4 },
			{ _T( "f5" ),			VK_F5 },
			{ _T( "f6" ),			VK_F6 },
			{ _T( "f7" ),			VK_F7 },
			{ _T( "f8" ),			VK_F8 },
			{ _T( "f9" ),			VK_F9 },
			{ _T( "f10" ),			VK_F10 }
		};
		static size_t const vk_tags_count = sizeof( vk_tags ) / sizeof( vk_tags[0] );
		//
		BOOST_FOREACH( opt2cmd_t const& tag, tags )
		{
			atom::string_t s = boost::lexical_cast<atom::string_t>( this->get< atom::po::string_t >( tag.opt ) );
			atom::parse_result< atom::char_t, BYTE > mods = atom::parse_tags( s, accel_tags, accel_tags_count, atom::string_t( _T( "+" ) ) );
			//
			if ( ( mods.total_found > 1 ) && ( mods.unparsed.size() == 1 ) ) {
				atom::parse_result< atom::char_t, WORD > vk = atom::parse_tags< atom::char_t, WORD >(s, vk_tags, vk_tags_count, mods.unparsed);
				if ( ( vk.total_found = 1 ) && ( vk.unparsed.size() == 0 ) ) {
				} else {
					vk.result = (WORD)mods.unparsed[0].c_str()[0];
				}
				accel.add_accel( tag.cmd, mods.result | FVIRTKEY, vk.result );

			} else {
				// throw exception
				*(this->getLogger()) << "Invalid accelerator format format " << s << std::endl;
			}
		}
	}
	///
	///
	typedef boost::tuple< atom::string_t, atom::string_t, atom::string_t, int >
		lang_t;
	typedef std::pair< lang_t, lang_t >
		langspair_t;
	//
	void langSetPair( langspair_t const& lngs );
	//
	langspair_t langGetPair() const;
	//
	void langSetLang( bool const from, size_t const ind );
	//
	void langEnum( boost::function< bool( lang_t const& ) > func ) const;

protected:
	//
	LOGGER_ACCESSOR( pref2logger )
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
	///
	typedef std::vector< lang_t >
		langs_t;
	//
	langspair_t langsPair;
	//
	langs_t langs;
	///
	///
	prefGroupsMap
		prefGroups;
	///
	processCallbacksMap
		processCallbacks;
};

inline bool operator==( pref::lang_t const& l, pref::lang_t const& r ) {
	return ( l.get<0>() == r.get<0>() );
}
