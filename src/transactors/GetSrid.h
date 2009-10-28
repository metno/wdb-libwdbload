/*
    wdb - weather and water data storage

    Copyright (C) 2007-2009 met.no

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


#ifndef GETSRID_H_
#define GETSRID_H_

/**
 * @addtogroup loader
 * @{
 * @addtogroup libwdbload
 * @{
 */

/**
 * @file
 * Definition and implementation of the GetSrid transactor
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
 * Transactor to identify the Spatial Reference Id connected to a given PROJ definition.
 * If the SRID is not identified, an wdb::empty_result exception is thrown.
 */
class GetSrid : public pqxx::transactor<>
{
public:
	/**
	 * Default constructor.
	 * @param	ret			return value (numerical srid)
	 * @param	proj		Descriptive PROJ string (srid)
	 */
	GetSrid( int & ret, const std::string proj ) :
    	pqxx::transactor<>("ReadSrid"),
    	return_(ret),
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
		query << "SELECT wci.getsridfromproj('" << proj_ << "')";
		R_ = T.exec( query.str() );
	}

	/**
	 * Commit handler. This is called if the transaction succeeds.
	 */
  	void on_commit()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.load.getsrid" );
  		if ( R_.size() == 1 ) {
  			// A row is returned
			if ( R_.at(0).at(0).is_null() ) {
				// NULL returned means we did not manage to identify the SRID
				log.debugStream() << "Failed to identify the SRID of the PROJ.4 string " << proj_;
				throw wdb::empty_result( "Could not identify the SRID" );
			}
			else {
				R_.at(0).at(0).to( return_ );
	 			log.debugStream() << "Identified the SRID as: " << return_;
			}
  		}
  		if ( R_.size() != 1 ) {
  			// Technically, it should be impossible for this to happen
  	        throw std::logic_error( "Transactor GetSrid did not return any rows. This should not be possible" );
  		}
  	}

	/**
	 * Abort handler. This is called if the transaction fails.
	 * @param	Reason	The reason for the abort of the call
	 */
  	void on_abort(const char Reason[]) throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.load.getsrid" );
		log.errorStream() << "Transaction " << Name() << " failed "
				  		  << Reason;
  	}

	/**
	 * Special error handler. This is called if the transaction fails with an unexpected error.
	 * Read the libpqxx documentation on transactors for details.
	 */
  	void on_doubt() throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.load.getsrid" );
		log.errorStream() << "Transaction " << Name() << " in indeterminate state";
  	}

private:
	/// SRID to be returned
	int & return_;
	/// The result returned by the query
    pqxx::result R_;
	/// PROJ string
	std::string proj_;

};

} // namespace load

} // namespace wdb

/**
 * @}
 *
 * @}
 */

#endif /* GETSRID_H_ */
