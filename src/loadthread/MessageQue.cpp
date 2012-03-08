/*
 wdb-libwdbload

 Copyright (C) 2012 met.no

 Contact information:
 Norwegian Meteorological Institute
 Box 43 Blindern
 0313 OSLO
 NORWAY
 E-mail: post@met.no

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 MA  02110-1301, USA
 */

#include "MessageQue.h"

namespace wdb
{
namespace load
{
namespace internal
{

MessageQue::MessageQue() :
		done_(false)
{
}

MessageQue::~MessageQue()
{
}

void MessageQue::put(const std::string & msg)
{
	{
		boost::lock_guard<boost::mutex> lock(mutex_);
		messages_.push_back(msg);
	}
	condition_.notify_one();
}

std::string MessageQue::get()
{
	boost::unique_lock<boost::mutex> lock(mutex_);
	while ( messages_.empty() )
	{
		if ( done_ )
			return std::string();
		condition_.wait(lock);
	}
	std::string ret = messages_.front();
	messages_.pop_front();
	return ret;
}

void MessageQue::done()
{
	done_ = true;
	condition_.notify_all();
}


}
}
}
