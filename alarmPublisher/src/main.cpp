/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2015  Jochen Fähnlein <jochen.faehnlein@ff-ipsheim.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <iostream>
#include <string>
#include <locale>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/shared_ptr.hpp>

#include "../../faxParser/include/IlsAnsbach.h"
#include "../../faxParser/include/ILocation.h"
#include "../../smsGateway/include/GatewayManager.h"

using namespace std;

// prototype definitions
void InitLogging();
string PrepareMessage(IOperation* operation);

int main(int argc, char **argv) 
{
    InitLogging();
    
    if(argc != 2)
    {
        BOOST_LOG_TRIVIAL(error) << "Usage: " << argv[0] << " fax.txt";
                
        return -1;
    }
    
    string faxFilename = argv[1];
    
    BOOST_LOG_TRIVIAL(info) << "Analyze fax: " << faxFilename << endl;
    
    try
    {
        IlsAnsbach faxParser;
        IOperation* operation = faxParser.Parse(faxFilename);
    
        string msg = PrepareMessage(operation);
            
        delete operation;
        
        BOOST_LOG_TRIVIAL(info) << "Send message ...";
        
        GatewayManager smsManager;
        smsManager.SendMessage("Recipients", msg);
    }
    catch(exception &e)
    {
        BOOST_LOG_TRIVIAL(error) << e.what() << endl;
    }
    
    return 0;
}

string PrepareMessage(IOperation* operation)
{
    BOOST_LOG_TRIVIAL(info) << "Prepare message to send"; 
    
    if(operation == nullptr)
    {
        return "";
    }
    
    stringstream ss;
    
    // Use a facet to display time in a custom format (only hour and minutes).
    boost::posix_time::time_facet* facet = new boost::posix_time::time_facet();
    facet->format("%H:%M");
    
    ss.imbue(std::locale(std::locale::classic(), facet));
    ss << operation->GetTimestamp();
    
    string alarmzeit = ss.str();
    string einsatzort = operation->GetEinsatzort().ToString();
    string keywords = operation->GetKeywords().GetKeyword();
    string comment = operation->GetComment();
    string priority = operation->GetPriority();
    
    stringstream result;
    result << "Alarmzeit:  " << alarmzeit << endl;
    result << "Einsatzort: " << einsatzort << endl;
    result << "Schlagw.:   " << keywords << endl;
    result << "Priorität:  " << priority << endl;
    result << "Bemerkung:  " << comment << endl;
    
    string msg = result.str();
    
    BOOST_LOG_TRIVIAL(debug) << "====== Generated message: ====================";
    BOOST_LOG_TRIVIAL(debug) << result;
    BOOST_LOG_TRIVIAL(debug) << "==============================================";
    
    return msg;
}

void InitLogging()
{
    namespace logging = boost::log;
    namespace src = boost::log::sources;
    namespace expr = boost::log::expressions;
    namespace sinks = boost::log::sinks;
    namespace keywords = boost::log::keywords;

    boost::shared_ptr<logging::core> core = logging::core::get();
    
    boost::shared_ptr<sinks::text_file_backend> backend = boost::make_shared<sinks::text_file_backend>(
        keywords::file_name = "alarmPublisher_%Y%m%d_%H%M%S.log"
    );
    
    // Wrap it into the frontend and register in the core.
    // The backend requires synchronization in the frontend.
    typedef sinks::synchronous_sink<sinks::text_file_backend> file_sink;
    
    boost::shared_ptr<file_sink> sink(new file_sink(backend));
    
    sink->set_formatter
    (
        expr::format("%1%  Z. %2%: [%3%] %4%")
            % expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S:%f")
            % expr::attr< unsigned int >("LineID")
            % logging::trivial::severity
            % expr::smessage
    );
    
    core->add_sink(sink);
    
    logging::add_console_log();

    logging::add_common_attributes();
}