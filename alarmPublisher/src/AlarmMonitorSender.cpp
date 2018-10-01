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

#include "AlarmMonitorSender.h"
#include "ConfigReader.h"
#include "restclient-cpp/restclient.h"

AlarmMonitorSender::AlarmMonitorSender() : endpoint(), api(), initialized(false)
{
    BOOST_LOG_TRIVIAL(debug) << "Create AlarmMonitorSender";
    
    Initialize();
}

AlarmMonitorSender::~AlarmMonitorSender()
{
}

void AlarmMonitorSender::Initialize()
{
    BOOST_LOG_TRIVIAL(info) << "Initialize AlarmMonitorSender from config";
    
    // read config settings for REST endpoint
    endpoint = ConfigReader::GetInstance().Get("AlarmMonitor.endpoint");
    api = ConfigReader::GetInstance().Get("AlarmMonitor.api");
    
    BOOST_LOG_TRIVIAL(info) << "use endpoint: " << endpoint;
    BOOST_LOG_TRIVIAL(info) << "use api: " << api;
    
    
    initialized = true;
}

void AlarmMonitorSender::Send(IOperation& operation) const
{
    if(!initialized)
    {
        BOOST_LOG_TRIVIAL(error) << "AlarmMonitorSender is not initialized -> unable to send a message";
        return;
    }
    
    BOOST_LOG_TRIVIAL(info) << "Send alarm info to alarmmonitors REST api";
    
    
    string url = endpoint + api;
    
    string data = GetDataAsJson();
    
    RestClient::Response r = RestClient::post(url, "application/json", data);
    
    BOOST_LOG_TRIVIAL(info) << "REST response:" << endl << r.body;
}

string AlarmMonitorSender::GetDataAsJson() const
{
    stringstream ss;

    ss << "{" << endl;
    ss << "  \"foo\": \"bla\"" << endl;
    ss << "}";

    string s = ss.str();

    BOOST_LOG_TRIVIAL(debug) << "Created JSON message:" << endl << s;

    return s;
}
