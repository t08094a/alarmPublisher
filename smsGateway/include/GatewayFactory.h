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

#ifndef GATEWAYFACTORY_H
#define GATEWAYFACTORY_H

#include <map>
#include <memory>
#include <string>
#include <set>
#include <boost/utility.hpp>
#include "ISmsGateway.h"

using namespace std;

// uses singleton implementation
class GatewayFactory : boost::noncopyable
{
private:
    set<string> availableGatewayNames;
    map<string, shared_ptr<ISmsGateway>> availableGateways;
    
    GatewayFactory();
    GatewayFactory(const GatewayFactory&);
    
    GatewayFactory& operator=(const GatewayFactory&);
    
    shared_ptr<ISmsGateway> CreateGateway(const string& name);

public:
    ~GatewayFactory();
    
    static GatewayFactory& GetInstance();
    const set<string> GetAvailableGatewayNames() const;
    shared_ptr<ISmsGateway> GetGateway(const string& name);
};

#endif // GATEWAYFACTORY_H
