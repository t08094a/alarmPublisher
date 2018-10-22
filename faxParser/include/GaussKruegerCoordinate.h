/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2017  Jochen Fähnlein <jochen.faehnlein@ff-ipsheim.de>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef COORDINATE_H
#define COORDINATE_H

#include <string>

using namespace std;

class GaussKruegerCoordinate
{
public:
    /**
    * @brief Initializes a new instance
    * 
    * @param _x The x value
    * @param _y The y value
    */
    GaussKruegerCoordinate(const string& _x, const string& _y);
    
    /**
    * @brief Initializes a new instance
    * 
    * @param _x The x value
    * @param _y The y value
    */
    GaussKruegerCoordinate(const double _x, const double _y);

    /**
    * @brief The copy constructor.
    * 
    * @param other The instance to copy.
    */
    GaussKruegerCoordinate(const GaussKruegerCoordinate& other);
    
    /**
    * @brief The destructor.
    * 
    */
    ~GaussKruegerCoordinate();
    
    /**
    * @brief The assignment operator overload.
    * 
    * @param other The instance to copy.
    * @return GaussKruegerCoordinate&
    */
    GaussKruegerCoordinate& operator=(const GaussKruegerCoordinate& other);
    
    /**
    * @brief The ability to compare to instances.
    * 
    * @param other The instance to compare with.
    * @return bool
    */
    bool operator==(const GaussKruegerCoordinate& other) const;
    
    /**
    * @brief Gets the X coordinate in Gauß Krüger format.
    * 
    * @return double
    */
    double GetX();
    
    /**
    * @brief Gets the Y coordinate in Gauß Krüger format.
    * 
    * @return double
    */
    double GetY();
    
    /**
    * @brief Gets a value whether x and y are both 0.
    * 
    * @return bool true if x and y are 0.
    */
    bool IsEmpty();
    
private:
    double x;
    double y;
    
    template<typename T> bool IsValid(const string& num);
};

#endif // COORDINATE_H
