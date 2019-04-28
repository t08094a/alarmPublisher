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

#include "AlarmMonitorSender.h"
#include "ConfigReader.h"
#include "ILocation.h"
#include "IKeywords.h"
#include "restclient-cpp/restclient.h"
#include "restclient-cpp/connection.h"

AlarmMonitorSender::AlarmMonitorSender() : endpoint(), api(), initialized(false)
{
    BOOST_LOG_TRIVIAL(debug) << "Create AlarmMonitorSender";
    
    Initialize();
}

AlarmMonitorSender::~AlarmMonitorSender()
{
    BOOST_LOG_TRIVIAL(info) << "disable REST client ...";
    RestClient::disable();
}

void AlarmMonitorSender::Initialize()
{
    BOOST_LOG_TRIVIAL(info) << "Initialize AlarmMonitorSender from config";
    
    // read config settings for REST endpoint
    endpoint = ConfigReader::GetInstance().Get("AlarmMonitor.endpoint");
    api = ConfigReader::GetInstance().Get("AlarmMonitor.api");
    
    BOOST_LOG_TRIVIAL(info) << "use endpoint: " << endpoint;
    BOOST_LOG_TRIVIAL(info) << "use api: " << api;
    
    BOOST_LOG_TRIVIAL(info) << "initialize REST client ...";
    RestClient::init();
    
    initialized = true;
}

void AlarmMonitorSender::Send(IOperation* operation)
{
    if(!initialized)
    {
        BOOST_LOG_TRIVIAL(error) << "AlarmMonitorSender is not initialized -> unable to send a message";
        return;
    }
    
    BOOST_LOG_TRIVIAL(info) << "Send alarm info to alarmmonitors REST api";
    
    
    string url = endpoint + api;
    const string contentType = "application/json";
    string data = GetDataAsJson(operation);
    
    RestClient::Response response = RestClient::post(url, contentType, data);

    if(response.code == 201)
    {
        BOOST_LOG_TRIVIAL(info) << "Successfully send REST request";
    }
    else
    {
        BOOST_LOG_TRIVIAL(error) << "REST response:" << endl << response.body;
    }
}

string AlarmMonitorSender::GetDataAsJson(IOperation* operation)
{
    ILocation& location = operation->GetEinsatzort();
    IKeywords& keywords =  operation->GetKeywords();

    stringstream ss;
    ss << "{" << endl;
    ss << "  \"time\": \"" << FormatDateTimeForJson(operation->GetTimestamp()) << "\"," << endl;
    ss << "  \"comment\": \"" << operation->GetComment() << "\"," << endl;
    ss << "  \"keywords\": {" << endl;
    ss << "    \"keyword\": \"" << keywords.GetKeyword() << "\"," << endl;
    ss << "    \"emergencyKeyword\": \"" << keywords.GetEmergencyKeyword() << "\"," << endl;
    ss << "    \"b\": \"" << keywords.GetB() << "\"," << endl;
    ss << "    \"r\": \"" << keywords.GetR() << "\"," << endl;
    ss << "    \"s\": \"" << keywords.GetS() << "\"," << endl;
    ss << "    \"t\": \"" << keywords.GetT() << "\"" << endl;
    ss << "  }," << endl;
    ss << "  \"placeOfAction\": {" << endl;
    ss << "    \"street\": \"" << location.GetStreet() << "\"," << endl;
    ss << "    \"houseNumber\": \"" << location.GetStreetNumber() << "\"," << endl;
    ss << "    \"addition\": \"" << operation->GetEinsatzortZusatz() << "\"," << endl;
    ss << "    \"city\": \"" << location.GetCity() << "\"";

    if(location.HasGeoCoordinates())
    {
        ss << "," << endl;
        ss << "    \"geoPosition\": {" << endl;
        ss << "      \"x\": \"" << boost::lexical_cast<std::string>(location.GetGeoLatitude()) << "\"," << endl;
        ss << "      \"y\": \"" << boost::lexical_cast<std::string>(location.GetGeoLongitude()) << "\"" << endl;
        ss << "    }" << endl;
    }

    ss << "  }," << endl;
    ss << "  \"priority\": " << operation->GetPriority() << "," << endl;

    GetResourceAsJson(operation->GetResources(), ss);

    ss << "}";

    string s = ss.str();

    BOOST_LOG_TRIVIAL(info) << "JSON content:" << endl << s;

    return s;

    /*
{
  "time": "2002-01-10T01:02:03Z",
  "comment": "comment1",
  "keywords": {
    "keyword": "Keyword1",
    "emergencyKeyword": "THL1",
    "b": "b",
    "r": "r",
    "s": "s",
    "t": "t"
  },
  "placeOfAction": {
    "street": "Meiserstraße",
    "houseNumber": "2",
    "addition": "Garage",
    "city": "Nürnberg",
    "geoPosition": {
      "x": "1.2",
      "y": "2.3"
    }
  },
  "priority": 1,
  "resources": [
    {
      "name": "LF8",
      "equipments": [
        {
          "name": "Wasser 600l"
        },
        {
          "name": "THL"
        }
      ]
    },
    {
      "name": "MTW",
      "equipments": [
        {
          "name": "Überdruckbelüftung"
        }
      ]
    }
  ]
}
*/
}

string AlarmMonitorSender::FormatDateTimeForJson(boost::posix_time::ptime timestamp) const
{
    using namespace boost::posix_time;

    static std::locale loc(std::cout.getloc(), new time_facet("%Y-%m-%dT%H:%M:%SZ"));

    ostringstream is;
    is.imbue(loc);
    is << timestamp;

    string result = is.str();
    return result;
}

void AlarmMonitorSender::GetResourceAsJson(const vector<unique_ptr<OperationResource>>& resources, stringstream& ss)
{
    ss << "  \"resources\": [" << endl;

    int resourcesCount = resources.size();
    int resourcesCurrentIndex = 0;
    for(auto const& resource : resources)
    {
        ss << "    {" << endl;
        ss << "      \"name\": \"" << resource.get()->GetFullName() << "\"," << endl;
        ss << "      \"equipments\": ";

        int equipmentCount = resource.get()->GetRequestedEquipment().size();
        if(equipmentCount == 0)
        {
            ss << "[]" << endl;
        }
        else
        {
            ss << "[" << endl;

            int equipmentCurrentIndex = 0;
            for(auto const& equipment : resource.get()->GetRequestedEquipment())
            {
                ss << "        {" << endl;
                ss << "          \"name\": \"" << equipment << "\"" << endl;
                ss << "        }";

                if(equipmentCurrentIndex < equipmentCount - 1)
                {
                    ss << ",";
                }

                ss << endl;

                equipmentCurrentIndex++;
            }

            ss << "      ]" << endl;
        }

        ss << "    }";

        if(resourcesCurrentIndex < resourcesCount - 1)
        {
            ss << ",";
        }

        ss << endl;
        resourcesCurrentIndex++;
    }

    ss << "  ]" << endl;
}
