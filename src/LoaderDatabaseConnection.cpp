/*
 wdb

 Copyright (C) 2008 met.no

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

#include "LoaderDatabaseConnection.h"
#include "transactors/loadTransactorWriteByteA.h"
#include "transactors/loaderTransactorValue.h"
#include "transactors/loaderTransactorWci.h"
#include "transactors/loaderTransactorPlaceDefinition.h"
#include "transactors/loaderTransactorSrid.h"
#include "transactors/loaderTransactorUnit.h"
#include <wdbEmptyResultException.h>
#include <pqxx/pqxx>
#include <stdexcept>

using namespace std;
using namespace pqxx;
using namespace pqxx::prepare;

namespace wdb
{
namespace database
{

LoaderDatabaseConnection::LoaderDatabaseConnection(const std::string & target, const std::string & wciUser)
	: pqxx::connection( target.c_str() )
{
    // Initialize WCI
    perform ( BeginWci( wciUser, 0 ), 1 );

    // Statement Get PlaceId
    prepare("ReadPlaceXref",
            "SELECT * FROM loaderBase.getplaceid ($1, $2, $3, $4, $5, $6, $7, $8, $9)" )
           ("varchar", treat_direct )
		   ("int4", treat_direct )
		   ("int4", treat_direct )
		   ("int4", treat_direct )
		   ("real", treat_direct )
		   ("real", treat_direct )
		   ("real", treat_direct )
		   ("real", treat_direct )
		   ("int4", treat_direct );

    // Statement getSrid
    prepare("ReadSrid",
            "SELECT loaderBase.getSrid( $1 )" )
           ("int4", treat_direct );

    // Statement insertSrid
    prepare("WriteSrid",
            "SELECT loaderBase.setSrid ($1, $2, $3)" )
           ("int4", treat_direct )
           ("varchar", treat_direct )
           ("varchar", treat_direct );

    // Statement insertSrid
    prepare("ReadUnitData",
            "SELECT * FROM loaderBase.unitconversion ( $1 )" )
           ("varchar", treat_direct );

    // Statement Insert value
    prepare("WriteWCI",
    		"select "
            "wci.write ("
            "$1::bytea,"
    		"$2::bigint,"
    		"$3::bigint,"
    		"$4::timestamp with time zone,"
    		"$5::timestamp with time zone,"
    		"$6::timestamp with time zone,"
    		"$7::integer,"
    		"$8::integer,"
    		"$9::integer,"
    		"$10::real,"
    		"$11::real,"
    		"$12::integer,"
    		"$13::integer,"
            "$14::integer"
    		")" )
            ("bytea", treat_binary )
            ("int8", treat_direct )
            ("int8", treat_direct )
            ("varchar", treat_direct )
			("varchar", treat_direct )
			("varchar", treat_direct )
			("int4", treat_direct )
			("int4", treat_direct )
            ("int4", treat_direct )
            ("real", treat_direct )
            ("real", treat_direct )
			("int4", treat_direct )
			("int4", treat_direct )
			("int4", treat_direct );

    // Statement Insert value
    prepare("WCIWriteByteA",
    		"select "
            "wci.write ("
            "$1::bytea,"
    		"$2::text,"
    		"$3::text,"
    		"$4::timestamp with time zone,"
    		"$5::timestamp with time zone,"
    		"$6::timestamp with time zone,"
    		"$7::text,"
    		"$8::text,"
    		"$9::real,"
    		"$10::real,"
    		"$11::integer,"
            "$12::integer"
    		")" )
            ("bytea", treat_binary )
            ("varchar", treat_direct )
            ("varchar", treat_direct )
            ("varchar", treat_direct )
			("varchar", treat_direct )
			("varchar", treat_direct )
			("varchar", treat_direct )
			("varchar", treat_direct )
            ("real", treat_direct )
            ("real", treat_direct )
			("int4", treat_direct )
			("int4", treat_direct );

}

LoaderDatabaseConnection::~LoaderDatabaseConnection()
{
    unprepare("WCIWriteByteA");
    unprepare("ReadPlaceXref");
    unprepare("ReadSrid");
    unprepare("WriteSrid");
    unprepare("ReadUnitData");
    perform ( EndWci( ), 1 );
}

void
LoaderDatabaseConnection::write( const double * values,
								 unsigned int noOfValues,
								 const std::string & dataProviderName,
								 const std::string & placeName,
								 const std::string & referenceTime,
								 const std::string & validTimeFrom,
								 const std::string & validTimeTo,
								 const std::string & valueParameterName,
								 const std::string & levelParameterName,
								 float levelFrom,
								 float levelTo,
								 int dataVersion,
								 int confidenceCode )
{
    try {
		perform(
			WriteByteA( values,
						noOfValues,
						dataProviderName,
						placeName,
						referenceTime,
						validTimeFrom,
						validTimeTo,
						valueParameterName,
						levelParameterName,
						levelFrom,
						levelTo,
						dataVersion,
						confidenceCode ),
			1
		);
	}
	catch (const exception &e)
	{
		// All exceptions thrown by libpqxx are derived from std::exception
	    throw WdbException(e.what(), __func__);
	}
}


void
LoaderDatabaseConnection::loadField(long int dataProvider,
								    long int placeId,
                                    const std::string & referenceTime,
                                    const std::string & validTimeFrom,
                                    const std::string & validTimeTo,
                                    int validTimeIndCode,
                                    int valueparameter,
								    const std::vector<wdb::database::WdbLevel> & levels,
                                    int dataVersion,
                                    int qualityCode,
                                    const double * values,
                                    unsigned int noOfValues )
{
    try {
		perform(
			WriteValue( dataProvider,
						placeId,
						referenceTime,
						validTimeFrom,
						validTimeTo,
						validTimeIndCode,
						valueparameter,
						levels,
						dataVersion,
						qualityCode,
						values,
	  					noOfValues ),
			1
		);
	}
	catch (const exception &e)
	{
		// All exceptions thrown by libpqxx are derived from std::exception
	    throw WdbException(e.what(), __func__);
	}
}

// Get PlaceId
long int
LoaderDatabaseConnection::getPlaceId(const std::string & geoObj,
                                       int geoDatum,
                                       long int iNum,
                                       long int jNum,
                                       float iInc,
                                       float jInc,
                                       float startLat,
                                       float startLon,
									   int origDatum)
{
	long int ret;
	try {
		perform(
			ReadPlaceDefinition( ret, geoObj, geoDatum, iNum, jNum, iInc, jInc, startLat, startLon, origDatum ),
			1
		);
	}
	catch (const WdbEmptyResultException &e)
	{
		throw;
	}
	catch (const exception &e)
	{
		// All exceptions thrown by libpqxx are derived from std::exception
	    throw WdbException(e.what(), __func__);
	}
	return ret;
}

long int
LoaderDatabaseConnection::setPlaceId(const std::string & geoObj,
                                       int geoDatum,
                                       long int iNum,
                                       long int jNum,
                                       float iInc,
                                       float jInc,
                                       float startLat,
                                       float startLon,
                                       int origDatum)
{
	int osrid = 0;
	// Get the Original SRID
	// Insert the Place Definition
	long int ret;
	try {
		perform(
			WritePlaceDefinition( ret, geoObj, geoDatum, iNum, jNum, iInc, jInc, startLat, startLon, origDatum ),
			1
		);
	}
	catch (const exception &e)
	{
		// All exceptions thrown by libpqxx are derived from std::exception
	    throw WdbException(e.what(), __func__);
	}

	return ret;
}

int
LoaderDatabaseConnection::getSrid( const std::string & projStr )
{
	long int ret;
	try {
		perform(
			ReadSrid( ret, projStr ),
			1
		);
	}
	catch (const WdbEmptyResultException &e)
	{
		throw e;
	}
	catch (const exception &e)
	{
		// All exceptions thrown by libpqxx are derived from std::exception
	    throw WdbException(e.what(), __func__);
	}
	return ret;
}


void
LoaderDatabaseConnection::readUnit( const std::string & unit, float * coeff, float * term )
{
	try {
		perform(
			ReadUnit( coeff, term, unit ),
			1
		);
	}
	catch (const WdbEmptyResultException &e)
	{
		// NOOP
	}
	catch (const exception &e)
	{
		// All exceptions thrown by libpqxx are derived from std::exception
	    throw WdbException(e.what(), __func__);
	}
}

}
}

