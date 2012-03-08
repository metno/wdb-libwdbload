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

#ifndef COPYSTATEMENTCONVERTER_H_
#define COPYSTATEMENTCONVERTER_H_

#include <map>
#include <string>
#include <pqxx/transaction>


namespace wdb
{
namespace load
{
class FloatDataEntry;

namespace internal
{

class CopyStatementConverter
{
public:
	CopyStatementConverter(pqxx::work & transaction);
	~CopyStatementConverter();

	std::string get(const FloatDataEntry & fda);

private:


	long long dataproviderid(const std::string & dataproviderName);
	long long placeid(const std::string & placeName);
	int paramid(const std::string & placeName);

	long long nextSerialNumber_();

	long long serialNumberPoolCurrent_;
	long long serialNumberPoolMax_;

	std::map<std::string, long long> dataproviders_;
	std::map<std::string, long long> places_;
	std::map<std::string, int> parameters_;

	pqxx::work & transaction_;
};

}
}
}

#endif /* COPYSTATEMENTCONVERTER_H_ */
