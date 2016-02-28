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

#include "../../faxParser/include/IlsAnsbach.h"
#include "../../faxParser/include/ILocation.h"


using namespace std;

string PrepareMessage(IOperation* operation);

int main(int argc, char **argv) 
{
    if(argc != 2)
    {
        cerr << "Usage: " << argv[0] << " fax.txt" << endl;
        
        return -1;
    }
    
    string faxFilename = argv[1];
    
    cout << "Analyze fax: " << faxFilename << endl;
    
    IlsAnsbach faxParser;
    IOperation* operation = faxParser.Parse(faxFilename);
    
    string msg = PrepareMessage(operation);
    cout << msg << endl;
    
    return 0;
}

string PrepareMessage(IOperation* operation)
{
    if(operation == nullptr)
    {
        return "";
    }
    
    cout << "====== Parsed Result: ========================" << endl;
    cout << operation->ToString() << endl;
    cout << "==============================================" << endl;
    
    // Use a facet to display time in a custom format (only hour and minutes).
//     boost::posix_time::time_facet* facet = new boost::posix_time::time_facet();
//     facet->format("%H:%M");
    
    // Use a facet to display time in a custom format (only hour and minutes).
    boost::posix_time::time_facet facet;
    facet.format("%H:%M");
    
    stringstream ss;
    ss.imbue(locale(std::locale::classic(), &facet));
    ss << operation->GetTimestamp();
    
    string alarmzeit = ss.str();
    string einsatzort = operation->GetEinsatzort().ToString();
    string keywords = operation->GetKeywords().GetKeyword();
    string comment = operation->GetComment();
    string priority = operation->GetPriority();
    
    stringstream result;
    result << "Alarmzeit:  " << alarmzeit << endl;
    result << "Einsatzort: " << einsatzort << endl;
    result << "Keywords:   " << keywords << endl;
    result << "Comment:    " << comment << endl;
    result << "Priorität:  " << priority << endl;
    
    string msg = result.str();
}