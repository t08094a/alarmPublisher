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

#ifndef IOPERATION_H
#define IOPERATION_H

#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/uuid/uuid.hpp>
#include "IPrintable.h"
#include "IKeywords.h"
#include "OperationKeywords.h"
#include "OperationResource.h"
#include "PropertyLocation.h"

class IOperation : public IPrintable
{
public:
    virtual ~IOperation() {}
    
    /**
     * @brief Gets the unique Id of this operation.
     * @return
     */
    virtual int GetId() const = 0;
    
    /**
     * @brief Gets the guid of this operation that is globally unique.
     * @return
     */
    virtual boost::uuids::uuid GetGuid() = 0;
    
    /**
     * @brief Gets the timestamp of when the operation materialized ("incoming" timestamp).
     *        For the actual alarm timestamp, use the property <see cref="P:Timestamp"/>.
     * @return
     */
    virtual boost::posix_time::ptime GetTimestampIncome() = 0;
    
    /**
     * @brief Gets the date and time of the actual alarm.
     * @return
     */
    virtual boost::posix_time::ptime GetTimestamp() = 0;
    
    /**
     * @brief Gets the absender
     * @return
     */
    virtual std::string GetAbsender() = 0;
    
    /**
     * @brief Adds the absender value.
     * @param value
     */
    virtual std::string GetTermin() = 0;
    
    virtual ILocation& GetEinsatzort() = 0;
    
    virtual std::string GetEinsatzortZusatz() = 0;
    
    virtual std::string GetEinsatzortPlannummer() = 0;
    
    virtual std::string GetEinsatzortStation() = 0;
    
    virtual double GetEinsatzortLatitude() = 0;
    
    virtual double GetEinsatzortLongitude() = 0;
    
    virtual ILocation& GetZielort() = 0;
    
    virtual std::string GetZielortZusatz() = 0;
    
    virtual std::string GetZielortStation() = 0;
    
    virtual std::string GetOperationNumber() = 0;
    
    virtual std::string GetMessenger() = 0;
    
    virtual std::string GetPriority() = 0;
    
    virtual IKeywords& GetKeywords() = 0;
    
    /**
     * @brief Gets the comment text. Usually this contains
     *        the result from the "Bemerkung" or "Hinweis" (etc.)-sections.
     * @return
     */
    virtual std::string GetComment() = 0;
    
    virtual const vector<unique_ptr<OperationResource>>& GetResources() const = 0;
    
    virtual std::string ToString() const = 0;
    
    friend std::ostream& operator<< (std::ostream& out, const IPrintable& printable)
    {
        out << printable.ToString();
        
        return out;
    }
};

#endif // IOPERATION_H
