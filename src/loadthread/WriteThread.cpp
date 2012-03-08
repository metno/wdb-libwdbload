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

#include "WriteThread.h"
#include "CopyData.h"
#include "CopyStatementConverter.h"
#include <LoaderConfiguration.h>
#include <boost/thread.hpp>
#include <boost/foreach.hpp>

namespace wdb
{
namespace load
{
namespace internal
{

WriteThread::WriteThread(const LoaderConfiguration & config) :
		config_(config)
{
	std::string connect = config_.database().pqDatabaseConnection();
	connection_ = PQconnectdb(connect.c_str());
	if ( ! connection_ )
		throw std::runtime_error("Unable to connect to database");

	thread_ = new boost::thread(CopyData(connection_, que_));

}

WriteThread::~WriteThread()
{
	que_.done();
	thread_->join();
	delete thread_;
	PQfinish(connection_);
}

void WriteThread::process(pqxx::work & transaction, const std::vector<FloatDataEntry> & points)
{
	CopyStatementConverter converter(transaction);
	BOOST_FOREACH(const FloatDataEntry & entry, points)
		que_.put(converter.get(entry));
}

}
}
}
