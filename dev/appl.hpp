#pragma once

namespace dev {

	class appl :
		public atom::node< LOKI_TYPELIST_2(appl2logger, appl2comp) >,
		public boost::enable_shared_from_this < appl > {

		typedef atom::node< LOKI_TYPELIST_2(appl2logger, appl2comp) > base_node_t;

	public:
		///
		static appl_ptr create(logger_ptr l) {
			return appl_ptr(new appl(l));
		}
		///
		~appl();
		///
		bool init(int argc, char const * const argv[], std::ostream& os);
		///
		void run(std::ostream& os, std::istream& is);
		///
		void clear();

	protected:
		LOGGER_ACCESSOR(appl2logger);
		//
		comp_ptr getRoot() { return (this->get_value(boost::mpl::identity< appl2comp >()).item()); }
		//
		void printError(atom::po::options_description_t const& desc, std::exception& exc);
		//
		bool processCommand(std::ostream& os);

	private:
		///
		appl(logger_ptr l);
		//
		atom::po
			po;
		//
		comp_ptr
			cursor;
	};
}