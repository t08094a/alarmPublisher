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

#ifndef GATEWAYMANAGER_H
#define GATEWAYMANAGER_H

#include <string>
#include <set>
#include "ISmsGateway.h"

using namespace std;

class GatewayManager
{
public:
    GatewayManager();
    ~GatewayManager();
    
    const set<string> GetPossibleGatewayNames() const;
    
    void SendMessage( const string& distributionList, const string& msg);
    void SendMessage( const string& gateway, const string& distributionList, const string& msg );
};

#endif // GATEWAYMANAGER_H
