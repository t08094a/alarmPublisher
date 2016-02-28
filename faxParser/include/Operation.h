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

#ifndef OPERATION_H
#define OPERATION_H

#include <vector>
#include <iostream>
#include "IOperation.h"
#include "OperationKeywords.h"
#include "OperationResource.h"
#include "PropertyLocation.h"

using namespace std;

class Operation : public IOperation
{
private:
    vector<unique_ptr<OperationResource>> resources;
    int id;
    boost::uuids::uuid guid;
    boost::posix_time::ptime timestampIncome;
    boost::posix_time::ptime timestamp;
    string absender;
    string termin;
    string einsatzortZusatz;
    string einsatzortPlannummer;
    string einsatzortStation;
    string zielortZusatz;
    string zielortStation;
    string operationNumber;
    string messenger;
    string priority;
    string comment;
    PropertyLocation einsatzort;
    PropertyLocation zielort;
    OperationKeywords keywords;
    
    void CopyValues(const Operation& other);

public:
    Operation();
    Operation ( const Operation& other );
    virtual ~Operation();
    Operation& operator= ( const Operation& other );
    bool operator== ( const Operation& other ) const;
    bool operator!= ( const Operation& other ) const;
    
    friend ostream& operator<< (ostream& out, const Operation& operation)
    {
        out << operation.ToString();
        
        return out;
    }
        
    /**
     * @brief Gets the unique Id of this operation.
     * @return
     */
    int GetId() const;

    /**
     * @brief Sets the unique Id of this operation.
     * @param id
     */
    void SetId(const int id);

    /**
     * @brief Gets the guid of this operation that is globally unique.
     * @return
     */
    boost::uuids::uuid GetGuid();

    /**
     * @brief Sets the guid of this operation that is globally unique.
     * @param guid
     */
    void SetGuid(const boost::uuids::uuid guid);

    /**
     * @brief Gets the timestamp of when the operation materialized ("incoming" timestamp).
     *        For the actual alarm timestamp, use the property <see cref="P:Timestamp"/>.
     * @return
     */
    boost::posix_time::ptime GetTimestampIncome();

    /**
     * @brief Sets the timestamp of when the operation materialized ("incoming" timestamp).
     *        For the actual alarm timestamp, use the property <see cref="P:Timestamp"/>.
     * @param time
     */
    void SetTimestampIncome(const boost::posix_time::ptime time);

    /**
     * @brief Gets the date and time of the actual alarm.
     * @return
     */
    boost::posix_time::ptime GetTimestamp();

    /**
     * @brief Sets the date and time of the actual alarm.
     * @param time
     */
    void SetTimestamp(const boost::posix_time::ptime time);

    /**
     * @brief Gets the absender
     * @return
     */
    string GetAbsender();

    /**
     * @brief Adds the absender value.
     * @param value
     */
    void SetAbsender(const string value);

    /**
     * @brief Adds the absender value.
     * @param value
     */
    string GetTermin();

    void SetTermin(const string termin);

    string GetEinsatzortZusatz();

    void SetEinsatzortZusatz(const string zusatz);

    string GetEinsatzortPlannummer();

    void SetEinsatzortPlannummer(const string plannummer);

    string GetEinsatzortStation();

    void SetEinsatzortStation(const string station);

    string GetZielortZusatz();

    void SetZielortZusatz(const string zusatz);

    string GetZielortStation();

    void SetZielortStation(const string station);

    string GetOperationNumber();

    void SetOperationNumber(const string number);

    PropertyLocation& GetEinsatzortInternal();
    
    ILocation& GetEinsatzort();

    PropertyLocation& GetZielortInternal();
    
    ILocation& GetZielort();
    
    string GetMessenger();

    void SetMessenger(const string messenger);

    string GetPriority();

    void SetPriority(string priority);

    IKeywords& GetKeywords();
    
    OperationKeywords& GetKeywordsInternal();

    void AddResource(OperationResource *resource);
    
    const vector<unique_ptr<OperationResource>>& GetResources() const;

    /**
     * @brief Gets the comment text. Usually this contains
     *        the result from the "Bemerkung" or "Hinweis" (etc.)-sections.
     * @return
     */
    string GetComment();

    /**
     * @brief sets the comment text. Usually this contains
     *        the result from the "Bemerkung" or "Hinweis" (etc.)-sections.
     * @param comment
     */
    void SetComment(const string comment);
    
    std::string ToString() const;
};

#endif // OPERATION_H
