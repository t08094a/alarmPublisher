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

#ifndef CMGATEWAY_H
#define CMGATEWAY_H

#include "ISmsGateway.h"

#include <vector>

using namespace std;

class CmGateway : public ISmsGateway
{
private:
    static const string name;
    
    string productToken; // = Guid, Persönlicher Identifikationscode
    string from;         // Absenderkennung der SMS (String, bis zu 11 Zeichen; Integer, bis zu 16 Zeichen)

    void InitializeFromConfig();

    string CreateMessage(const string& msg, vector<string>& telephoneNumbers);

public:
    CmGateway();
    CmGateway ( const CmGateway& other );
    ~CmGateway();
    CmGateway& operator= ( const CmGateway& other );

    static const string GetGatewayName();
    string GetName() const;

    void SendMessage(const string& msg, bool debug = false );
};

#endif // CMGATEWAY_H
