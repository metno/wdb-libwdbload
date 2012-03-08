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

#include "CopyData.h"
#include "MessageQue.h"
#include <wdbLogHandler.h>
#include <boost/shared_ptr.hpp>


namespace wdb
{
namespace load
{
namespace internal
{

CopyData::CopyData(PGconn * connection, internal::MessageQue & que) :
		connection_(connection), que_(que)
{
}

CopyData::~CopyData()
{
}

void CopyData::operator () ()
{
	if ( PQstatus(connection_) != CONNECTION_OK )
		throw std::runtime_error("Error when connecting to database");

	PGresult * result = PQexec(connection_, "COPY wdb_int.floatvalue FROM STDIN");
	if ( PQresultStatus(result) != PGRES_COPY_IN )
		throw std::runtime_error("Error when performing query");

	for ( std::string msg = que_.get(); not msg.empty(); msg = que_.get() )
	{
		int result = 0;
		while ( ! result )
			result = PQputCopyData(connection_, msg.c_str(), msg.size());
		if ( result == -1 )
			throw std::runtime_error(PQerrorMessage(connection_));
	}
	int copyResult = 0;
	while ( ! copyResult )
		copyResult = PQputCopyEnd(connection_, NULL);
	if ( copyResult < 0 )
		throw std::runtime_error(PQerrorMessage(connection_));
}



} /* namespace internal */
} /* namespace load */
} /* namespace wdb */
