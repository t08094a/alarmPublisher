/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2017  <copyright holder> <email>
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

#include "CmGateway.h"
#include "ConfigReader.h"
#include "UrlEncoding.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <boost/log/trivial.hpp>
#include <boost/lexical_cast.hpp>

#include <curl/curl.h>

// Define our struct for accepting LCs output
struct BufferStruct
{
    char* buffer;
    size_t size;
};

// This is the function we pass to LC, which writes the output to a BufferStruct
static size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
    size_t realsize = size * nmemb;

    struct BufferStruct* mem = (struct BufferStruct*) data;

    mem->buffer = (char*) realloc(mem->buffer, mem->size + realsize + 1);

    if(mem->buffer)
    {
        memcpy(&(mem->buffer[mem->size]), ptr, realsize);
        mem->size += realsize;
        mem->buffer[mem->size] = 0;
    }
    else
    {
        // out of memory
        BOOST_LOG_TRIVIAL(error) << "not enough memory (realloc returned NULL)";
    }

    return realsize;
}


const string CmGateway::name = "cm";

CmGateway::CmGateway() : productToken("00000000-0000-0000-0000-000000000000"), from("FF Alarm")
{
    BOOST_LOG_TRIVIAL(info) << "Create cm gateway";

    InitializeFromConfig();
}

CmGateway::CmGateway(const CmGateway& other)
{
    productToken = other.productToken;
    from = other.from;
}

CmGateway::~CmGateway()
{
    // nop
}

CmGateway& CmGateway::operator=(const CmGateway& other)
{

}

const string CmGateway::GetGatewayName()
{
    return name;
}

string CmGateway::GetName() const
{
    return name;
}

void CmGateway::SendMessage(const string& msg, bool debug)
{
    BOOST_LOG_TRIVIAL(info) << "Send message via cm";

    CURL *curl;
    curl = curl_easy_init();

    if(curl)
    {
        BOOST_LOG_TRIVIAL(info) << "curl for cm initialized";

        vector<string> telephoneNumbers = ConfigReader::GetInstance().GetTelephoneNumbers(true);

        string msgToSend = CreateMessage(msg, telephoneNumbers);

        string targetUrl = "https://sgw01.cm.nl/gateway.ashx";

        curl_easy_setopt(curl, CURLOPT_URL, targetUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTPS);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);                        /* Now specify we want to POST data */
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, msgToSend.c_str());   /* pass in a pointer to the data - libcurl will not copy */
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");  /* some servers don't like requests that are made without a user-agent field, so we provide one */

        struct curl_slist *headers=NULL;
        curl_slist_append(headers, "Content-Type: application/xml");
        curl_slist_append(headers, "charset: utf-8");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);


        struct BufferStruct output; // Create an instance of out BufferStruct to accept LCs output
        output.buffer = NULL;
        output.size = 0;

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback); /* get the post result, Passing the function pointer to LC */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&output); // Passing our BufferStruct to LC

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


        // Perform the request, result will get the return code
        CURLcode result = curl_easy_perform(curl);

        // Check for errors
        if(result != CURLE_OK)
        {
            BOOST_LOG_TRIVIAL(error) << "curl_easy_perform() failed: " << curl_easy_strerror(result);

            BOOST_LOG_TRIVIAL(error) << "curl_easy_perform() failed: " << output.buffer;

        }

        if( output.buffer )
        {
            free (output.buffer);
            output.buffer = 0;
            output.size = 0;
        }

        curl_slist_free_all(headers); /* free the header list */
        curl_easy_cleanup(curl);
    }
    else
    {
        BOOST_LOG_TRIVIAL(error) << "Unable to initialize curl for sending message with cm";
    }
}

string CmGateway::CreateMessage(const string& msg, vector<string>& telephoneNumbers)
{
    BOOST_LOG_TRIVIAL(info) << "Create cm message";

    string fromEncoded = from;
    string msgEncoded = msg;

    stringstream ss;

    ss << "<?xml version=\"1.0\"?>" << endl;
    ss << "<MESSAGES>" << endl;
    ss << "  <AUTHENTICATION>" << endl;
    ss << "    <PRODUCTTOKEN>" << productToken << "</PRODUCTTOKEN>" << endl;
    ss << "  </AUTHENTICATION>" << endl;
    ss << "  <MSG>" << endl;
    ss << "    <FROM>" << fromEncoded << "</FROM>" << endl;

    for(string const& number : telephoneNumbers)
    {
        ss << "    <TO>" << number << "</TO>";
    }

    ss << "    <BODY>" << msgEncoded << "</BODY>" << endl;
    ss << "  </MSG>" << endl;
    ss << "</MESSAGES>";

    string s = ss.str();

    BOOST_LOG_TRIVIAL(debug) << "Created message:" << endl << s;

    return s;
}

void CmGateway::InitializeFromConfig()
{
    BOOST_LOG_TRIVIAL(info) << "Initialize cm from config";

/*
==========
config.ini
==========
[cm]
ProductToken = 9VUtGMBbe151aaf4E38BNnp
From = FF Alarm
*/

    productToken = ConfigReader::GetInstance().Get("cm.ProductToken");
    from = ConfigReader::GetInstance().Get("cm.From");
}
