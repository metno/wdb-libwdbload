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

#ifndef FLOATDATAENTRY_H_
#define FLOATDATAENTRY_H_

#include <string>

namespace wdb
{
namespace load
{

class FloatDataEntry
{
public:
	FloatDataEntry();

	FloatDataEntry(float value,
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
	~FloatDataEntry();


    float value() const
    {
        return value_;
    }
    void value(float v)
    {
    	value_ = v;
    }

    const std::string & dataProviderName() const
    {
        return dataProviderName_;
    }
    void dataProviderName(const std::string & name)
    {
    	dataProviderName_ = name;
    }

    const std::string & placeName() const
    {
        return placeName_;
    }
    void placeName(const std::string & name)
    {
    	placeName_ = name;
    }

    const std::string & referenceTime() const
    {
        return referenceTime_;
    }
    void referenceTime(const std::string & time)
    {
    	referenceTime_ = time;
    }

    const std::string & validTimeFrom() const
    {
        return validTimeFrom_;
    }
    void validTimeFrom(const std::string & time)
    {
    	validTimeFrom_ = time;
    }

    const std::string & validTimeTo() const
    {
        return validTimeTo_;
    }
    void validTimeTo(const std::string & time)
    {
    	validTimeTo_ = time;
    }

    const std::string & valueParameterName() const
    {
        return valueParameterName_;
    }
    void valueParameterName(const std::string & name)
    {
        valueParameterName_ = name;
    }

    const std::string & levelParameterName() const
    {
        return levelParameterName_;
    }
    void levelParameterName(const std::string & name)
    {
        levelParameterName_ = name;
    }

    float levelFrom() const
    {
        return levelFrom_;
    }
    void levelFrom(float level)
    {
    	levelFrom_ = level;
    }

    float levelTo() const
    {
        return levelTo_;
    }
    void levelTo(float level)
    {
    	levelTo_ = level;
    }

    int dataVersion() const
    {
        return dataVersion_;
    }
    void dataVersion(int version)
    {
    	dataVersion_ = version;
    }

    int confidenceCode() const
    {
        return confidenceCode_;
    }
    void confidenceCode(int code)
    {
        confidenceCode_ = code;
    }
	
	
private:
	float value_;
	std::string dataProviderName_;
	std::string placeName_;
	std::string referenceTime_;
	std::string validTimeFrom_;
	std::string validTimeTo_;
	std::string valueParameterName_;
	std::string levelParameterName_;
	float levelFrom_;
	float levelTo_;
	int dataVersion_;
	int confidenceCode_;
};

}
}

#endif /* FLOATDATAENTRY_H_ */
