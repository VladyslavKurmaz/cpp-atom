#pragma once

namespace dev {

	class appl :
		public atom::node< LOKI_TYPELIST_2( appl2logger, appl2entity ) >,
		public boost::enable_shared_from_this< appl > {

		typedef atom::node< LOKI_TYPELIST_2( appl2logger, appl2entity ) > base_node_t;

	public:
		///
		static appl_ptr create( logger_ptr l ) {
			return appl_ptr( new appl( l ) );
		}
		///
		~appl();
		///
		bool init( int argc, char const * const argv[] );
		///
		void run( std::ostream& os, std::istream& is );
		///
		void clear();

	protected:
		LOGGER_ACCESSOR( appl2logger );

	private:
		///
		appl( logger_ptr l );
	};
}