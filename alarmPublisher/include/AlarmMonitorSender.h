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

#ifndef ALARMMONITORSENDER_H
#define ALARMMONITORSENDER_H

#include "Operation.h"
#include "../../faxParser/include/IOperation.h"
#include <boost/log/trivial.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;

class AlarmMonitorSender
{
private:
    string endpoint;
    string api;
    
    bool initialized;
    
    void Initialize();
    
    string FormatDateTimeForJson(boost::posix_time::ptime timestamp) const;
    void GetResourceAsJson(const vector<unique_ptr<OperationResource>>& resources, stringstream& ss);
    
public:
    AlarmMonitorSender();
    ~AlarmMonitorSender();
    
    string GetDataAsJson(IOperation* operation);
    void Send(IOperation* operation);
};

#endif // ALARMMONITORSENDER_H
