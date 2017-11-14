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

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "IParser.h"
#include "Operation.h"
#include "CurrentSection.h"

using namespace std;


class IlsAnsbach : public IParser
{
private:
    vector<string> definedKeywords = {"ABSENDER", "FAX", "TERMIN", "EINSATZNUMMER", "NAME", "STRAßE", "ORT", "OBJEKT", "PLANNUMMER",
                                      "STATION", "KOORDINATE", "STRAßE", "ORT", "OBJEKT", "STATION", "SCHLAGW", "STICHWORT", "PRIO",
                                      "EINSATZMITTEL", "ALARMIERT", "GEFORDERTE AUSSTATTUNG"};
    vector<string> sectionNames = {"MITTEILER","EINSATZORT","ZIELORT","EINSATZGRUND", "EINSATZMITTEL", "BEMERKUNG", "ENDE FAX"};
    map<string, CurrentSection> sectionResolver = { 
            {"MITTEILER", CurrentSection::BMitteiler },
            {"EINSATZORT", CurrentSection::CEinsatzort },
            {"ZIELORT", CurrentSection::DZielort },
            {"EINSATZGRUND", CurrentSection::EEinsatzgrund },
            {"EINSATZMITTEL", CurrentSection::FEinsatzmittel },
            {"BEMERKUNG", CurrentSection::GBemerkung },
            {"ENDE FAX", CurrentSection::HFooter }
        };
        
    typedef void (IlsAnsbach::*WorkerFunction)(vector<string>& lines, Operation* operation);
    map<CurrentSection, WorkerFunction> sectionWorkerResolver;
    
public:
    IlsAnsbach();
    IlsAnsbach ( const IlsAnsbach& other );
    virtual ~IlsAnsbach();
    
    IOperation* Parse(const string& filename);
    IOperation* Parse(vector<string>& lines);
    
    IlsAnsbach& operator= ( const IlsAnsbach& other );
    bool operator== ( const IlsAnsbach& other ) const;
    
private:
    CurrentSection GetSection(const string& line);
    bool IsSectionBegin(const string& line);
	string CreateSectionHeaderLine(const string& sectionName);
	void InsertSectionHeaderLine(vector<string>& lines, const string& headerLine, CurrentSection toInsertSection, const int index);
    void ParseLineWithKeyword(string line, string& prefix, string& value);
        
    void ReadFaxHeader(vector<string>& lines, Operation* operation);
    void ReadMitteiler(vector<string>& lines, Operation* operation);
    void ReadEinsatzort(vector<string>& lines, Operation* operation);
    void ReadZielort(vector<string>& lines, Operation* operation);
    void ReadEinsatzgrund(vector<string>& lines, Operation* operation);
    void ReadEinsatzmittel(vector<string>& lines, Operation* operation);
    void ReadBemerkung(vector<string>& lines, Operation* operation);
};

#endif // ILSANSBACH_H
