#include "./pch.hpp"
#include "./classes.hpp"
#include "./langs.hpp"

langs::langs(boost::property_tree::ptree const& c, logger_ptr l) : config(c), langsPair(), languages(){
    atom::mount<langs2logger>( this, l );
	//
	// configure available languages
	lang_t const en = boost::make_tuple(_T("eng"), _T("en"), _T("English"), AD_PANEL_IMAGE_LANG_EN);
	lang_t const ua = boost::make_tuple(_T("ukr"), _T("uk"), _T("Український"), AD_PANEL_IMAGE_LANG_UA);
	lang_t const ru = boost::make_tuple(_T("rus"), _T("ru"), _T("Русский"), AD_PANEL_IMAGE_LANG_RU);

	this->languages.push_back(en);
	this->languages.push_back(ua);
	this->languages.push_back(ru);
	//
	this->setPair(std::make_pair(en, ru));

#if 0


	boost::property_tree::ptree const& tr = this->config.get_child(CONFIG_AD_TRANSLATE);
	size_t langCnt = langs.size();
	size_t const sz = 32;
	//
	dcb_t dcb;
	dcb.updateDC(sz * langCnt, sz);
	RECT rt;
	SetRect(&rt, 0, 0, sz * langCnt, sz);
	FillRect(dcb.dc, &rt, reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));
	SetBkMode(dcb.dc, TRANSPARENT);
	//
	SetRect(&rt, 0, 0, sz, sz);
	BOOST_FOREACH(boost::property_tree::ptree::value_type const & l, langs) {
		std::string name = l.second.get_child(CONFIG_AD_LANGUAGE_NAME).data();
		std::string c2 = l.second.get_child(CONFIG_AD_LANGUAGE_CODE2).data();
		std::string c3 = l.second.get_child(CONFIG_AD_LANGUAGE_CODE3).data();

		boost::to_upper(c2);
		DrawTextA(dcb.dc, c2.c_str(), c2.length(), &rt, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		OffsetRect(&rt, sz, 0);
	}
	bitmapSave(_T("lang.bmp"), dcb.dc, dcb.bitmap);


#endif


}

langs::~langs() {
}

void langs::clear() {
  base_t::clear();
}

void langs::setPair(langspair_t const& lngs) {
	this->langsPair = lngs;
}

langspair_t langs::getPair() const {
	return this->langsPair;
}

void langs::setLang(bool const from, size_t const ind) {
	if ((0 <= ind) && (ind < this->languages.size())) {
		lang_t l = this->languages[ind];
		if (from) {
			this->langsPair.first = l;
		}
		else {
			this->langsPair.second = l;
		}
	}
}

size_t langs::count() const{
	return this->languages.size();
}

void langs::foreach(boost::function< bool(lang_t const&) > func) const {
	BOOST_FOREACH(lang_t const& l, languages) {
		if (!func(l)) {
			break;
		}
	}
}
