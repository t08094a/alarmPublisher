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

#ifndef IKEYWORDS_H
#define IKEYWORDS_H

#include <string>
#include <iostream>
#include "IPrintable.h"

using namespace std;

/**
 * @brief Contains the keywords ("stichwörter") for an operation.
 */
class IKeywords : public IPrintable
{
public:
    virtual ~IKeywords() {}
    
    /**
     * @brief Gets the "Stichwort" (generic keyword), direct or equivalent.
     * @return
     */
    virtual string GetKeyword() const = 0;

    /**
     * @brief Gets the B/R/S/T/etc. keyword for sources that don't distinguish between them.
     * @return
     */
    virtual string GetEmergencyKeyword() const = 0;
    
    /**
     * @brief Gets the "Stichwort B" (specific keyword), direct or equivalent.
     * @return
     */
    virtual string GetB() const = 0;
    
    /**
     * @brief Gets the "Stichwort R" (specific keyword), direct or equivalent.
     * @return
     */
    virtual string GetR() const = 0;
    
    /**
     * @brief Gets the "Stichwort S" (specific keyword), direct or equivalent.
     * @return
     */
    virtual string GetS() const = 0;
    
    /**
     * @brief Gets the "Stichwort T" (specific keyword), direct or equivalent.
     * @return
     */
    virtual string GetT() const = 0;
    
    /**
     * @brief Returns a string that contains all informations.
     * @return A string that contains all informations.
     */
    virtual std::string ToString() const = 0;
    
    bool operator== (const IKeywords& other) const
    {
        return this->GetKeyword() == other.GetKeyword() &&
               this->GetEmergencyKeyword() == other.GetEmergencyKeyword() &&
               this->GetB() == other.GetB() &&
               this->GetR() == other.GetR() &&
               this->GetS() == other.GetS() &&
               this->GetT() == other.GetT();
    }
    
    friend ostream& operator<< (ostream& out, const IKeywords& keywords)
    {
        out << keywords.ToString();
        
        return out;
    }
};

#endif // IKEYWORDS_H
