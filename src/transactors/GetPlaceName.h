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


#ifndef GETPLACENAME_H_
#define GETPLACENAME_H_

/**
 * @addtogroup loader
 * @{
 * @addtogroup libwdbload
 * @{
 */

/**
 * @file
 * Definition and implementation of the GetPlaceName transactor
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
 * Transactor to read the name of a place definition. The transactor returns the Place Name of
 * the Place definition defined by the input parameters. If no place id is found, the transactor
 * throws a wdb::empty_result.
 */
class GetPlaceName : public pqxx::transactor<>
{
public:
	/**
	 * Default constructor.
	 * @param	ret			Reference to be returned to the calling instance
	 * @param	numX		Number of X
	 * @param	numY		Number of Y
	 * @param	incX		Increment X
	 * @param	incY		Increment Y
	 * @param	startX		Start X
	 * @param	startY		Start Y
	 * @param	proj		Projection
	 */
	GetPlaceName( std::string & ret,
				  int xN, int yN,
				  float xI, float yI,
				  float xStart, float yStart,
				  const std::string & proj ) :
    	pqxx::transactor<>("GetPlaceName"),
    	return_(ret),
    	xNumber_(xN),
    	yNumber_(yN),
		xIncrement_(xI),
		yIncrement_(yI),
		startX_(xStart),
		startY_(yStart),
		proj_(proj)
    {
    	// NOOP
    }

	/**
	 * Functor. The transactors functor executes the query.
	 */
	void operator()(argument_type &T)
  	{
		R_ = T.prepared("GetPlaceName")
					   (xNumber_)
					   (yNumber_)
					   (xIncrement_)
					   (yIncrement_)
					   (startX_)
					   (startY_)
					   (proj_).exec();
	}

	/**
	 * Commit handler. This is called if the transaction succeeds.
	 */
  	void on_commit()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.load.getplacename" );
  		if ( R_.size() == 1 ) {
  			if ( R_.at(0).at(0).is_null() ) {
  				log.debugStream() << "Did not find any placename matching the definition ("
								  << xNumber_ << ", "
								  << yNumber_ << ", "
								  << xIncrement_ << ", "
								  << yIncrement_ << ", "
								  << startX_ << ", "
								  << startY_ << ", "
								  << '\'' << proj_ << "')";
  				std::stringstream def;
				def << "No match for: "
					<< xNumber_ << ", "
				    << yNumber_ << ", "
				    << xIncrement_ << ", "
				    << yIncrement_ << ", "
				    << startX_ << ", "
				    << startY_ << ", "
				    << '\'' << proj_;
  				throw wdb::empty_result( "Failed to identify a placename. " + def.str() );
			}
			else {
	  			R_.at(0).at(2).to( return_ );
	  		}
  		}
  		else {
  			// Should always return 1 row
        	log.errorStream() << "Transaction " << Name() << " returned " << R_.size() << " rows.";
			throw wdb::wci_error("Error in wci.getplacename()");
  		}
  	}

	/**
	 * Abort handler. This is called if the transaction fails.
	 * @param	Reason	The reason for the abort of the call
	 */
  	void on_abort(const char Reason[]) throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.load.getplacename" );
		log.errorStream() << "Transaction " << Name() << " failed. " << Reason;
  	}

	/**
	 * Special error handler. This is called if the transaction fails with an unexpected error.
	 * Read the libpqxx documentation on transactors for details.
	 */
  	void on_doubt() throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.load.getplacename" );
		log.errorStream() << "Transaction " << Name() << " in indeterminate state.";
  	}

private:
	/// The reference used to store the result returned to the calling class
	std::string & return_;
	/// The result returned by the query
    pqxx::result R_;
	/// Number of points on the i axis
    long int xNumber_;
    /// Number of points on the j axis
    long int yNumber_;
    /// Increments on the i axis
    float xIncrement_;
    /// Increments on the j acis
    float yIncrement_;
    /// Starting longitude
    float startX_;
    /// Starting latitude
    float startY_;
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

#endif /*GETPLACENAME_H_*/
