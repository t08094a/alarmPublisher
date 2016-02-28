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

#ifndef PROPERTYLOCATION_H
#define PROPERTYLOCATION_H

#include <string>
#include <iostream>
#include "IPrintable.h"
#include "ILocation.h"

using namespace std;

class PropertyLocation : public ILocation
{
private:
    string location;
    string zipCode;
    string city;
    string street;
    string streetNumber;
    string intersection;
    string geoLatitude;
    string geoLongitude;
    string property;
    
public:
    PropertyLocation();
    PropertyLocation ( const PropertyLocation& other );
    virtual ~PropertyLocation();
    
    PropertyLocation& operator= ( const PropertyLocation& other );
    bool operator== ( const PropertyLocation& other ) const;
    bool operator!= (const PropertyLocation& other) const;
    
    /**
     * @brief Gets the location name.
     * @return
     */
    string GetLocation() const;

    /**
     * @brief Sets the location name.
     * @param location
     */
    void SetLocation(const string location);

    /**
     * @brief Gets the zip code of the city.
     * @return
     */
    string GetZipCode() const;

    /**
     * @brief Sets the zip code of the city.
     * @param zipCode
     */
    void SetZipCode(const string zipCode);

    /**
     * @brief Gets the city name.
     * @return
     */
    string GetCity() const;

    /**
     * @brief Sets the city name.
     * @param city
     */
    void SetCity(const string city);

    /**
     * @brief Gets the street. May contain the street number.
     * @return
     */
    string GetStreet() const;

    /**
     * @brief Sets the street. May contain the street number.
     * @param street
     */
    void SetStreet(const string street);

    /**
     * @brief Gets the street number. May be contained within the street.
     * @return
     */
    string GetStreetNumber() const;

    /**
     * @brief Sets the street number. May be contained within the street.
     * @param number
     */
    void SetStreetNumber(const string number);

    /**
     * @brief Gets a description of the "Intersection" (if provided by alarmsource).
     * @return
     */
    string GetIntersection() const;

    /**
     * @brief Sets a description of the "Intersection" (if provided by alarmsource).
     * @param intersection
     */
    void SetIntersection(const string intersection);

    /**
     * @brief Gets the latitude of the location (if provided by alarmsource).
     * @return
     */
    string GetGeoLatitude() const;

    /**
     * @brief Sets the latitude of the location (if provided by alarmsource).
     * @param latitude
     */
    void SetGeoLatitude(const string latitude);

    /**
     * @brief Gets the longitude of the location (if provided by alarmsource).
     * @return
     */
    string GetGeoLongitude() const;

    /**
     * @brief Sets the longitude of the location (if provided by alarmsource).
     * @param longitude
     */
    void SetGeoLongitude(const string longitude);

    /**
     * @brief Gets the name of the property (company, site, house etc.).
     * @return
     */
    string GetProperty() const;

    /**
     * @brief Sets the name of the property (company, site, house etc.).
     * @param property
     */
    void SetProperty(const string property);

    /**
     * @brief Gets whether or not this instance represents a meaningful geographic location.
     *        This takes only ZipCode, City and Street into account.
     * @return
     */
    bool IsMeaningful() const;

    /**
     * @brief Gets whether or not there are meaningful values for the geo
     *        coordinates (latitude and longitude) defined.
     * @return
     */
    bool HasGeoCoordinates() const;

    /**
     * @brief Returns a string describing the property location like:
     *        "[[Street] [StreetNumber]], [ZipCode] [City]".
     * @return
     */
    string ToString() const;
};

#endif // PROPERTYLOCATION_H
