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

#ifndef WRITETREAD_H_
#define WRITETREAD_H_

#include "MessageQue.h"
#include <FloatDataEntry.h>
#include <pqxx/transaction>
#include <boost/thread.hpp>
#include <libpq-fe.h>
#include <vector>

namespace wdb
{
namespace load
{
class LoaderConfiguration;
namespace internal
{

class WriteThread
{
public:
	explicit WriteThread(const LoaderConfiguration & config);
	~WriteThread();

	void process(pqxx::work & transaction, const std::vector<FloatDataEntry> & points);

private:
	const LoaderConfiguration & config_;
	PGconn * connection_;
 	MessageQue que_;
 	boost::thread * thread_;

};

} /* namespace internal */
} /* namespace load */
} /* namespace wdb */
#endif /* WRITETREAD_H_ */
