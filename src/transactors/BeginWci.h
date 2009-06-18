/*
    wdb - weather and water data storage

    Copyright (C) 2007 met.no

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


#ifndef BEGINWCI_H_
#define BEGINWCI_H_

/**
 * @addtogroup loader
 * @{
 * @addtogroup libwdbload
 * @{
 */

/**
 * @file
 * Definition and implementation of the Begin WCI Transactor
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
 * Transactor to Begin the WCI
 */
class BeginWci : public pqxx::transactor<>
{
public:
	/**
	 * Default constructor.
	 * @param	mode 		loadmode
	 */
	BeginWci( const std::string & wciUser, int mode ) :
    	pqxx::transactor<>("BeginWci"),
    	wciUser_(wciUser), mode_(mode)
    {
    	// NOOP
    }

	/**
	 * Functor. The transactors functor executes the query.
	 */
	void operator()(argument_type &T)
  	{
		std::ostringstream query;
		query << "SELECT wci.begin('" << wciUser_ << "'";
		for ( int i = 0; i < 3; ++ i)
			query << ", " << mode_;
		query << ')';
		pqxx::result R = T.exec(query.str());
	}

	/**
	 * Commit handler. This is called if the transaction succeeds.
	 */
  	void on_commit()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.load.beginwci" );
		log.infoStream() << "wci.begin call complete";
  	}

	/**
	 * Abort handler. This is called if the transaction fails.
	 * @param	Reason	The reason for the abort of the call
	 */
  	void on_abort(const char Reason[]) throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.load.beginwci" );
		log.errorStream() << "Transaction " << Name() << " failed "
				  		  << Reason;
  	}

	/**
	 * Special error handler. This is called if the transaction fails with an unexpected error.
	 * Read the libpqxx documentation on transactors for details.
	 */
  	void on_doubt() throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.load.beginwci" );
		log.errorStream() << "Transaction " << Name() << " in indeterminate state";
  	}

private:
	// wci user for wci.begin call
	std::string wciUser_;
	/// Parameter ID
	int mode_;
};

} // namespace load

} // namespace wdb

/**
 * @}
 *
 * @}
 */

#endif /*BEGINWCI_H_*/
