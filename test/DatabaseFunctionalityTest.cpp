/*
 libwdbload 

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

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE DatabaseFunctionalityTest
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <LoaderDatabaseConnection.h>
#include <LoaderConfiguration.h>

namespace
{
	wdb::load::LoaderConfiguration getConfig()
	{
		int argc = 2;
		char * argv[4] = {
				(char *) "test_program",
				(char *) "--namespace=test"
		};
		wdb::load::LoaderConfiguration ret;
		ret.parse(argc, argv);
		return ret;
	}
}

class DatabaseFunctionalityTestFixture
{
public:
	DatabaseFunctionalityTestFixture() :
		connection(getConfig())
	{
	}

	wdb::load::LoaderDatabaseConnection connection;
};


BOOST_FIXTURE_TEST_CASE( getVersion, DatabaseFunctionalityTestFixture )
{
	std::string version = connection.wciVersion();

	std::cout << "Database wci version = " << version << std::endl;
}

//BOOST_FIXTURE_TEST_CASE( writeTest, DatabaseFunctionalityTestFixture )
//{
//	BOOST_CHECK_MESSAGE(false, "no tests for writing");
//}

BOOST_FIXTURE_TEST_CASE( getPlaceNameTest, DatabaseFunctionalityTestFixture )
{
	// based on what should be in database after a normal install

	std::string name = connection.getPlaceName(100, 100, 0.1, 0.1, 5, 55, "+proj=longlat +ellps=WGS84 +no_defs");
	BOOST_CHECK_EQUAL("test grid, wgs84", name);
}

//BOOST_FIXTURE_TEST_CASE( addPlaceDefinitionTest, DatabaseFunctionalityTestFixture )
//{
//	BOOST_CHECK_MESSAGE(false, "no tests for adding place definitions");
//}

BOOST_FIXTURE_TEST_CASE(readUnitTest, DatabaseFunctionalityTestFixture )
{
	// based on what should be in database after a normal install

	float coeff = -1;
	float term = -1;
	connection.readUnit("Cel", & coeff, & term);
	BOOST_CHECK_CLOSE(1, coeff, 0.0001);
	BOOST_CHECK_CLOSE(273.15, term, 0.0001);
}
