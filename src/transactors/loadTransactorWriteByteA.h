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


#ifndef LOADERTRANSACTORWRITEBYTEA_H_
#define LOADERTRANSACTORWRITEBYTEA_H_

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

namespace database {

/**
 * Transactor to write a binary value. If the transaction fails,
 * it logs the error and throws a pqxx exception.
 */
class WriteByteA : public pqxx::transactor<>
{
public:
	/**
	 * Default constructor.
     * @param	values				A pointer to the array of values (doubles) in the field
     * @param	noOfValues			The size of the values array
     * @param	dataProviderName	The data provider name of the field
     * @param	placeName			The place name of the fields grid description
     * @param	referenceTime		The reference time of the field
     * @param	validTimeFrom		Valid time from of the field
     * @param	validTimeTo			Valid time to of the field
     * @param	valueParameterName	The WDB name designation of the value parameter
     * @param	levelParameterName	The WDB name designation of the level parameter
     * @param	levelFrom			The lower level bound of the data
     * @param	levelTo				The upper level bound of the data
     * @param	dataVersion			The data version of the field
     * @param	confidenceCode		WDB confidence code to be associated with the field
	 */
	WriteByteA(const double * values,
			   int valuesSize,
			   const std::string dataProviderName,
			   const std::string placeName,
			   const std::string referenceTime,
			   const std::string validTimeFrom,
			   const std::string validTimeTo,
			   const std::string valueParameterName,
			   const std::string levelParameterName,
			   float levelFrom,
			   float levelTo,
			   int dataVersion,
			   int confidenceCode ) :
    	pqxx::transactor<>("WriteValue"),
    	dataProviderName_(dataProviderName),
    	placeName_(placeName),
    	referenceTime_(referenceTime),
    	validTimeFrom_(validTimeFrom),
    	validTimeTo_(validTimeTo),
    	valueParameterName_(valueParameterName),
    	levelParameterName_(levelParameterName),
    	levelFrom_(levelFrom),
    	levelTo_(levelTo),
    	dataVersion_(dataVersion),
    	confidenceCode_(confidenceCode),
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
		const std::vector<float> data(values_, values_ + noOfValues_);
		const char * rawData = reinterpret_cast<const char *>(& data[0]);
		size_t binarySize = noOfValues_ * sizeof(float) / sizeof(char);
		const std::string binaryData(rawData, binarySize);
		// Write
		R = T.prepared("WCIWriteByteA")
					  (binaryData)
					  (dataProviderName_)
					  (placeName_)
					  (referenceTime_)
					  (validTimeFrom_)
					  (validTimeTo_)
					  (valueParameterName_)
					  (levelParameterName_)
					  (levelFrom_)
					  (levelTo_)
					  (dataVersion_)
					  (confidenceCode_).exec();
  	}

	/**
	 * Commit handler. This is called if the transaction succeeds.
	 */
  	void on_commit()
  	{
  		// Log
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.baseload" );
		log.infoStream() << "Value inserted in database";
  	}

	/**
	 * Abort handler. This is called if the transaction fails.
	 * @param	Reason	The reason for the abort of the call
	 */
  	void on_abort(const char Reason[]) throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.baseload" );
		log.warnStream()  << "Transaction " << Name()
						  << " failed while trying wci.write ( "
						  << " binary-data, "
						  << dataProviderName_ << ", "
						  << placeName_ << ", "
						  << referenceTime_ << ", "
						  << validTimeFrom_ << ", "
						  << validTimeTo_ << ", "
						  << valueParameterName_ << ", "
						  << levelParameterName_ << ", "
						  << levelFrom_ << ", "
						  << levelTo_ << ", "
						  << dataVersion_ << ", "
						  << confidenceCode_ << ")";
  	}

	/**
	 * Special error handler. This is called if the transaction fails with an unexpected error.
	 * Read the libpqxx documentation on transactors for details.
	 */
  	void on_doubt() throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.baseload" );
		log.warnStream() << "Transaction " << Name() << " in indeterminate state";
  	}

private:
	/// DataProvider
    std::string dataProviderName_;
    /// Place Definition of Value
    std::string placeName_;
    /// Reference Time of Value
    std::string referenceTime_;
    /// Valid Time From
    std::string validTimeFrom_;
    /// Valid Time To
    std::string validTimeTo_;
    /// Value Parameter
    std::string valueParameterName_;
    /// Level Parameter
    std::string levelParameterName_;
    /// Level from
    float levelFrom_;
    /// Level to
    float levelTo_;
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

#endif /*LOADERTRANSACTORWRITEBYTEA_H_*/
