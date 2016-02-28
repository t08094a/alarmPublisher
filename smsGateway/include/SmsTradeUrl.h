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

#ifndef SMSTRADEURL_H
#define SMSTRADEURL_H

#include "ISmsGateway.h"
#include <boost/asio.hpp>

using namespace std;

class SmsTradeUrl : public ISmsGateway
{
private:
    static const string name;
    boost::asio::io_service io_service;
    string gateway;
    string key;        // Persönlicher Identifikationscode (String, bis zu 35 Zeichen)
    string route;      // Auswahl der SMS-Route (basic|economy|gold|direct)
    string from;       // Absenderkennung der SMS (String, bis zu 11 Zeichen; Integer, bis zu 16 Zeichen)
    
    void InitializeFromConfig();
    string CreateParameterString( const string& msg, const string& receivers, bool debug );
    
public:
    SmsTradeUrl();
    SmsTradeUrl(string gatewayKey);
    SmsTradeUrl(const SmsTradeUrl& other);
    ~SmsTradeUrl();
    
    static const string GetGatewayName();
    string GetName() const;
    
    SmsTradeUrl& operator= ( const SmsTradeUrl& other );
    
    void SendMessage( const string& receivers, const string& msg, bool debug = false );
};

#endif // SMSTRADEURL_H
