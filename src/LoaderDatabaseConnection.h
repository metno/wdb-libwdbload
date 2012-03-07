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

#ifndef LOADERDATABASECONNECTION_H_
#define LOADERDATABASECONNECTION_H_

#include "LoaderDatabaseInterface.h"
#include "WdbLevel.h"
#include "FloatDataEntry.h"
#include <pqxx/connection>
#include <string>
#include <limits>

extern "C" {
	struct PlaceSpecification;
}

namespace wdb
{
namespace load
{
class LoaderConfiguration;


/**
 * Database connection class for loading programs, with a loading function and
 * a few utility functions. Loading programs may use this as a base class for
 * writing to wci. Writing is done through the loadField method, and it is up
 * to each subclass to determine the correct arguments to give.
 *
 * The other methods in this class provide a way to request information from
 * the database which may later be used in a call to loadField.
 */
class LoaderDatabaseConnection : public pqxx::connection, public LoaderDatabaseInterface
{
public:
	explicit LoaderDatabaseConnection(const LoaderConfiguration & config);
	virtual ~LoaderDatabaseConnection();

	// OPERATIONS
    /**
     * Load a Value into the Database for the designated field.
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
    virtual void write(
					const float * values,
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
				    int confidenceCode
    );

    virtual void write(const std::vector<FloatDataEntry> & points);

    /**
     * Get the value to use for "undefined" entries in a grid.
     */
    double getUndefinedValue() const { return std::numeric_limits<double>::quiet_NaN(); }

    /** Find the PlaceName given by a combination of attributes.
     * This function shall be overloaded for the various data types.
     * The following parameters identify a regular grid.
     * @param	xNum		Number of points along the x axis
     * @param	yNum		Number of points along the y axis
     * @param	xInc		Size of increments along the x axis
     * @param	yInc		Size of increments along the y axis
     * @param	startX		Start X of the grid
     * @param	startY		Start Y of the grid
     * @param	origProj	The original projection of the grid
     * @return the PlaceName
     */
    std::string getPlaceName( int xNum,
                              int yNum,
                              float xInc,
                              float yInc,
                              float startX,
                              float startY,
							  std::string origProj );

    std::string getPlaceName(const PlaceSpecification & ps);

    /** Find the PlaceName given by a combination of attributes.
     * This function shall be overloaded for the various data types.
     * The following parameters identify a regular grid.
     * @param 	name		Name of the place definition to be inserted
     * @param	xNum		Number of points along the x axis
     * @param	yNum		Number of points along the y axis
     * @param	xInc		Size of increments along the x axis
     * @param	yInc		Size of increments along the y axis
     * @param	startX		Start X of the grid
     * @param	startY		Start Y of the grid
     * @param	origProj	The original projection of the grid
     * @return the PlaceName
     */
    std::string addPlaceDefinition( std::string placeName,
									int xNum,
									int yNum,
									float xInc,
									float yInc,
									float startX,
									float startY,
									std::string origProj );


    std::string addPlaceDefinition( std::string placeName,
									const PlaceSpecification & ps);

    /**
     * Load a Value into the Database for the designated field.
     * @param	dataProvider	The data provider id of the field
     * @param	placeId			The place id of the fields grid description
     * @param	referenceTime	The reference time of the field
     * @param	validTimeFrom	Valid time from of the field
     * @param	validTimeTo		Valid time to of the field
     * @param	validTimeIndCode	Valid time indeterminate code of the field
     * @param	parameter		Information about the unit, usage, and statistics type of field data
     * @param	levels			Vector of level parameters
     * @param	dataVersion		The data version of the field
     * @param	qualityCode		WDB quality code to be associated with the field
     * @param	values			A pointer to the array of values (doubles) in the field
     * @param	noOfValues		The size of the values array
     */
    virtual void loadField(	long int dataProvider,
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
	     		    unsigned int noOfValues );

    /**
     * Read the Unit Data
     * @param	unit		The unit to be queries
     * @param	coeff1		First coefficient of conversion
     * @param	term1		First term of conversion
     * @param	coeff2		Second coefficient of conversion
     * @param	term2		Second term of conversion
     */
    void readUnit( const std::string & unit, float * coeff, float * term );

private:
    void setup_();

	std::string dataProvider_(const std::string & given) const;
	std::string placeName_(const std::string & given) const;
	std::string referenceTime_(const std::string & given) const;
	std::string valueParameter_(const std::string & given) const;
	std::string levelParameter_(const std::string & given) const;
	int dataVersion_(int given) const;
	int confidenceCode_(int given) const;


    const LoaderConfiguration * config_;
};

}

}

#endif /* LOADERDATABASECONNECTION_H_ */
