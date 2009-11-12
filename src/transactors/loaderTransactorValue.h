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


#ifndef LOADERTRANSACTORVALUE_H_
#define LOADERTRANSACTORVALUE_H_

/**
 * @addtogroup loadingprogram
 * @{
 * @addtogroup loaderBase
 * @{
 */

/**
 * @file
 * Definition and implementation of value loading transactors used in the loader applications.
 */

// PROJECT INCLUDES
#include <wdbException.h>
#include <wdbLogHandler.h>

// SYSTEM INCLUDES

#include "libpq-fe.h"

#include <pqxx/transactor>
#include <pqxx/result>
#include <pqxx/largeobject>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

// FORWARD REFERENCES
//


namespace wdb {

namespace load {

/**
 * Transactor to write the contents of a value. If the transaction fails, it logs the error and
 * throws a wdbException.
 */
class WriteValue : public pqxx::transactor<>
{
public:
	/**
	 * Default constructor.
	 * @param	dataProv	Query parameter
	 * @param	place		Query parameter
	 * @param	refTime		Query parameter
	 * @param	valTF		Query parameter
	 * @param	valTT		Query parameter
	 * @param	valTIC		Query parameter
	 * @param	valParam	Query parameter
	 * @param	levels		Set of levels detected for the value
	 * @param	dataVer		Query parameter
	 * @param	conf		Query parameter
	 * @param	values		Query parameter
	 * @param	valuesSize	Query parameter
	 */
	WriteValue(long int dataProv,
			   long int place,
			   const std::string refTime,
			   const std::string valTF,
			   const std::string valTT,
			   int valTIC,
			   int valParam,
			   const std::vector <wdb::load::WdbLevel> & levels,
			   int dataVer,
			   int conf,
			   const double * values,
			   int valuesSize ) :
    	pqxx::transactor<>("WriteValue"),
    	dataProvider_(dataProv),
    	placeId_(place),
    	referenceTime_(refTime),
    	validTimeFrom_(valTF),
    	validTimeTo_(valTT),
    	validTimeIC_(valTIC),
    	valueParameter_(valParam),
    	levels_(levels),
    	dataVersion_(dataVer),
    	confidenceCode_(conf),
    	values_(values),
    	noOfValues_(valuesSize)
    {
    	// NOOP
    }

	/**
	 * Functor. The transactors functor executes the query.
	 */
	void operator()(argument_type &T)
  	{
		//WDB_LOG & log = WDB_LOG::getInstance( "wdb.loaderBase.value" );
		const std::vector<float> data(values_, values_ + noOfValues_);
		const char * rawData = reinterpret_cast<const char *>(& data[0]);
		size_t binarySize = noOfValues_ * sizeof(float) / sizeof(char);
		const std::string binaryData(rawData, binarySize);
		// Write
		for ( std::vector <wdb::load::WdbLevel>::const_iterator level = levels_.begin(); level != levels_.end(); ++ level )
		{
			// Write Value
			R = T.prepared("WriteWCI")
						  (binaryData)
						  (dataProvider_)
						  (placeId_)
						  (referenceTime_)
						  (validTimeFrom_)
	 					  (validTimeTo_)
	 					  (validTimeIC_)
						  (valueParameter_)
						  (level->levelParameter_)
						  (level->levelFrom_)
						  (level->levelTo_)
						  (level->levelIndeterminate_)
						  (dataVersion_)
						  (confidenceCode_).exec();
		}
  	}

	/**
	 * Commit handler. This is called if the transaction succeeds.
	 */
  	void on_commit()
  	{
  		// Log
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.loaderBase.value" );
		log.infoStream() << "Inserted Value in database";

  	}

	/**
	 * Abort handler. This is called if the transaction fails.
	 * @param	Reason	The reason for the abort of the call
	 */
  	void on_abort(const char Reason[]) throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.loaderBase.value" );
		log.warnStream()  << "Transaction " << Name() << " failed while trying to write: ";
		for ( std::vector <wdb::load::WdbLevel>::const_iterator level = levels_.begin(); level != levels_.end(); ++ level )
		{
			log.warnStream()  << " ( DP: " << (dataProvider_)
							  << " PID: " << (placeId_)
							  << " REFT: " << (referenceTime_)
							  << " VTFR: " << (validTimeFrom_)
							  << " VTTO: " << (validTimeTo_)
							  << " VPAR: " << (valueParameter_)
							  << " LPAR: " << (level->levelParameter_)
							  << " LFR: " << (level->levelFrom_)
							  << " LTO: " << (level->levelTo_)
							  << " DVER: " << (dataVersion_)
							  << " CONF: " << (confidenceCode_)
							  << " ) ";
		}
  	}

	/**
	 * Special error handler. This is called if the transaction fails with an unexpected error.
	 * Read the libpqxx documentation on transactors for details.
	 */
  	void on_doubt() throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.loaderBase.value" );
		log.warnStream() << "Transaction " << Name() << " in indeterminate state";
  	}

private:
	/// DataProviderId
    long int dataProvider_;
    /// Place Definition ID of Value
    long int placeId_;
    /// Reference Time of Value
    std::string referenceTime_;
    /// Valid Time From
    std::string validTimeFrom_;
    /// Valid Time To
    std::string validTimeTo_;
    /// Codespace ID of Reference Domain
    int validTimeIC_;
    /// Codespace ID of Reference Domain
    int valueParameter_;
    /// Level measurements for the parameter
    const std::vector <wdb::load::WdbLevel> & levels_;
    /// Data Version of Value
    int dataVersion_;
    /// Confidence Code
    int confidenceCode_;
    /// Values
	const double * values_;
	/// Number of Values
	int noOfValues_;
	/// Result
   	pqxx::result R;

};



} // namespace database

} // namespace wdb

/**
 * @}
 *
 * @}
 */

#endif /*LOADERTRANSACTORVALUE_H_*/
