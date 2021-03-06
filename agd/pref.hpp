#pragma once
#include "./classes.hpp"

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > pref2logger;



class pref :
	public atom::node< LOKI_TYPELIST_1( pref2logger ) >,
	public boost::enable_shared_from_this< pref > {
	typedef atom::node< LOKI_TYPELIST_1( pref2logger ) >
		base_t;
public:
	///
	static pref_ptr create( logger_ptr l ) {
		return pref_ptr( new pref( l ) );
	}
	///
	~pref();
	///
	bool init( int argc, char const * const argv[] );
	///
	template <typename T>
	struct conv{
		T get(boost::property_tree::ptree const& c, std::string const& key){
			return boost::lexical_cast<T>(c.get_child(key).data());
		}
	};
	template <>
	struct conv<bool>{
		bool get(boost::property_tree::ptree const& c, std::string const& key){
			std::string s = c.get_child(key).data();
			if ((s == "true") || (s == "1")){
				return true;
			}
			return false;
		}
	};
	template <typename T >
	T get(std::string const& key) {
		return conv<T>().get(this->config, key);
	}
	template <typename T >
	void put(std::string const& key, T const& v) {
		this->config.put(key, v);
	}
	//
	boost::property_tree::ptree const& getModeConfig(std::string const& key);
	//
	void getView(bool const clip, RECT& r);
	//
	alignment_t::type getAlignment();
	//
	void calculateDocks(unsigned int const alpha, placement_t& windowPlacement);
	//
	bool parseHotkey(std::string const& id, hotkey& hk);
	//
	bool translateAccel(HWND hWnd, MSG* msg);
	//
	enum popupMenuType{
		pmtTransparency,
		pmtAlignment,
		pmtVRatio,
		pmtHRation
	};
	///
	template < typename T, size_t N >
	void parseAccel( T const (&tags)[N], atom::accel& accel ) {

		static atom::parse_tag< char, BYTE > const accel_tags[] = {
			{ "alt",		FALT },
			{ "ctrl",		FCONTROL },
			{ "shift",		FSHIFT }
		};
		static size_t const accel_tags_count = sizeof( accel_tags ) / sizeof( accel_tags[0] );

		static atom::parse_tag< char, WORD > const vk_tags[] = {
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
		BOOST_FOREACH( T const& tag, tags )
		{
			std::string s = this->get< std::string >( tag.id );
			atom::parse_result< char, BYTE > mods = atom::parse_tags( s, accel_tags, accel_tags_count, std::string("+") );
			//
			if ( ( mods.total_found > 1 ) && ( mods.unparsed.size() == 1 ) ) {
				atom::parse_result< char, WORD > vk = atom::parse_tags< char, WORD >(s, vk_tags, vk_tags_count, mods.unparsed);
				if ( ( vk.total_found = 1 ) && ( vk.unparsed.size() == 0 ) ) {
				} else {
					vk.result = (WORD)mods.unparsed[0].c_str()[0];
				}
				accel.add_accel( tag.command, mods.result | FVIRTKEY, vk.result );

			} else {
				// throw exception
				//*(this->getLogger()) << "Invalid accelerator format format " << s << std::endl;
			}
		}
	}

protected:
	LOGGER_ACCESSOR( pref2logger )

private:
	///
	pref( logger_ptr l );
	///
	boost::filesystem::path
		configFileName;
	boost::property_tree::ptree
		config;
	//
	atom::accel
		accel;
};
