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

#include "../include/GaussKruegerCoordinate.h"
#include <string>
#include <exception>
#include <typeinfo>
#include <boost/log/trivial.hpp>
#include <boost/lexical_cast.hpp>


using namespace std;

GaussKruegerCoordinate::GaussKruegerCoordinate(const string& _x, const string& _y) : x("0"), y("0")
{
    if(! _x.empty() && IsValid<double>(_x))
    {
        this->x = _x;
    }

    if(! _y.empty() && IsValid<double>(_y))
    {
        this->y = _y;
    }
}

GaussKruegerCoordinate::GaussKruegerCoordinate(const GaussKruegerCoordinate& other)
{
    this->x = other.x;
    this->y = other.y;
}

GaussKruegerCoordinate::~GaussKruegerCoordinate()
{
    // nop
}

GaussKruegerCoordinate& GaussKruegerCoordinate::operator=(const GaussKruegerCoordinate& other)
{
    if(this != &other)
    {
        this->x = other.x;
        this->y = other.y;
    }
    
    return *this;
}

bool GaussKruegerCoordinate::operator==(const GaussKruegerCoordinate& other) const
{
    return this->x == other.x && this->y == other.y;
}

string GaussKruegerCoordinate::GetX()
{
    return this->x;
}

string GaussKruegerCoordinate::GetY()
{
    return this->y;
}

bool GaussKruegerCoordinate::IsEmpty()
{
    return this->x == "0" || this->y == "0";
}

template<typename T> 
bool GaussKruegerCoordinate::IsValid(const string& num) 
{ 
    bool flag = true; 

    try 
    { 
        boost::lexical_cast<T>(num);
    } 
    catch (boost::bad_lexical_cast &ex) 
    { 
        flag = false; 
        
        BOOST_LOG_TRIVIAL(error) << "Unable to parse " << num << " to " << typeid(T).name()  << ex.what();
    } 

    return flag; 
} 
