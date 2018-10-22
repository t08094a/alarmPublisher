#define BOOST_TEST_DYN_LINK


#include <boost/test/unit_test.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include "restclient-cpp/restclient.h"
#include "../include/AlarmMonitorSender.h"
#include "../../faxParser/include/Operation.h"

using namespace boost::unit_test;
using namespace std;


BOOST_AUTO_TEST_CASE( SendIsCalledCorrectly )
{
    Operation* op = new Operation();
    op->SetComment("comment1");
    op->SetPriority("1");
    op->SetTimestamp(boost::posix_time::ptime(boost::gregorian::date(2002, boost::gregorian::Jan, 10), boost::posix_time::time_duration(1,2,3)));
    
    PropertyLocation& einsatzort = op->GetEinsatzortInternal();
    einsatzort.SetStreet("Meiserstraße");
    einsatzort.SetStreetNumber("2");
    
    AlarmMonitorSender* sender = new AlarmMonitorSender();
    sender->Send(op);
}

