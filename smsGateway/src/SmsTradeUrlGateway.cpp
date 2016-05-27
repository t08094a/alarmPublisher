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

#include "SmsTradeUrlGateway.h"
#include "ConfigReader.h"
#include "UrlEncoding.h"

#include <fstream>
#include <iostream>

#include <boost/log/trivial.hpp>
#include <boost/algorithm/string/join.hpp>

#include <curl/curl.h>

const string SmsTradeUrlGateway::name = "SmsTradeUrl";

SmsTradeUrlGateway::SmsTradeUrlGateway() : url(), key(), route("direct"), from("FF Alarm")
{
    BOOST_LOG_TRIVIAL(info) << "Create SmsTrade URL gateway";
    
    InitializeFromConfig();
}

SmsTradeUrlGateway::SmsTradeUrlGateway(const SmsTradeUrlGateway& other)
{
    BOOST_LOG_TRIVIAL(info) << "Call Copy-CCtor SmsTrade URL gateway";
    
    url = other.url;
    key = other.key;
    route = other.route;
    from = other.from;
}

SmsTradeUrlGateway::~SmsTradeUrlGateway()
{
    // nop
}

const string SmsTradeUrlGateway::GetGatewayName()
{
    return name;
}

string SmsTradeUrlGateway::GetName() const
{
    return name;
}

void SmsTradeUrlGateway::SendMessage(const string& msg, bool debug)
{
    BOOST_LOG_TRIVIAL(info) << "Send message via SmsTrade URL";
    
    CURL *curl;
    
    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    
    if(curl) 
    {
        const string notificationUrl = BuildParameter(msg, debug);
        
        curl_easy_setopt(curl, CURLOPT_URL, "http://gateway.smstrade.de");

        curl_easy_setopt(curl, CURLOPT_HEADER, 1);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, notificationUrl.c_str());
//         curl_easy_setopt(curl, CURLOPT_TRANSFERTEXT, 1);
        
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
//         curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
        
        // Perform the request, res will get the return code
        CURLcode res = curl_easy_perform(curl);
        
        // Check for errors
        if(res != CURLE_OK)
        {
            BOOST_LOG_TRIVIAL(error) << "curl_easy_perform() failed: " << curl_easy_strerror(res);
        }
//         else
//         {
//             BOOST_LOG_TRIVIAL(info) << "response: " << curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE);
//         }

        // always cleanup
        curl_easy_cleanup(curl);
    }
    else
    {
        BOOST_LOG_TRIVIAL(error) << "Unable to initialize curl for sending message to SmsTrade URL";
    }

    curl_global_cleanup();
}

const string SmsTradeUrlGateway::BuildParameter(const string& msg, bool debug)
{
    vector<string> telephoneNumbersList = ConfigReader::GetInstance().GetTelephoneNumbers();
    string telephoneNumbers = boost::algorithm::join(telephoneNumbersList, ";");
    
    // url encode inputs
    string msgEncoded = UrlEncoding::Encode(msg);
    string fromEncoded = UrlEncoding::Encode(from);
    
    BOOST_LOG_TRIVIAL(info) << "encoded message:" << endl << msgEncoded;
    
    const string appendChar = "&";
    
    stringstream ss;
    ss << "key=" << key << appendChar;
    ss << "to=" << telephoneNumbers << appendChar;
    ss << "message=" << msgEncoded << appendChar;
    ss << "route=" << route << appendChar;
    ss << "from=" << fromEncoded; // << appendChar;
    
    if(debug)
    {
        ss << "debug=1" << appendChar;
    }
    
    ss << "message_id=1" << appendChar;
    ss << "cost=1" << appendChar;
    ss << "count=1" << appendChar;
    ss << "dlr=1" << appendChar;
    //ss << "charset=UTF-8" << appendChar;
    ss << "messagetype=flash" << appendChar;
    ss << "concat=1";
    
    string s = ss.str();
    
    BOOST_LOG_TRIVIAL(info) << "Created URL:" << endl << s;
    
    return s;
}

void SmsTradeUrlGateway::InitializeFromConfig()
{
     BOOST_LOG_TRIVIAL(info) << "Initialize SmsTrade URL from config";
    
/*
==========
config.ini
==========
[SmsTrade]
Key = 9VUtGMBbe151aaf4E38BNnp
Route = direct
From = FF Alarm
Url = https://gateway.smstrade.de/bulk
*/
    key = ConfigReader::GetInstance().Get("SmsTrade.Key");
    route = ConfigReader::GetInstance().Get("SmsTrade.Route");
    from = ConfigReader::GetInstance().Get("SmsTrade.From");
    url = ConfigReader::GetInstance().Get("SmsTrade.Url");
}

SmsTradeUrlGateway& SmsTradeUrlGateway::operator=(const SmsTradeUrlGateway& other)
{
    if (this == &other)
    {
        return *this; // handle self assignment
    }
    
    url = other.url;
    key = other.key;
    route = other.route;
    from = other.from;
        
    return *this;
}
