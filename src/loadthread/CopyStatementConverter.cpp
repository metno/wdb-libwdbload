/*
 wdb-libwdbload

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

#include "CopyStatementConverter.h"
#include <FloatDataEntry.h>
#include <sstream>


namespace wdb
{
namespace load
{
namespace internal
{

CopyStatementConverter::CopyStatementConverter(pqxx::work & transaction) :
		transaction_(transaction),
		serialNumberPoolCurrent_(0),
		serialNumberPoolMax_(0)
{
}

CopyStatementConverter::~CopyStatementConverter()
{
}

std::string CopyStatementConverter::get(const FloatDataEntry & fda)
{
	long long serial = nextSerialNumber_();

	std::ostringstream s;
	s << serial << '\t' << 1 << '\t'
			<< dataproviderid(fda.dataProviderName()) << '\t'
			<< placeid(fda.placeName()) << '\t' << fda.referenceTime() << '\t'
			<< fda.validTimeFrom() << '\t' << fda.validTimeTo() << '\t' << 0
			<< '\t' << paramid(fda.valueParameterName()) << '\t'
			<< paramid(fda.levelParameterName()) << '\t' << fda.levelFrom()
			<< '\t' << fda.levelTo() << '\t' << 0 << '\t' << 0 << '\t' << 0
			<< '\t' << 0 << '\t' << fda.value() << '\t'
			<< "2012-02-23 10:22:12.863787+01\n";
	return s.str();
}

long long CopyStatementConverter::nextSerialNumber_()
{
	if ( serialNumberPoolCurrent_ >= serialNumberPoolMax_ )
	{
		pqxx::result r = transaction_.exec("SELECT setval('wdb_int.floatvalue_valueid_seq'::regclass, nextval('wdb_int.floatvalue_valueid_seq'::regclass) + 1000)");
		serialNumberPoolMax_ = r[0][0].as<long long>();
		serialNumberPoolCurrent_ = serialNumberPoolMax_ - 1000;
	}
	return serialNumberPoolCurrent_ ++;
}

long long CopyStatementConverter::dataproviderid(
		const std::string & dataproviderName)
{
	long long & dataProviderid = dataproviders_[dataproviderName];
	if (!dataProviderid)
	{
		pqxx::result r = transaction_.exec(
				"SELECT dataproviderid FROM wci.getdataprovider('"
						+ transaction_.esc(dataproviderName) + "')");
		if (r.empty())
			throw std::runtime_error(
					dataproviderName + ": No such dataprovider");

		dataProviderid = r[0][0].as<long long>();
	}
	return dataProviderid;
}

long long CopyStatementConverter::placeid(const std::string & placeName)
{
	long long & placeid = places_[placeName];
	if (!placeid)
	{
		pqxx::result r = transaction_.exec(
				"SELECT placeid FROM wci.getplacedefinition('"
						+ transaction_.esc(placeName) + "')");
		if (r.empty())
			throw std::runtime_error(placeName + ": No such placename");
		placeid = r[0][0].as<long long>();
	}
	return placeid;
}

int CopyStatementConverter::paramid(const std::string & placeName)
{
	int & paramid = parameters_[placeName];
	if (!paramid)
	{
		pqxx::result r = transaction_.exec(
				"SELECT parameterid FROM wci.getparameter('"
						+ transaction_.esc(placeName) + "')");
		if (r.empty())
			throw std::runtime_error(placeName + ": No such parameter");
		paramid = r[0][0].as<int>();
	}
	return paramid;
}

}
}
}
