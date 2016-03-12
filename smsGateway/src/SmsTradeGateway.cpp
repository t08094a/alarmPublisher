/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2015  Jochen Fähnlein <jochen.faehnlein@ff-ipsheim.de>
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

#include "SmsTradeGateway.h"
#include "SmstradeBinding.nsmap" // get the gSOAP-generated namespace bindings
#include "ConfigReader.h"
#include <iostream>
#include <fstream>
#include <stdsoap2.h>

const string SmsTradeGateway::name = "SmsTrade";

SmsTradeGateway::SmsTradeGateway() : key(), route("direct"), from("FF Alarm")
{
    InitializeFromConfig();
}

SmsTradeGateway::~SmsTradeGateway()
{
    // nop
}

const string SmsTradeGateway::GetGatewayName()
{
    return name;
}

string SmsTradeGateway::GetName() const
{
    return name;
}

void SmsTradeGateway::SendMessage(const string& recipients, const string& msg, bool debug) // to = Empfänger der Nachricht (Integer, bis zu 16 Zeichen)
{
    map<string, string> options;
    
    if(debug)
    {
        options["debug"] = "1";           // Aktiviert den Debugmodus. SMS werden nicht versendet und nicht berechnet
    }
    options["message_id"] = "1";      // Aktiviert die Rückgabe der Message ID
    options["count"] = "1";           // Aktiviert die Rückgabe der SMS Anzahl
    options["dlr"] = "1";             // Aktiviert den Empfang eines Versandberichtes für diese SMS
    options["messagetype"] = "flash"; // Typ der Nachricht: flash, unicode, binary, voice
    
    SendMessage(recipients, msg, options);
}

void SmsTradeGateway::SendMessage(const string& recipients, const string& msg, const map<string, string>& options)
{
    SmstradeBindingProxy server(SOAP_C_UTFSTRING);
    
    InitializeProxy(server);
    
    for(map<string, string>::const_iterator it = options.begin(); it != options.end(); ++it)
    {
        SetParameter(server, it->first, it->second);
    }
    
    SendMessage(server, recipients, msg);
        
    server.destroy();
}

void SmsTradeGateway::SendMessage(SmstradeBindingProxy& server, const string& to, const string& msg)
{
    struct ns1__sendSMSResponse returnData;
        
    // returns error code or SOAP_OK
    int result = server.sendSMS(key, to, "äöüß" + msg, route, from, returnData); // $returnval Array mit Daten: 0 => Returncode, 1 => MessageID, 2 => entstandene Kosten, 3 => Anzahl der SMS, 4 => Zeitpunkt des Versandes
    if(result != SOAP_OK)
    {
        server.soap_stream_fault(std::cerr);
    }
    else
    {
        // TODO: returnData auswerten
    }
}

void SmsTradeGateway::InitializeProxy(SmstradeBindingProxy& server)
{
    server.soap->connect_timeout = 10; // connect within 10 s
    server.soap->send_timeout = 5;     // send timeout is 5s
    server.soap->recv_timeout = 5;     // receive timeout is 5s
    
    soap_ssl_init();
    
    soap* serverSoap = &(*server.soap);
    
    if(serverSoap == nullptr)
    {
        soap_stream_fault(serverSoap, std::cerr);
    }
    
    int result = soap_ssl_client_context(serverSoap, SOAP_SSL_NO_AUTHENTICATION, NULL, NULL, NULL, NULL, NULL); // SOAP_SSL_DEFAULT besser: SOAP_SSL_CLIENT
    
    if(result)
    {
        soap_stream_fault(serverSoap, std::cerr);
    }
}

void SmsTradeGateway::SetParameter(SmstradeBindingProxy& server, const string& parameterName, const string& value)
{
    string setOptionalParamResponse;
        
    int result = server.setOptionalParam(parameterName, value, setOptionalParamResponse);
    if(result != SOAP_OK)
    {
        server.soap_stream_fault(std::cerr);
    }
}

void SmsTradeGateway::InitializeFromConfig()
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
