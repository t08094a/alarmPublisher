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

#include <sstream>

#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include <boost/property_tree/ini_parser.hpp>

ConfigReader::ConfigReader()
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
    const string filename = "alarmPublisher.config";
    
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
            string msg = "The config file \"" + etcFullPath + "\" does not exist -> kill";
            BOOST_LOG_TRIVIAL(fatal) << msg;
            
            throw runtime_error(msg);
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
    
    try
    {
        boost::property_tree::ini_parser::read_ini(resultFilename, pt);
    }
    catch(const boost::property_tree::ini_parser_error &e)
    {
        BOOST_LOG_TRIVIAL(error) << e.what();
    }
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

vector<string> ConfigReader::GetTelephonNumbers(const string& distributionList)
{
    // TODO: get telephone numbers based on distributionList. this defines the section in the config
    vector<string> numbers;
    
    auto recipientNode = pt.get_child_optional("Recipients");
    if(recipientNode.is_initialized() == false)
    {
        BOOST_LOG_TRIVIAL(error) << "Unable to read the telephone numbers. The root node is not initialized";
        
        return numbers;
    }
    
    BOOST_LOG_TRIVIAL(info) << "Found the following SMS recipients:";
    
    for(const pair<string, boost::property_tree::ptree> &kv : recipientNode.get())
    {
        // v.first is the name of the child
        // v.second is the child tree
        string name = kv.first;
        string number = kv.second.get_value<string>();
    
        BOOST_LOG_TRIVIAL(info) << name << ": " << number;
    
        numbers.push_back(number);
    }
    
    return numbers;
}
