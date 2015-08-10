#include "./pch.hpp"
#include "./classes.hpp"
#include "./langs.hpp"

langs::langs(boost::property_tree::ptree const& c, logger_ptr l) : config(c), langsPair(), languages(){
    atom::mount<langs2logger>( this, l );
	//
	std::string lfName = this->config.get_child(CONFIG_AD_LANGUAGE_FROM).data();
	std::string ltName = this->config.get_child(CONFIG_AD_LANGUAGE_TO).data();
	boost::property_tree::ptree const& lngs = this->config.get_child(CONFIG_AD_LANGUAGE);
	//
	unsigned int command = AD_PANEL_TB_LANGUAGE;
	int img = AD_PANEL_IMAGE_LANG_FIRST;

	language_t lf;
	language_t lt;
	BOOST_FOREACH(boost::property_tree::ptree::value_type const & l, lngs) {
		language_t nl;

		std::string name = l.second.get_child(CONFIG_AD_LANGUAGE_NAME).data();
		std::string c2 = l.second.get_child(CONFIG_AD_LANGUAGE_CODE2).data();
		std::string c3 = l.second.get_child(CONFIG_AD_LANGUAGE_CODE3).data();

		nl.enable = false;
		nl.name = boost::lexical_cast<atom::string_t>(name);
		nl.c2 = boost::lexical_cast<atom::string_t>(c2);
		nl.c3 = boost::lexical_cast<atom::string_t>(c3);
		nl.command = command++;
		nl.img = img++;

		this->languages.push_back(nl);

		if (lfName == c2){
			lf = nl;
		}
		if (ltName == c2){
			lt = nl;
		}
	}
	//
	this->update();
	this->setPair(std::make_pair(lf, lt));


	////
	//// configure available languages
	//lang_t const en = boost::make_tuple(_T("eng"), _T("en"), _T("English"), AD_PANEL_IMAGE_LANG_EN);
	//lang_t const ua = boost::make_tuple(_T("ukr"), _T("uk"), _T("Український"), AD_PANEL_IMAGE_LANG_UA);
	//lang_t const ru = boost::make_tuple(_T("rus"), _T("ru"), _T("Русский"), AD_PANEL_IMAGE_LANG_RU);

	//this->languages.push_back(en);
	//this->languages.push_back(ua);
	//this->languages.push_back(ru);
	////
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
		language_t l = this->languages[ind];
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

void langs::foreach(boost::function< bool(language_t const&) > func) const {
	BOOST_FOREACH(language_t const& l, languages) {
		if (!func(l)) {
			break;
		}
	}
}

void langs::update(){
	// scan available languages
	char ts[MAX_PATH] = { 0 };
	GetEnvironmentVariableA(ENV_TESSDATA_PREFIX, ts, MAX_PATH);
	boost::filesystem::path tsData(ts);
	tsData /= boost::filesystem::path("tessdata");

	BOOST_FOREACH(language_t& l, this->languages) {
		boost::filesystem::path ts_file = tsData / boost::filesystem::path(l.c3 + atom::string_t(_T(".traineddata")));
		l.enable = boost::filesystem::exists(ts_file);
	}
}

