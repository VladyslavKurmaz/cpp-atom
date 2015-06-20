#pragma once

namespace dev {
	class logger :
		public atom::log,
		public boost::enable_shared_from_this < logger >
	{
	public:
		///
		static logger_ptr create() {
			return logger_ptr(new logger);
		}

		~logger() {
		}


	private:
		///
		logger() : atom::log() {
		}
	};
}