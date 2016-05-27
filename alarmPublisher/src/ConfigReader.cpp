/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2016  Jochen Fähnlein <jochen.faehnlein@ff-ipsheim.de>
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
 *
 */

#include "ConfigReader.h"

#define CSV_IO_NO_THREAD // use without threading
#include "fast-cpp-csv-parser/csv.h"

#include <sstream>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include <boost/property_tree/ini_parser.hpp>

ConfigReader::ConfigReader() : telephoneNumbersFilename()
{
    BOOST_LOG_TRIVIAL(debug) << "Create ConfigReader";
    
    Initialize();
}

ConfigReader::~ConfigReader()
{
    // nop
}

void ConfigReader::Initialize()
{
    const string configFilename = "alarmPublisher.config";
    string resultConfigFilename = SearchConfigFile(configFilename, true);
    
    try
    {
        boost::property_tree::ini_parser::read_ini(resultConfigFilename, pt);
    }
    catch(const boost::property_tree::ini_parser_error &e)
    {
        BOOST_LOG_TRIVIAL(error) << e.what();
    }
    
    const string telephoneFilename = "telephoneNumbers.csv";
    this->telephoneNumbersFilename = SearchConfigFile(telephoneFilename, true);
}

string ConfigReader::SearchConfigFile(string filename, bool throwIfMissing)
{
    boost::filesystem::path currentPath(boost::filesystem::current_path());
    BOOST_LOG_TRIVIAL(info) << "Search config file \"" << filename << "\" in path " << currentPath;
    
    string resultFilename;
    
    if ( !boost::filesystem::exists( filename ) )
    {
        // config file does not exist in current path -> try read it from /etc
        string etcFullPath = "/etc/alarmPublisher/" + filename;
        
        BOOST_LOG_TRIVIAL(warning) << "The config file \"" << filename << "\" does not exist -> try to read it from \"" << etcFullPath << "\"";
        
        if ( !boost::filesystem::exists( etcFullPath ) )
        {
            string msg = "The config file \"" + etcFullPath + "\" does not exist";
            
            if(throwIfMissing)
            {
                msg += " -> kill";
                
                BOOST_LOG_TRIVIAL(fatal) << msg;
                
                throw runtime_error(msg);
            }
            else
            {
                BOOST_LOG_TRIVIAL(warning) << msg;
            }
        }
        else
        {
            resultFilename = etcFullPath;
        }
    }
    else
    {
        resultFilename = filename;
    }
    
    return resultFilename;
}

string ConfigReader::Get(const string& path) const
{
    string result;
    
    try
    {
        result = pt.get<string>(path);
    }
    catch(const boost::property_tree::ini_parser_error &e)
    {
        BOOST_LOG_TRIVIAL(error) << e.what();
    }
    
    return result;
}

vector<string> ConfigReader::GetTelephoneNumbers()
{
    vector<string> numbers;
    
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    string daytimeColumnName = GetTelephoneColumnNameBasedOnDateTime(now);
    
    BOOST_LOG_TRIVIAL(info) << "Search telephone numbers with 'x' in column: " << daytimeColumnName;
    
    using namespace io;
    CSVReader<3,trim_chars<' ', '\t'>,no_quote_escape<';'>,ignore_overflow,single_and_empty_line_comment<'#'>> in(telephoneNumbersFilename);
    in.read_header(ignore_extra_column, "Empfänger", "Telefonnummer", daytimeColumnName);
    
    string name1;
    string telNumber;
    string active;
    
    while(in.read_row(name1, telNumber, active))
    {
        boost::algorithm::to_lower(active);
        
        if(active != "x")
        {
            continue;
        }
        
        BOOST_LOG_TRIVIAL(info) << name1 << ": \t" << telNumber;
        numbers.push_back(telNumber);
    }
        
    return numbers;
}

string ConfigReader::GetTelephoneColumnNameBasedOnDateTime(const boost::posix_time::ptime& currentTime)
{
    // "Wochentag Tag", "Wochentag Nacht", "Wochenende Tag", "Wochenende Nacht"
    // abhängig der aktuellen Uhrzeit, spalte muss ein 'x' enthalten
    
    string columnName;
    
    short dayOfWeek = currentTime.date().day_of_week().as_number(); // (0==Sunday, 1==Monday, etc)

    if(dayOfWeek == 0 || dayOfWeek == 6)
    {
        columnName = "Wochenende";
    }
    else
    {
        columnName = "Wochentag";
    }
    
    columnName += " ";
    
    int currentHour = static_cast<int>(currentTime.time_of_day().hours());
    
    if(currentHour >= 7 && currentHour <= 17)
    {
        columnName += "Tag";
    }
    else
    {
        columnName += "Nacht";
    }
    
    return columnName;
}