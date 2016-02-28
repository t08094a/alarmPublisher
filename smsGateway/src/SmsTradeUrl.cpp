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

#include "SmsTradeUrl.h"
#include "ConfigReader.h"
#include "UrlEncoding.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>

#include <boost/asio/socket_base.hpp>
#include <boost/log/trivial.hpp>

const string SmsTradeUrl::name = "SmsTradeUrl";

SmsTradeUrl::SmsTradeUrl() : io_service(), gateway("gateway.smstrade.de"), key(), route("direct"), from("FF Alarm")
{
    InitializeFromConfig();
}

SmsTradeUrl::SmsTradeUrl(string gatewayKey) : SmsTradeUrl()
{
    key = gatewayKey;
}


SmsTradeUrl::SmsTradeUrl (const SmsTradeUrl& other)
{
    // TODO
}

SmsTradeUrl::~SmsTradeUrl()
{
    io_service.stop();
}

SmsTradeUrl& SmsTradeUrl::operator= (const SmsTradeUrl& other)
{
    // TODO
}

const string SmsTradeUrl::GetGatewayName()
{
    return name;
}

string SmsTradeUrl::GetName() const
{
    return name;
}

void SmsTradeUrl::InitializeFromConfig()
{
    /*
==========
config.ini
==========
[SmsTrade]
Key = 9VUtGMBbe151aaf4E38BNnp
Route = direct
From = FF Alarm
*/
    
    key = ConfigReader::GetInstance().Get("SmsTrade.Key");
    route = ConfigReader::GetInstance().Get("SmsTrade.Route");
    from = ConfigReader::GetInstance().Get("SmsTrade.From");
}

void SmsTradeUrl::SendMessage (const string& receivers, const string& msg, bool debug)
{
    try
    {
        using boost::asio::ip::tcp;
        
        string protocol = "http";
        
        // get a list of endpoints corresponding to the server name.
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(gateway, protocol);
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        tcp::resolver::iterator end;
        
        // try each endpoint until we successfully establish a connection.
        boost::system::error_code error = boost::asio::error::host_not_found;
        
        tcp::socket socket(io_service);
        while (error && endpoint_iterator != end)
        {
            socket.close();
            socket.connect(*endpoint_iterator++, error);
        }
        if (error)
        {
            throw boost::system::system_error(error);
        }
        
        // form the request. We specify the "Connection: close" header so that the
        // server will close the socket after transmitting the response. This will
        // allow us to treat all data up until the EOF as the content.
        boost::asio::streambuf request;
        ostream request_stream(&request);
        
        string data = CreateParameterString(msg, receivers, debug);
        int dataLength = data.length();
        
        string endLine = "\r\n";
        
        request_stream << "POST / HTTP/1.1" << endLine;
        request_stream << "Host: " << gateway << endLine;
        request_stream << "Accept: */*" << endLine;
        request_stream << "Connection: close" << endLine;
        request_stream << "Content-Type: application/x-www-form-urlencoded" << endLine;
        request_stream << "Content-Length: " << dataLength << endLine;
        request_stream << endLine; // empty line before data
        request_stream << data;
        
        // send the request.
        boost::asio::write(socket, request);
        
        // read the response status line. The response streambuf will automatically
        // grow to accommodate the entire line. The growth may be limited by passing
        // a maximum size to the streambuf constructor.
        boost::asio::streambuf response;
        boost::asio::read_until(socket, response, "\r\n");
        
        // check that response is OK.
        istream response_stream(&response);
        string http_version;
        response_stream >> http_version;
        unsigned int status_code;
        response_stream >> status_code;
        string status_message;
        getline(response_stream, status_message);
        
        if (!response_stream || http_version.substr(0, 5) != "HTTP/")
        {
            cout << "Invalid response" << endl;
            //BOOST_LOG_TRIVIAL(error) << "Invalid response" << endl;
        }
        else if (status_code != 200)
        {
            cout << "Response returned with status code " << status_code << endl;
            //BOOST_LOG_TRIVIAL(error) << "Response returned with status code " << status_code << endl;
        }
        else
        {
            // read the response headers, which are terminated by a blank line.
            boost::asio::read_until(socket, response, "\r\n\r\n");

            // process the response headers.
            string header;
            while (getline(response_stream, header) && header != "\r")
            {
                cout << header << endl;
                //BOOST_LOG_TRIVIAL(info) << header << endl;
            }

            // write whatever content we already have to output.
            if (response.size() > 0)
            {
                cout << "Response: " << &response << endl;
                //BOOST_LOG_TRIVIAL(info) << "Response: " << &response << endl;
            }

            // read until EOF, writing data to output as we go.
            boost::system::error_code error;
            while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error))
            {
                cout << &response;
                //BOOST_LOG_TRIVIAL(info) << &response;
            }

            if (error != boost::asio::error::eof)
            {
                throw boost::system::system_error(error);
            }
        }
    }
    catch (std::exception& e)
    {
        cout << "Exception: " << e.what() << endl;
        // BOOST_LOG_TRIVIAL(error) << "Exception: " << e.what() << endl;
    }
}

string SmsTradeUrl::CreateParameterString (const string& msg, const string& receivers, bool debug)
{
    string encodedMsg = UrlEncoding::Encode(msg);
    string encodedFrom = UrlEncoding::Encode(from);
    
    const char separator = '&';
    const char activate = '1';
    
    stringstream ss;
    
    if(debug)
    {
        ss << "debug=" << activate << separator;
    }
    
    ss << "key=" << key << separator;
    ss << "message=" << encodedMsg << separator;
    ss << "to=" << receivers << separator;
    ss << "from=" << encodedFrom << separator;
    ss << "route=" << route << separator;
    //ss << "count=" << activate << separator;
    ss << "dlr=" << activate << separator;
    ss << "messagetype=flash";
    
    return ss.str();
}
