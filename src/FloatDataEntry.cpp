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

#include "FloatDataEntry.h"
#include <limits>

namespace wdb
{
namespace load
{

FloatDataEntry::FloatDataEntry() :
		value_(std::numeric_limits<float>::quiet_NaN()),
		levelFrom_(0),
		levelTo_(0),
		dataVersion_(0),
		confidenceCode_(0)
{}

FloatDataEntry::FloatDataEntry(
		float value,
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
	    ) :
	value_(value),
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
	confidenceCode_(confidenceCode)
{
}

FloatDataEntry::~FloatDataEntry()
{
}

}
}
