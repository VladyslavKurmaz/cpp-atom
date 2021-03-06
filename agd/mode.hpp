#pragma once

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > mode2logger;
typedef atom::nstorage< pref, boost::shared_ptr, atom::narray1 > mode2pref;
typedef atom::nstorage< window, boost::shared_ptr, atom::narray1 > mode2window;

class mode :
	public atom::node< LOKI_TYPELIST_3( mode2logger, mode2pref, mode2window ) >,
	public boost::enable_shared_from_this< mode > {
public:
	typedef atom::node< LOKI_TYPELIST_3( mode2logger, mode2pref, mode2window ) >
		base_t;
	///
	template < class T >
	static mode_ptr create(boost::property_tree::ptree const& c, logger_ptr l, pref_ptr p, window_ptr w) {
		return mode_ptr( new T( c, l, p, w ) );
	}
	///
	virtual ~mode(){}
	///
	virtual void activate(bool const state) = 0 {
		this->refresh();
	}
	///
	virtual void show(bool const state) = 0 {
		this->refresh();
	}

	///
	virtual bool command( int const id ) = 0;
	//
	virtual void key( KEY_EVENT_RECORD const& k ) = 0;
	//
	virtual void mouselbdown( bool dblclick, int x, int y, unsigned int state ) = 0;
	//
	virtual void mouselbup( int x, int y, unsigned int state ) = 0;
	//
	virtual void mousemove( int x, int y, unsigned int state ) = 0;
	///
	virtual void paint(paint_param_t& paintParam, RECT const& rect) = 0{
		FillRect(paintParam.dcb.dc, &rect, this->bkBrush);
	}
	///
	virtual void clear() = 0;
	///
	COLORREF getBkColor() const {
		return this->bkColor;
	}
	///
	void setBkColor(COLORREF const color) {
		this->bkColor = color;
		this->bkBrush = CreateSolidBrush(this->bkColor);
	}
	///
	unsigned int getAlpha() const {
		return this->alpha;
	}
	///
	void setAlpha(unsigned int const a) {
		this->alpha = a;
		this->refresh();
	}


protected:
	//
	LOGGER_ACCESSOR( mode2logger )
	PREF_ACCESSOR( mode2pref )
	WINDOW_ACCESSOR( mode2window )
	///
	mode(boost::property_tree::ptree const& c, logger_ptr l, pref_ptr p, window_ptr w) : config(c), bkColor(0xFFFFFF), bkBrush(), alpha(0) {
		atom::mount<mode2logger>( this, l );
		atom::mount<mode2pref>( this, p );
		atom::mount<mode2window>( this, w );
		//
		// create background brash and read alpha level from config
		std::string hex = this->config.get_child(CONFIG_BK_COLOR).data();
		std::stringstream converter(hex);
		unsigned int color;
		converter >> std::hex >> color;
		unsigned char* argb = reinterpret_cast<unsigned char*>(&color);
		this->setAlpha(argb[3]);
		this->setBkColor(RGB(argb[2], argb[1], argb[0]));
	}
	///
	void refresh(){
		this->getWindow()->setAlpha(this->alpha).invalidate();
	}
	///
	boost::property_tree::ptree const&
		config;
	COLORREF
		bkColor;
	atom::shared_gdiobj< HBRUSH >
		bkBrush;
	unsigned int
		alpha;

private:
};
