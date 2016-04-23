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

#include "ConfigReader.h"
#include "GatewayFactory.h"
#include "GatewayManager.h"

#include <iostream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/log/trivial.hpp>

using namespace std;

GatewayManager::GatewayManager()
{
    // nop
}

GatewayManager::~GatewayManager()
{
    // nop
}

const set<string> GatewayManager::GetPossibleGatewayNames() const
{
    return GatewayFactory::GetInstance().GetAvailableGatewayNames();
}
    
void GatewayManager::SendMessage(const string& distributionList, const string& msg)
{
    vector<string> gateways;
    string gatewayConfig = ConfigReader::GetInstance().Get("GatewaySelector.Active");
    
    if(gatewayConfig.find(";"))
    {
        boost::algorithm::split(gateways, gatewayConfig, boost::is_any_of(",;"), boost::algorithm::token_compress_on);                
    }
    else
    {
        gateways.push_back(gatewayConfig);
    }
    
    for(string const& gw : gateways)
    {
        SendMessage(gw, distributionList, msg);
    }
}

void GatewayManager::SendMessage(const string& gateway, const string&  distributionList, const string&  msg)
{
    BOOST_LOG_TRIVIAL(info) << "Send message to \"" << distributionList << "\" via: " << gateway;
    
    ISmsGateway* foundGateway = GatewayFactory::GetInstance().GetGateway(gateway).get();
    
    if(foundGateway != nullptr)
    {
        foundGateway->SendMessage(distributionList, msg);
    }
    else
    {
        BOOST_LOG_TRIVIAL(error) << "The gateway \"" << gateway << "\" was not found!";
    }
}