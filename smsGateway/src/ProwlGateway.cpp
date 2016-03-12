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

#include "../include/ProwlGateway.h"
#include "ConfigReader.h"

#include <curl/curl.h>

const string ProwlGateway::name = "Prowl";

ProwlGateway::ProwlGateway() : hostname(), port(), apiKey()
{
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
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.prowlapp.com/publicapi/add?apikey=9VUtGMBbe151aaf4E38BNnp&application=FF%20Alarm&event=Alarm&description=Hello%20World&priority=2");

        /*
        * If you want to connect to a site who isn't using a certificate that is
        * signed by one of the certs in the CA bundle you have, you can skip the
        * verification of the server's certificate. This makes the connection
        * A LOT LESS SECURE.
        *
        * If you have a CA cert for the server stored someplace else than in the
        * default bundle, then the CURLOPT_CAPATH option might come handy for
        * you.
        */ 
        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        
        /*
        * If the site you're connecting to uses a different host name that what
        * they have mentioned in their server certificate's commonName (or
        * subjectAltName) fields, libcurl will refuse to connect. You can skip
        * this check, but this will make the connection less secure.
        */ 
        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        
        curl_easy_setopt(curl, CURLOPT_HEADER, 0);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
        
        /* Perform the request, res will get the return code */
        CURLcode res = curl_easy_perform(curl);
        
        /* Check for errors */
        if(res != CURLE_OK)
        {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

void ProwlGateway::InitializeFromConfig()
{
    /*
    ==========
    config.ini
    ==========
    [Prowl]
    ApiKey = 9VUtGMBbe151aaf4E38BNnp
    */
    string priority = "[PRI2]";
    hostname = "prowl.weks.net";
    port = "443";
    string messageSize = "11400";
    string buffersize = "512";
    
    apiKey = priority + "9VUtGMBbe151aaf4E38BNnp";
    //apiKey = ConfigReader::GetInstance().Get("Prowl.ApiKey");
}

void ProwlGateway::BuildNotificationRequestUrl(string msg, bool debug)
{
    string protocol = "https";
    string url = "https://api.prowlapp.com/publicapi/add";
    string application = "FF%20Alarm";
    string event = "Alarm";
    string priority = 2;
    
    string s = protocol + "://" + url + "?apikey=" + apiKey + "&application=" + application + "&priority=" + priority + "&event=" + event + "&description=" + msg;
}
