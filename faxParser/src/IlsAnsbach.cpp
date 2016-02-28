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
#include "Operation.h"
#include "OperationKeywords.h"
#include "OperationResource.h"
#include "ParserUtility.h"
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

using namespace std;

IlsAnsbach::IlsAnsbach() 
{
    // nop
}

IlsAnsbach::IlsAnsbach ( const IlsAnsbach& other ) 
{
    // nop
}

IlsAnsbach::~IlsAnsbach() 
{
    // nop
}

IOperation* IlsAnsbach::Parse(const std::string& filename)
{
    std::ifstream ifs(filename.c_str(), std::ios::in | std::ios::binary);
    if(!ifs || ifs.is_open() == false)
    {
        cerr << "Error opening file \"" << filename << "\"" << endl;
        return nullptr;
    }
    
    cout << "Parsing file: " << filename << endl;

    std::vector<std::string> data;
    std::string line;
    
    while (std::getline(ifs, line))
    {
        data.push_back(line);
    }
    
    return Parse(data);
}

IOperation* IlsAnsbach::Parse(vector<string> lines)
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

