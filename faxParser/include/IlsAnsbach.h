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

#ifndef ILSANSBACH_H
#define ILSANSBACH_H

#include <string>
#include <vector>

#include "IParser.h"
#include "IOperation.h"
#include "CurrentSection.h"

class IlsAnsbach : public IParser
{
private:
    std::vector<std::string> definedKeywords = {"ABSENDER", "FAX", "TERMIN", "EINSATZNUMMER", "NAME", "STRAßE", "ORT", "OBJEKT", "PLANNUMMER",
                                         "STATION", "STRAßE", "ORT", "OBJEKT", "STATION", "SCHLAGW", "STICHWORT", "PRIO",
                                         "EINSATZMITTEL", "ALARMIERT", "GEFORDERTE AUSSTATTUNG"};

public:
    IlsAnsbach();
    IlsAnsbach ( const IlsAnsbach& other );
    virtual ~IlsAnsbach();
    
    IOperation* Parse(const std::string& filename);
    IOperation* Parse(std::vector<std::string> lines);
    
    IlsAnsbach& operator= ( const IlsAnsbach& other );
    bool operator== ( const IlsAnsbach& other ) const;
    
private:
    bool GetSection(const std::string &line, CurrentSection& section, bool& keywordsOnly);
};

#endif // ILSANSBACH_H
