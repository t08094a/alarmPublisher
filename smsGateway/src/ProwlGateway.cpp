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

#include "ProwlGateway.h"
#include "ConfigReader.h"
#include "UrlEncoding.h"

#include <iostream>
#include <sstream>

#include <boost/log/trivial.hpp>

#include <curl/curl.h>

const string ProwlGateway::name = "Prowl";

ProwlGateway::ProwlGateway() : apiKey(), url(), application(), event(), priority()
{
    BOOST_LOG_TRIVIAL(info) << "Create Prowl gateway";
    
    InitializeFromConfig();
}

ProwlGateway::~ProwlGateway()
{
    // nop
}

const string ProwlGateway::GetGatewayName()
{
    return name;
}

string ProwlGateway::GetName() const
{
    return name;
}

void ProwlGateway::SendMessage(const string& to, const string& msg, bool debug)
{
    CURL *curl;
    
    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    
    if(curl) 
    {
        const string notificationUrl = BuildNotificationRequestUrl(msg, debug);
        
        curl_easy_setopt(curl, CURLOPT_URL, notificationUrl.c_str());

        // If you want to connect to a site who isn't using a certificate that is
        // signed by one of the certs in the CA bundle you have, you can skip the
        // verification of the server's certificate. This makes the connection
        // A LOT LESS SECURE.
        // 
        // If you have a CA cert for the server stored someplace else than in the
        // default bundle, then the CURLOPT_CAPATH option might come handy for
        // you.

        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        
        // If the site you're connecting to uses a different host name that what
        // they have mentioned in their server certificate's commonName (or
        // subjectAltName) fields, libcurl will refuse to connect. You can skip
        // this check, but this will make the connection less secure.
        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        
        curl_easy_setopt(curl, CURLOPT_HEADER, 0);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
        
        // Perform the request, res will get the return code
        CURLcode res = curl_easy_perform(curl);
        
        // Check for errors
        if(res != CURLE_OK)
        {
            BOOST_LOG_TRIVIAL(error) << "curl_easy_perform() failed: " << curl_easy_strerror(res);
        }

        // always cleanup
        curl_easy_cleanup(curl);
    }
    else
    {
        BOOST_LOG_TRIVIAL(error) << "Unable to initialize curl for sending message to Prowl";
    }

    curl_global_cleanup();
}

void ProwlGateway::InitializeFromConfig()
{
    BOOST_LOG_TRIVIAL(info) << "Initialize Prowl from config";
    
    // ==========
    // config.ini
    // ==========
    // [Prowl]
    // ApiKey = xxx
    // Url = https://prowl.weks.net/publicapi/add
    // Application = FF Alarm
    // Event = Alarm
    // Priority = 2
    
    apiKey = ConfigReader::GetInstance().Get("Prowl.ApiKey");
    url = ConfigReader::GetInstance().Get("Prowl.Url");
    application = ConfigReader::GetInstance().Get("Prowl.Application");
    event = ConfigReader::GetInstance().Get("Prowl.Event");
    priority = ConfigReader::GetInstance().Get("Prowl.Priority");
}

const string ProwlGateway::BuildNotificationRequestUrl(const string& msg, bool debug)
{
    // url encode inputs
    string applicationEncoded = UrlEncoding::Encode(application);
    string eventEncoded = UrlEncoding::Encode(event);
    string msgEncoded = UrlEncoding::Encode(msg);
    
    const string appendChar = "&";
    
    stringstream ss;
    ss << url << "?";
    ss << "apikey=" << apiKey << appendChar;
    ss << "application=" << applicationEncoded << appendChar;
    ss << "priority=" << priority << appendChar;
    ss << "event=" << eventEncoded << appendChar;
    ss << "description=" << msgEncoded;
    
    string s = ss.str();
    return s;
}
