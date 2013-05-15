/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2013.05.10 - initial structure                                 |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_ZOOM_LOGGER_HPP
#define ATOM_ZOOM_LOGGER_HPP
#pragma once

#include <atom/util/log.hpp>

namespace atom { namespace zoom {

	class logger :
		public atom::log,
		public boost::enable_shared_from_this< logger >
	{
	public:
		///
		static logger_ptr create() {
			return logger_ptr( new logger() );
		}

		~logger() {
		}

	private:
		///
		logger() : atom::log() {
		}
	};
} }

#endif//ATOM_ZOOM_LOGGER_HPP

