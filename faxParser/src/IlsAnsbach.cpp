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

#include "IlsAnsbach.h"
#include "OperationKeywords.h"
#include "OperationResource.h"
#include "ParserUtility.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

IlsAnsbach::IlsAnsbach() 
{
    sectionWorkerResolver[CurrentSection::AHeader] = &IlsAnsbach::ReadFaxHeader;
    sectionWorkerResolver[CurrentSection::BMitteiler] = &IlsAnsbach::ReadMitteiler;
    sectionWorkerResolver[CurrentSection::CEinsatzort] = &IlsAnsbach::ReadEinsatzort;
    sectionWorkerResolver[CurrentSection::DZielort] = &IlsAnsbach::ReadZielort;
    sectionWorkerResolver[CurrentSection::EEinsatzgrund] = &IlsAnsbach::ReadEinsatzgrund;
    sectionWorkerResolver[CurrentSection::FEinsatzmittel] = &IlsAnsbach::ReadEinsatzmittel;
    sectionWorkerResolver[CurrentSection::GBemerkung] = &IlsAnsbach::ReadBemerkung;
//     sectionWorkerResolver[CurrentSection::HFooter] = &IlsAnsbach::ReadFooter;
}

IlsAnsbach::IlsAnsbach ( const IlsAnsbach& other ) 
{
    // nop
}

IlsAnsbach::~IlsAnsbach() 
{
    // nop
}

IOperation* IlsAnsbach::Parse(const string& filename)
{
    ifstream ifs(filename.c_str(), std::ios::in | std::ios::binary);
    if(!ifs || ifs.is_open() == false)
    {
        cerr << "Error opening file \"" << filename << "\"" << endl;
        return nullptr;
    }
    
    cout << "Parsing file: " << filename << endl;

    vector<std::string> data;
    string line;
    
    while (std::getline(ifs, line))
    {
        data.push_back(line);
    }
    
    return Parse2(data);
}

IOperation* IlsAnsbach::Parse2(vector<string>& lines)
{
    Operation* operation = new Operation();
    
    ParserUtility::Trim(lines);
    ParserUtility::EraseEmptyItems(lines);
    
    CurrentSection section = CurrentSection::AHeader;
    
    while(section != CurrentSection::Undefined)
    {
        map<CurrentSection, WorkerFunction>::iterator it = sectionWorkerResolver.find(section);
        
        if(it != sectionWorkerResolver.end())
        {
            WorkerFunction ptr = it->second;
            (this->*ptr)(lines, operation);
            
            if(lines.size() > 0)
            {
                section = GetSection(lines.front());
                
                // delete header line
                lines.erase(lines.begin());
            }
            else
            {
                section = CurrentSection::Undefined;
            }
        }
        else
        {
            section = CurrentSection::Undefined;
        }
    }
    
    return operation;
}

IOperation* IlsAnsbach::Parse(vector<string>& lines)
{
    Operation* operation = new Operation();
    OperationResource* last = new OperationResource();

    ParserUtility::Trim(lines);

    CurrentSection section = CurrentSection::AHeader;
    bool keywordsOnly = true;

    for (size_t i = 0; i < lines.size(); i++)
    {
        string line = lines[i];
        if (line.length() == 0)
        {
            continue;
        }

        boost::posix_time::ptime currentTimestamp = operation->GetTimestamp();
        boost::posix_time::ptime parsedTime = ParserUtility::ReadFaxTimestamp(line, currentTimestamp);
        operation->SetTimestamp(parsedTime);

        boost::algorithm::trim(line);
        bool isSectionHeader = GetSection(line, section, keywordsOnly);
        if (isSectionHeader)
        {
            continue;
        }

        string msg = line;
        string prefix = "";
        string keyword = "";

        if (keywordsOnly)
        {
            bool lineStartsWithKeyword = ParserUtility::StartsWithKeyword(line, definedKeywords, keyword);
            if (lineStartsWithKeyword == false)
            {
                continue;
            }

            string::size_type x = line.find(':');
            if (x == string::npos)
            {
                prefix = keyword;

                line.erase(0, prefix.length());
                boost::trim(line);

                msg = line;
            }
            else
            {
                prefix = line.substr(0, x);

                string tmpMsg = line.substr(x + 1);
                boost::trim(tmpMsg);
                msg = tmpMsg;
            }

            boost::trim(prefix);
            boost::to_upper(prefix);
        }

        switch (section)
        {
            case CurrentSection::AHeader:
            {
                if(prefix == "ABSENDER")
                {
                    operation->SetAbsender(msg);
                }
                else if(prefix == "TERMIN" )
                {
                    operation->SetTermin(msg);
                }
                else if (prefix == "EINSATZNUMMER")
                {
                    operation->SetOperationNumber(msg);
                }

                break;
            }

            case CurrentSection::BMitteiler:
            {
                if(prefix == "NAME")
                {
                    operation->SetMessenger(msg);
                }

                break;
            }

            case CurrentSection::CEinsatzort:
            {
                PropertyLocation& einsatzort = operation->GetEinsatzortInternal();

                if(prefix == "STRAßE")
                {
                    string street, streetNumber, appendix;

                    ParserUtility::AnalyzeStreetLine(msg, street, streetNumber, appendix);

                    operation->SetEinsatzortZusatz(appendix);
                    einsatzort.SetStreet(street);
                    einsatzort.SetStreetNumber(streetNumber);
                }
                else if(prefix == "ORT")
                {
                    string zipCode = ParserUtility::ReadZipCodeFromCity(msg);
                    einsatzort.SetZipCode(zipCode);

                    if (zipCode.empty())
                    {
                        boost::trim(zipCode);

                        if(zipCode.empty())
                        {
                            //Logger.Instance.LogFormat(LogType.Warning, this, "Could not find a zip code for city '{0}'. Route planning may fail or yield wrong results!", operation.Einsatzort.City);
                        }
                    }

                    string city = msg.erase(0, zipCode.length());
                    boost::trim(city);
                    einsatzort.SetCity(city);

                    // The City-text often contains a dash after which the administrative city appears multiple times (like "City A - City A City A").
                    // However we can (at least with google maps) omit this information without problems!
                    string::size_type dashIndex = einsatzort.GetCity().find(" - ");
                    if (dashIndex != string::npos)
                    {
                        // Ignore everything after the dash
                        city = city.substr(0, dashIndex);
                        einsatzort.SetCity(city);
                    }
                }
                else if (prefix == "OBJEKT")
                {
                    einsatzort.SetProperty(msg);
                }
                else if(prefix == "PLANNUMMER")
                {
                    operation->SetEinsatzortPlannummer(msg);
                }
                else if (prefix == "STATION")
                {
                    operation->SetEinsatzortStation(msg);
                }

                break;
            }

            case CurrentSection::DZielort:
            {
                PropertyLocation& zielort = operation->GetZielortInternal();

                if(prefix == "STRAßE")
                {
                    string street, streetNumber, appendix;

                    ParserUtility::AnalyzeStreetLine(msg, street, streetNumber, appendix);

                    operation->SetZielortZusatz(appendix);
                    zielort.SetStreet(street);
                    zielort.SetStreetNumber(streetNumber);
                }
                else if (prefix == "ORT")
                {
                    string plz = ParserUtility::ReadZipCodeFromCity(msg);
                    zielort.SetZipCode(plz);

                    string city = msg.erase(0, plz.length());
                    boost::trim(city);
                    zielort.SetCity(city);
                }
                else if (prefix == "OBJEKT")
                {
                    zielort.SetProperty(msg);
                }
                else if (prefix == "STATION")
                {
                    operation->SetZielortStation(msg);
                }

                break;
            }

            case CurrentSection::EEinsatzgrund:
            {
                OperationKeywords& keywords = operation->GetKeywordsInternal();

                if(prefix =="SCHLAGW.")
                {
                    keywords.SetKeyword(msg);
                }
                else if(prefix == "STICHWORT B")
                {
                    keywords.SetB(msg);
                }
                else if(prefix == "STICHWORT R")
                {
                    keywords.SetR(msg);
                }
                else if(prefix == "STICHWORT S")
                {
                    keywords.SetS(msg);
                }
                else if(prefix == "STICHWORT T")
                {
                    keywords.SetT(msg);
                }
                else if(prefix == "PRIO.")
                {
                    operation->SetPriority(msg);
                }

                break;
            }

            case CurrentSection::FEinsatzmittel:
            {
                string upperLine = boost::to_upper_copy(line);

                if (boost::starts_with(upperLine, "EINSATZMITTEL"))
                {
                    msg = ParserUtility::GetMessageText(line, "Einsatzmittel");
                    last->SetFullName(msg);
                }
                else if (boost::starts_with(upperLine, "ALARMIERT") && msg.empty() == false)
                {
                    msg = ParserUtility::GetMessageText(line, "Alarmiert");

                    boost::posix_time::ptime dt = ParserUtility::TryGetTimestampFromMessage(msg, operation->GetTimestamp());
                    last->SetTimestamp(dt);
                }
                else if (boost::starts_with(upperLine, "GEFORDERTE AUSSTATTUNG"))
                {
                    msg = ParserUtility::GetMessageText(line, "Geforderte Ausstattung");
                    boost::trim(msg);

                    if (msg.empty() == false)
                    {
                        last->AddRequestedEquipment(msg);
                    }

                    operation->AddResource(last);
                    last = new OperationResource();
                }

                break;
            }

            case CurrentSection::GBemerkung:
            {
                string comment = operation->GetComment() + msg + "\n";
                operation->SetComment(comment);

                break;
            }

            case CurrentSection::HFooter:
                // The footer can be ignored completely.
                break;
            default:
                break;
        }
    }

    string comment = operation->GetComment();
    ParserUtility::RemoveTrailingNewline(comment);
    operation->SetComment(comment);

    delete last;
    
    return operation;
}

bool IlsAnsbach::GetSection(const string& line, CurrentSection &section, bool &keywordsOnly)
{
    if (line.find("MITTEILER") != string::npos)
    {
        section = CurrentSection::BMitteiler;
        keywordsOnly = true;
        return true;
    }

    if (line.find("EINSATZORT") != string::npos)
    {
        section = CurrentSection::CEinsatzort;
        // Is not true but only works that way.
        keywordsOnly = true;
        return true;
    }

    if (line.find("ZIELORT") != string::npos)
    {
        section = CurrentSection::DZielort;
        // Is not true but only works that way.
        keywordsOnly = true;
        return true;
    }

    if (line.find("EINSATZGRUND") != string::npos)
    {
        section = CurrentSection::EEinsatzgrund;
        keywordsOnly = true;
        return true;
    }

    if (line.find("EINSATZMITTEL") != string::npos)
    {
        section = CurrentSection::FEinsatzmittel;
        keywordsOnly = true;
        return true;
    }

    if (line.find("BEMERKUNG") != string::npos)
    {
        section = CurrentSection::GBemerkung;
        keywordsOnly = false;
        return true;
    }

    if (line.find("ENDE FAX") != string::npos)
    {
        section = CurrentSection::HFooter;
        keywordsOnly = false;
        return true;
    }

    return false;
}

CurrentSection IlsAnsbach::GetSection(const string& line)
{
    CurrentSection section = CurrentSection::Undefined;
    
    vector<string>::const_iterator it_found = find_if(sectionNames.begin(), sectionNames.end(), [&line](string s) -> 
                                                bool { return (line.find(s) != string::npos); });
    
    if(it_found != sectionNames.end())
    {
        map<string, CurrentSection>::const_iterator iValue = sectionResolver.find(*it_found);
        if(iValue == sectionResolver.end())
        {
            throw runtime_error("The section '" + *it_found + "' was not defined");
        }
        
        section = iValue->second;
    }
    
    return section;
}

bool IlsAnsbach::IsSectionBegin(const string& line)
{
    vector<string>::const_iterator it_found = find_if(sectionNames.begin(), sectionNames.end(), [&line](string s) -> 
                                                bool { return (line.find(s) != string::npos); });
    
    if(it_found != sectionNames.end())
    {
        return true;
    }
    
    return false;
}

void IlsAnsbach::ParseLineWithKeyword(string line, string& prefix, string& value)
{
    string keyword = "";
    value = "";
        
    bool lineStartsWithKeyword = ParserUtility::StartsWithKeyword(line, definedKeywords, keyword);
    if (lineStartsWithKeyword == false)
    {
        return;
    }
    
    string::size_type x = line.find(':');
    if (x == string::npos)
    {
        prefix = keyword;

        line.erase(0, prefix.length());
        
        value = line;
    }
    else
    {
        prefix = line.substr(0, x);
        value = line.substr(x + 1);
    }

    boost::trim(prefix);
    boost::to_upper(prefix);
    boost::trim(value);
}

void IlsAnsbach::ReadFaxHeader(vector<string>& lines, Operation* operation)
{
    if(lines.size() == 0)
    {
        return;
    }
    
    boost::posix_time::ptime currentTimestamp = operation->GetTimestamp();
    boost::posix_time::ptime parsedTime = ParserUtility::ReadFaxTimestamp(lines.front(), currentTimestamp);
    operation->SetTimestamp(parsedTime);
    
    int itemsToDelete = 0;
    
    for(string& line : lines)
    {
        bool isSection = IsSectionBegin(line);
        
        // if the next section starts end here
        if(isSection)
        {
            break;
        }
        
        itemsToDelete++;
        
        string value = line;
        string prefix = "";
        
        ParseLineWithKeyword(line, prefix, value);
        
        if(prefix == "ABSENDER")
        {
            operation->SetAbsender(value);
        }
        else if(prefix == "TERMIN" )
        {
            operation->SetTermin(value);
        }
        else if (prefix == "EINSATZNUMMER")
        {
            operation->SetOperationNumber(value);
        }
    }
    
    if(itemsToDelete > 0)
    {
        // remove line from vector after parsing
        lines.erase(lines.begin(), lines.begin() + itemsToDelete);
    }
}

void IlsAnsbach::ReadMitteiler(vector<string>& lines, Operation* operation)
{
    if(lines.size() == 0)
    {
        return;
    }
    
    int itemsToDelete = 0;
    
    for(string& line : lines)
    {
        bool isSection = IsSectionBegin(line);
        
        // if the next section starts end here
        if(isSection)
        {
            break;
        }
        
        itemsToDelete++;
        
        string value = line;
        string prefix = "";
        
        ParseLineWithKeyword(line, prefix, value);
        
        if(prefix == "NAME")
        {
            operation->SetMessenger(value);
        }
    }
    
    if(itemsToDelete > 0)
    {
        // remove line from vector after parsing
        lines.erase(lines.begin(), lines.begin() + itemsToDelete);
    }
}

void IlsAnsbach::ReadEinsatzort(vector<string>& lines, Operation* operation)
{
    if(lines.size() == 0)
    {
        return;
    }
    
    int itemsToDelete = 0;
    PropertyLocation& einsatzort = operation->GetEinsatzortInternal();
    
    for(string& line : lines)
    {
        bool isSection = IsSectionBegin(line);
        
        // if the next section starts end here
        if(isSection)
        {
            break;
        }
        
        itemsToDelete++;
        
        string value = line;
        string prefix = "";
        
        ParseLineWithKeyword(line, prefix, value);
        
        if(prefix == "STRAßE")
        {
            string street, streetNumber, appendix;

            ParserUtility::AnalyzeStreetLine(value, street, streetNumber, appendix);

            operation->SetEinsatzortZusatz(appendix);
            einsatzort.SetStreet(street);
            einsatzort.SetStreetNumber(streetNumber);
        }
        else if(prefix == "ORT")
        {
            string zipCode = ParserUtility::ReadZipCodeFromCity(value);
            einsatzort.SetZipCode(zipCode);

            if (zipCode.empty())
            {
                boost::trim(zipCode);

                if(zipCode.empty())
                {
                    //Logger.Instance.LogFormat(LogType.Warning, this, "Could not find a zip code for city '{0}'. Route planning may fail or yield wrong results!", operation.Einsatzort.City);
                }
            }

            string city = value.erase(0, zipCode.length());
            boost::trim(city);
            einsatzort.SetCity(city);

            // The City-text often contains a dash after which the administrative city appears multiple times (like "City A - City A City A").
            // However we can (at least with google maps) omit this information without problems!
            string::size_type dashIndex = einsatzort.GetCity().find(" - ");
            if (dashIndex != string::npos)
            {
                // Ignore everything after the dash
                city = city.substr(0, dashIndex);
                einsatzort.SetCity(city);
            }
        }
        else if (prefix == "OBJEKT")
        {
            einsatzort.SetProperty(value);
        }
        else if(prefix == "PLANNUMMER")
        {
            operation->SetEinsatzortPlannummer(value);
        }
        else if (prefix == "STATION")
        {
            operation->SetEinsatzortStation(value);
        }
    }
    
    if(itemsToDelete > 0)
    {
        // remove line from vector after parsing
        lines.erase(lines.begin(), lines.begin() + itemsToDelete);
    }
}

void IlsAnsbach::ReadZielort(vector<string>& lines, Operation* operation)
{
    if(lines.size() == 0)
    {
        return;
    }
    
    int itemsToDelete = 0;
    PropertyLocation& zielort = operation->GetZielortInternal();
    
    for(string& line : lines)
    {
        bool isSection = IsSectionBegin(line);
        
        // if the next section starts end here
        if(isSection)
        {
            break;
        }
        
        itemsToDelete++;
        
        string value = line;
        string prefix = "";
        
        ParseLineWithKeyword(line, prefix, value);
                
        if(prefix == "STRAßE")
        {
            string street, streetNumber, appendix;

            ParserUtility::AnalyzeStreetLine(value, street, streetNumber, appendix);

            operation->SetZielortZusatz(appendix);
            zielort.SetStreet(street);
            zielort.SetStreetNumber(streetNumber);
        }
        else if (prefix == "ORT")
        {
            string plz = ParserUtility::ReadZipCodeFromCity(value);
            zielort.SetZipCode(plz);

            string city = value.erase(0, plz.length());
            boost::trim(city);
            zielort.SetCity(city);
        }
        else if (prefix == "OBJEKT")
        {
            zielort.SetProperty(value);
        }
        else if (prefix == "STATION")
        {
            operation->SetZielortStation(value);
        }
    }
    
    if(itemsToDelete > 0)
    {
        // remove line from vector after parsing
        lines.erase(lines.begin(), lines.begin() + itemsToDelete);
    }
}

void IlsAnsbach::ReadEinsatzgrund(vector<string>& lines, Operation* operation)
{
    if(lines.size() == 0)
    {
        return;
    }
    
    int itemsToDelete = 0;
    OperationKeywords& keywords = operation->GetKeywordsInternal();
    
    for(string& line : lines)
    {
        bool isSection = IsSectionBegin(line);
        
        // if the next section starts end here
        if(isSection)
        {
            break;
        }
        
        itemsToDelete++;
        
        string value = line;
        string prefix = "";
                
        if(prefix =="SCHLAGW.")
        {
            keywords.SetKeyword(value);
        }
        else if(prefix == "STICHWORT B")
        {
            keywords.SetB(value);
        }
        else if(prefix == "STICHWORT R")
        {
            keywords.SetR(value);
        }
        else if(prefix == "STICHWORT S")
        {
            keywords.SetS(value);
        }
        else if(prefix == "STICHWORT T")
        {
            keywords.SetT(value);
        }
        else if(prefix == "PRIO.")
        {
            operation->SetPriority(value);
        }
    }
    
    if(itemsToDelete > 0)
    {
        // remove line from vector after parsing
        lines.erase(lines.begin(), lines.begin() + itemsToDelete);
    }
}

void IlsAnsbach::ReadEinsatzmittel(vector<string>& lines, Operation* operation)
{
    if(lines.size() == 0)
    {
        return;
    }
    
    int itemsToDelete = 0;
    OperationResource *operationResource = nullptr;
    
    for(string& line : lines)
    {
        bool isSection = IsSectionBegin(line);
        
        // if the next section starts end here
        if(isSection)
        {
            break;
        }
        
        itemsToDelete++;
        
        string value = line;
        string prefix = "";
        
        ParseLineWithKeyword(line, prefix, value);
        
        string upperLine = boost::to_upper_copy(line);

        if (boost::starts_with(upperLine, "EINSATZMITTEL"))
        {
            if(operationResource == nullptr)
            {
                operationResource = new OperationResource();
            }
            
            value = ParserUtility::GetMessageText(line, "Einsatzmittel");
            operationResource->SetFullName(value);
        }
        else if (boost::starts_with(upperLine, "ALARMIERT") && value.empty() == false)
        {
            value = ParserUtility::GetMessageText(line, "Alarmiert");

            boost::posix_time::ptime dt = ParserUtility::TryGetTimestampFromMessage(value, operation->GetTimestamp());
            
            if(operationResource != nullptr)
            {
                operationResource->SetTimestamp(dt);
            }
        }
        else if (boost::starts_with(upperLine, "GEFORDERTE AUSSTATTUNG"))
        {
            value = ParserUtility::GetMessageText(line, "Geforderte Ausstattung");
            boost::trim(value);

            if (value.empty() == false)
            {
                operationResource->AddRequestedEquipment(value);
            }

            if(operationResource != nullptr)
            {
                operation->AddResource(operationResource);
                operationResource = nullptr;
            }
        }
    }
    
    if(itemsToDelete > 0)
    {
        // remove line from vector after parsing
        lines.erase(lines.begin(), lines.begin() + itemsToDelete);
    }
}

void IlsAnsbach::ReadBemerkung(vector<string>& lines, Operation* operation)
{
    if(lines.size() == 0)
    {
        return;
    }
    
    int itemsToDelete = 0;
    string comment;
    
    for(string& line : lines)
    {
        bool isSection = IsSectionBegin(line);
        
        // if the next section starts end here
        if(isSection)
        {
            break;
        }
        
        // remove line from vector after parsing
        lines.pop_back();
        
        comment += boost::trim_copy(line);        
    }
    
    if(itemsToDelete > 0)
    {
        // remove line from vector after parsing
        lines.erase(lines.begin(), lines.begin() + itemsToDelete);
    }
    
    boost::trim(comment);
    operation->SetComment(comment);
}
