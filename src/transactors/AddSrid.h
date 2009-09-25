/*
    wdb - weather and water data storage

    Copyright (C) 2009 met.no

    Contact information:
    Norwegian Meteorological Institute
    Box 43 Blindern
    0313 OSLO
    NORWAY
    E-mail: wdb@met.no

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


#ifndef ADDSRID_H_
#define ADDSRID_H_

/**
 * @addtogroup loader
 * @{
 * @addtogroup libwdbload
 * @{
 */

/**
 * @file
 * Definition and implementation of the AddSrid transactor
 */

// PROJECT INCLUDES
#include <wdbException.h>
#include <wdbLogHandler.h>

// SYSTEM INCLUDES
#include <pqxx/transactor>
#include <pqxx/result>
#include <iostream>
#include <string>

// FORWARD REFERENCES
//

namespace wdb {

namespace load {

/**
 * Transactor to add an srid to the database. The PL/PGSQL function
 * raises an error if it runs into any problems, which will cause pqxx to
 * throw and exception
 */
class AddSrid : public pqxx::transactor<>
{
public:
	/**
	 * Default constructor.
	 * @param	sridname	Placename of the definition inserted
	 * @param	proj		Projection
	 */
	AddSrid( const std::string & sridname,
			 const std::string & proj ) :
    	pqxx::transactor<>("AddSrid"),
    	sridName_(sridname),
		proj_(proj)
    {
    	// NOOP
    }

	/**
	 * Functor. The transactors functor executes the query.
	 */
	void operator()(argument_type &T)
  	{
		std::ostringstream query;
		query << "SELECT wci.addsrid( "
			  << "'" << sridName_ << "', "
			  << "'" << proj_ << "')";
		pqxx::result R = T.exec( query.str() );
	}

	/**
	 * Commit handler. This is called if the transaction succeeds.
	 */
  	void on_commit()
  	{
  		// NOOP
  	}

	/**
	 * Abort handler. This is called if the transaction fails.
	 * @param	Reason	The reason for the abort of the call
	 */
  	void on_abort(const char Reason[]) throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.load.addsrid" );
		log.errorStream() << "Transaction " << Name() << " failed. " << Reason;
  	}

	/**
	 * Special error handler. This is called if the transaction fails with an unexpected error.
	 * Read the libpqxx documentation on transactors for details.
	 */
  	void on_doubt() throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.load.addsrid" );
		log.errorStream() << "Transaction " << Name() << " in indeterminate state.";
  	}

private:
	/// The sridname to be inserted
	std::string sridName_;
	/// The SRID of the object
	std::string proj_;
};

} // namespace load

} // namespace wdb

/**
 * @}
 *
 * @}
 */

#endif /* ADDSRID_H_ */
