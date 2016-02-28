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

#include <sstream>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include "OperationResource.h"


OperationResource::OperationResource()
{
    // nop
}

OperationResource::OperationResource (const OperationResource& other)
{
    this->requestedEquipment = other.requestedEquipment;
    this->fullName = other.fullName;
    this->timestamp = other.timestamp;
}

OperationResource::~OperationResource()
{
    requestedEquipment.clear();
}

OperationResource& OperationResource::operator= (const OperationResource& other)
{
    if (this == &other)
    {
        return *this; // handle self assignment
    }
    
    this->requestedEquipment = other.requestedEquipment;
    this->fullName = other.fullName;
    this->timestamp = other.timestamp;
    
    return *this;
}

bool OperationResource::operator== (const OperationResource& other) const
{
    return fullName == other.fullName &&
           timestamp == other.timestamp &&
           requestedEquipment == other.requestedEquipment;
}

bool OperationResource::operator!= (const OperationResource& other) const
{
    return operator==(other) == false;
}

string OperationResource::GetFullName()
{
    return fullName;
}

void OperationResource::SetFullName(const string name)
{
    fullName = name;
}

const boost::posix_time::ptime OperationResource::GetTimestamp() const
{
    return timestamp;
}

void OperationResource::SetTimestamp(const boost::posix_time::ptime timestamp)
{
    this->timestamp = timestamp;
}

const vector<string>& OperationResource::GetRequestedEquipment() const
{
    return requestedEquipment;
}

void OperationResource::AddRequestedEquipment(string equipment)
{
    requestedEquipment.push_back(equipment);
}

string OperationResource::ToString() const
{
    stringstream ss;
    
    ss << fullName;
    ss << "\t(" << "Time: " << timestamp;
    ss << "\t" << "Equipments:";
    
    size_t i = 1;
    for(string s : requestedEquipment)
    {
        ss << s;
        
        if(i < requestedEquipment.size())
        {
            ss << ", ";
        }
        
        i++;
    }
    
    ss << ")";
    
    return ss.str();
}
