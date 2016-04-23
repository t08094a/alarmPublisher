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

#include "GatewayFactory.h"
#include "ProwlGateway.h"
#include "SmsTradeGateway.h"
#include "SmsTradeUrlGateway.h"

#include <exception>

#include <boost/log/trivial.hpp>

GatewayFactory::GatewayFactory()
{
    BOOST_LOG_TRIVIAL(info) << "Create GatewayFactory";
    
    RegisterGateway(SmsTradeGateway::GetGatewayName());
    RegisterGateway(SmsTradeUrlGateway::GetGatewayName());
    RegisterGateway(ProwlGateway::GetGatewayName());
}

GatewayFactory::GatewayFactory(const GatewayFactory&)
{
    // nop
}

GatewayFactory::~GatewayFactory()
{
    // nop
}

GatewayFactory& GatewayFactory::GetInstance()
{
    static GatewayFactory _instance;
    return _instance;
}

shared_ptr<ISmsGateway> GatewayFactory::CreateGateway(const string& name)
{
    BOOST_LOG_TRIVIAL(info) << "Create gateway \"" << name << "\"";
    
    ISmsGateway* gw = nullptr;
    
    if(availableGatewayNames.find(name) != availableGatewayNames.end())
    {
        if(name == SmsTradeGateway::GetGatewayName())
        {
            gw = new SmsTradeGateway();
        }
        else if(name == SmsTradeUrlGateway::GetGatewayName())
        {
            gw = new SmsTradeUrlGateway();
        }
        else if (name == ProwlGateway::GetGatewayName())
        {
            gw = new ProwlGateway();
        }
    }
    else
    {
        throw invalid_argument("The gateway '" + name + "' does not exist!");
    }
    
    shared_ptr<ISmsGateway> gateway(gw);
    return gateway;
}

const set<string> GatewayFactory::GetAvailableGatewayNames() const
{
    return availableGatewayNames;
}

shared_ptr<ISmsGateway> GatewayFactory::GetGateway(const string& name)
{
    // try to find an existing instance; if not found, std::map will return availableGateways.end()
    map<string, shared_ptr<ISmsGateway>>::iterator it = availableGateways.find(name);
    
    shared_ptr<ISmsGateway> gateway;
    
    if(it == availableGateways.end())
    {
        // if no instance with the proper type was found, make one
        gateway = CreateGateway(name);
        
        // add the newly created gateway to the map for later lookup
        availableGateways[name] = gateway;
    }
    else
    {
        // if already had an instance
        gateway = it->second;
    }
    
    return gateway;
}

void GatewayFactory::RegisterGateway(const string& gatewayName)
{
    BOOST_LOG_TRIVIAL(info) << "Register gateway \"" << gatewayName << "\"";
    availableGatewayNames.insert(gatewayName);
}
