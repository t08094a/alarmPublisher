/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2015  Jochen Fähnlein <jochen.faehnlein@ff-ipsheim.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "PropertyLocation.h"
#include <cmath>
#include <string>
#include <sstream>
#include <boost/algorithm/string/trim.hpp>

PropertyLocation::PropertyLocation() : location(), zipCode(), city(), street(), streetNumber(), intersection(), geoLatitude(-1), geoLongitude(-1), property()
{
    // nop
}

PropertyLocation::PropertyLocation (const PropertyLocation& other)
{
    //copy ctor
    this->city = other.city;
    this->geoLatitude = other.geoLatitude;
    this->geoLongitude = other.geoLongitude;
    this->intersection = other.intersection;
    this->location = other.location;
    this->property = other.property;
    this->street = other.street;
    this->streetNumber = other.streetNumber;
    this->zipCode = other.zipCode;
}

PropertyLocation::~PropertyLocation()
{
    // nop
}

PropertyLocation& PropertyLocation::operator= (const PropertyLocation& other)
{
    if (this == &other)
    {
        return *this; // handle self assignment
    }
    
    this->city = other.city;
    this->geoLatitude = other.geoLatitude;
    this->geoLongitude = other.geoLongitude;
    this->intersection = other.intersection;
    this->location = other.location;
    this->property = other.property;
    this->street = other.street;
    this->streetNumber = other.streetNumber;
    this->zipCode = other.zipCode;

    //assignment operator
    return *this;
}

bool PropertyLocation::operator== (const PropertyLocation& other) const
{
    return this->city == other.city &&
           std::fabs(this->geoLatitude - other.geoLatitude) < 1 &&
           std::fabs(this->geoLongitude - other.geoLongitude) < 1 &&
           this->intersection == other.intersection &&
           this->location == other.location &&
           this->property == other.property &&
           this->street == other.street &&
           this->streetNumber == other.streetNumber &&
           this->zipCode == other.zipCode;
}

bool PropertyLocation::operator!= (const PropertyLocation& other) const
{
    return ! operator==(other);
}

string PropertyLocation::GetLocation() const
{
    return location;
}

void PropertyLocation::SetLocation(const string location)
{
    this->location = location;
    boost::trim(this->location);
}

string PropertyLocation::GetZipCode() const
{
    return zipCode;
}

void PropertyLocation::SetZipCode(const string zipCode)
{
    this->zipCode = zipCode;
    boost::trim(this->zipCode);
}

string PropertyLocation::GetCity() const
{
    return city;
}

void PropertyLocation::SetCity(const string city)
{
    this->city = city;
    boost::trim(this->city);
}

string PropertyLocation::GetStreet() const
{
    return street;
}

void PropertyLocation::SetStreet(const string street)
{
    this->street = street;
    boost::trim(this->street);
}

string PropertyLocation::GetStreetNumber() const
{
    return streetNumber;
}

void PropertyLocation::SetStreetNumber(const string number)
{
    this->streetNumber = number;
    boost::trim(this->streetNumber);
}

string PropertyLocation::GetIntersection() const
{
    return intersection;
}

void PropertyLocation::SetIntersection(const string intersection)
{
    this->intersection = intersection;
    boost::trim(this->intersection);
}

double PropertyLocation::GetGeoLatitude() const
{
    return geoLatitude;
}

void PropertyLocation::SetGeoLatitude(const double latitude)
{
    this->geoLatitude = latitude;
}

double PropertyLocation::GetGeoLongitude() const
{
    return geoLongitude;
}

void PropertyLocation::SetGeoLongitude(const double longitude)
{
    this->geoLongitude = longitude;
}

string PropertyLocation::GetProperty() const
{
    return property;
}

void PropertyLocation::SetProperty(const string property)
{
    this->property = property;
    boost::trim(this->property);
}

bool PropertyLocation::IsMeaningful() const
{
    return zipCode.empty() == false || city.empty() == false || street.empty() == false;
}

bool PropertyLocation::HasGeoCoordinates() const
{
    return geoLatitude > 0 && geoLongitude > 0;
}

string PropertyLocation::ToString() const
{
    std::stringstream ss;

    if(street.empty() == false)
    {
        ss << street;

        if(streetNumber.empty() == false)
        {
            ss << " " << streetNumber;
        }
    }

    if(zipCode.empty() == false)
    {
        if(ss.tellp() > 0)
        {
            ss << ", ";
        }

        ss << zipCode;
    }

    if(city.empty() == false)
    {
        if(zipCode.empty() == false)
        {
            ss << " ";
        }
        else
        {
            ss << ", ";
        }

        ss << city;
    }

    return ss.str();
}
