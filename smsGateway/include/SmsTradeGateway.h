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

#ifndef SMSTRADEGATEWAY_H
#define SMSTRADEGATEWAY_H

#include "ISmsGateway.h"
#include "soapSmstradeBindingProxy.h"

#include <map>
#include <boost/assign.hpp>

using namespace std;

/*
enum ParameterValues : unsigned char
{
    Deactivate = 0,
    Activate = 1
};

enum ResponseCode : unsigned char
{
    ReceiverNotCorrect = 10,
    SenderNotCorrect = 20,
    MessageNotCorrect = 30,
    MessageTypeNotCorrect = 31,
    RouteNotCorrect = 40,
    AuthorizationError = 50,
    PaymentError = 60,
    NetNotAvailable = 70,
    FeatureNotAvailable = 71,
    SmsC_Error = 80,
    Ok = 100
};

struct Route
{
    string Basic;
    string Gold;
    string Direct;
} static Route = { "basic", "gold", "direct" };

struct Charset
{
    string Utf8;
    string ISO_8859_1;
    string ISO_8859_15;
} static Charset = { "UTF-8", "ISO-8859-1", "ISO-8859-15" };

struct MessageType
{
    string Flash;
    string Unicode;
    string Binary;
    string Voice;
} static MessageType = { "flash", "unicode", "binary", "voice" };
*/

class SmsTradeGateway : public ISmsGateway
{
private:
    static const string name;

    map<string, string> responseCode = {
        {"10", "Empfängernummer ist nicht korrekt"}, 
        {"20", "Absenderkennung ist nicht korrekt"},
        {"30", "Nachrichtentext ist nicht korrekt"},
        {"31", "Messagetyp ist nicht korrekt"},
        {"40", "SMS Route ist nicht korrekt"},
        {"50", "Identifikation ist fehlgeschlagen"},
        {"60", "Nicht genügend Guthaben"},
        {"70", "Netz wird nicht abgedeckt"},
        {"71", "Feature nicht möglich"},
        {"80", "Übergabe an SMS-C fehlgeschlagen"},
        {"100", "SMS wurde angenommen und versendet"}
    };
    
    string key;        // Persönlicher Identifikationscode (String, bis zu 35 Zeichen)
    string route;      // Auswahl der SMS-Route (basic|economy|gold|direct)
    string from;       // Absenderkennung der SMS (String, bis zu 11 Zeichen; Integer, bis zu 16 Zeichen)
    
    void InitializeProxy(SmstradeBindingProxy& server);
    void SetParameter(SmstradeBindingProxy& server, const string& parameterName, const string& value);
    void SendMessage(const string& distributionList, const string& msg, const map< string, string >& options );
    void SendMessage(SmstradeBindingProxy& server, const string& to, const string& msg);
    void InitializeFromConfig();
    string ParseResponseCode(string code);
    
public:
    SmsTradeGateway();
    ~SmsTradeGateway();
    
    static const string GetGatewayName();
    string GetName() const;
    
    void SendMessage( const string& distributionList, const string& msg, bool debug = false );
};

#endif // SMSTRADEGATEWAY_H
