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
#include "LoaderConfiguration.h"
#include "transactors/AddPlaceDefinition.h"
#include "transactors/AddSrid.h"
#include "transactors/BeginWci.h"
#include "transactors/EndWci.h"
#include "transactors/GetPlaceName.h"
#include "transactors/GetSrid.h"
#include "transactors/InfoParameterUnit.h"
#include "transactors/loadTransactorWriteByteA.h"
#include "transactors/loaderTransactorValue.h"
#include <pqxx/pqxx>
#include <stdexcept>

using namespace std;
using namespace pqxx;
using namespace pqxx::prepare;
using namespace wdb::load;

namespace wdb
{

namespace load
{

LoaderDatabaseConnection::LoaderDatabaseConnection(const LoaderConfiguration & config)
	: pqxx::connection(config.database().pqDatabaseConnection()), config_(new LoaderConfiguration(config))
{
	setup_(config.database().user);
}

LoaderDatabaseConnection::LoaderDatabaseConnection( const std::string & target, const std::string & wciUser )
	: pqxx::connection( target.c_str() ), config_(0)
{
	setup_(wciUser);
}

LoaderDatabaseConnection::~LoaderDatabaseConnection()
{
    unprepare("WCIWriteByteA");
    unprepare("GetPlaceName");
    unprepare("InfoParameterUnit");
    perform ( EndWci( ), 1 );

    delete config_;
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
						dataProvider_(dataProviderName),
						placeName_(placeName),
						referenceTime,
						validTimeFrom,
						validTimeTo,
						valueParameterName,
						levelParameterName,
						levelFrom,
						levelTo,
						dataVersion_(dataVersion),
						confidenceCode_(confidenceCode) ),
			1
		);
	}
	catch (const exception &e)
	{
		// All exceptions thrown by libpqxx are derived from std::exception
	    throw;
	}
}

// Get PlaceId
std::string
LoaderDatabaseConnection::getPlaceName( int xNum,
                                        int yNum,
                                        float xInc,
                                        float yInc,
                                        float startX,
                                        float startY,
									    std::string origProj )
{
	std::string ret;
	try {
		perform(
			GetPlaceName( ret, xNum, yNum, xInc, yInc, startX, startY, origProj ),
			1
		);
	}
	catch ( const wdb::empty_result &e )
	{
		throw;
	}
	return ret;
}

// Get PlaceId
void
LoaderDatabaseConnection::addPlaceDefinition( std::string placeName,
											  int xNum,
											  int yNum,
											  float xInc,
											  float yInc,
											  float startX,
											  float startY,
											  std::string origProj )
{
	/// First check that the SRID is legitimate
	try {
		int srid;
		perform(
			GetSrid( srid, origProj ),
			1
		);
	}
	catch ( const wdb::empty_result &e )
	{
		// No SRID was found
		perform(
			AddSrid( placeName_(placeName), origProj ),
			1
		);
		// We use the placeName as the sridName, as this makes it
		// easier to connect which placedefinition the srid was
		// first inserted for later. We also expect the placename
		// to be unique.
	}
	// SRID has been found and is valid... attempt to insert place definiton
	perform(
		AddPlaceDefinition( placeName_(placeName), xNum, yNum, xInc, yInc, startX, startY, origProj ),
		1
	);
}



void
LoaderDatabaseConnection::loadField(long int dataProvider,
								    long int placeId,
                                    const std::string & referenceTime,
                                    const std::string & validTimeFrom,
                                    const std::string & validTimeTo,
                                    int validTimeIndCode,
                                    int valueparameter,
								    const std::vector<wdb::load::WdbLevel> & levels,
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
						dataVersion_(dataVersion),
						confidenceCode_(qualityCode),
						values,
	  					noOfValues ),
			1
		);
	}
	catch (const exception &e)
	{
		// All exceptions thrown by libpqxx are derived from std::exception
	    throw;
	}
}

void
LoaderDatabaseConnection::readUnit( const std::string & unit, float * coeff, float * term )
{
	try {
		perform(
			InfoParameterUnit( coeff, term, unit ),
			1
		);
	}
	catch (const exception &e)
	{
		// All exceptions thrown by libpqxx are derived from std::exception
	    throw e;
	}
}

void LoaderDatabaseConnection::setup_(const std::string & wciUser)
{
	// Initialize WCI
	perform ( BeginWci( wciUser, 0 ), 1 );

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

	// Statement Get PlaceId
	prepare("GetPlaceName",
			"SELECT * FROM wci.getplacename ($1, $2, $3, $4, $5, $6, $7)" )
		   ("int4", treat_direct )
		   ("int4", treat_direct )
		   ("real", treat_direct )
		   ("real", treat_direct )
		   ("real", treat_direct )
		   ("real", treat_direct )
		   ("varchar", treat_direct );

	// Statement insertSrid
	prepare("InfoParameterUnit",
			"SELECT * FROM wci.info ( $1, NULL::wci.infoparameterunit )" )
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
}

std::string LoaderDatabaseConnection::dataProvider_(const std::string & given) const
{
	if ( config_ )
	{
		if ( not config_->loading().dataProvider.empty() )
			return config_->loading().dataProvider;
		else if ( not config_->loading().defaultDataProvider.empty() )
			return config_->loading().defaultDataProvider;
	}
	return given;

//	if ( config_ and not config_->loading().dataProvider.empty() )
//		return config_->loading().dataProvider;
//	return given;
}

std::string LoaderDatabaseConnection::placeName_(const std::string & given) const
{
	if ( config_ and not config_->loading().placeName.empty() )
		return config_->loading().placeName;
	return given;
}

int LoaderDatabaseConnection::dataVersion_(int given) const
{
	if ( config_ )
		return config_->loading().dataVersion;
	return given;
}

int LoaderDatabaseConnection::confidenceCode_(int given) const
{
	if ( config_ )
		return config_->loading().confidenceCode;
	return given;
}

}

}

