#define BOOST_TEST_DYN_LINK

#include <memory>
#include <string>
#include <boost/test/unit_test.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include "../include/OperationResource.h"

using namespace std;

BOOST_AUTO_TEST_CASE(SetFullnameReturnsTheSameValue)
{
    string expected = "THL";
    
    OperationResource resource;
    resource.SetFullName(expected);
    
    BOOST_CHECK(expected == resource.GetFullName());
}

BOOST_AUTO_TEST_CASE(SetTimestampReturnsTheSameValue)
{
    boost::posix_time::ptime expected(boost::gregorian::date(2002, boost::gregorian::Jan, 10), boost::posix_time::time_duration(1,2,3));
    
    OperationResource resource;
    resource.SetTimestamp(expected);
    
    BOOST_CHECK(expected == resource.GetTimestamp());
}

BOOST_AUTO_TEST_CASE(AddEquipmentReturnsTheSameValue)
{
    string equip1 = "THL";
    string equip2 = "Überdruckbelüftung";
    
    OperationResource resource;
    resource.AddRequestedEquipment(equip1);
    resource.AddRequestedEquipment(equip2);
    
    BOOST_CHECK(equip1 == resource.GetRequestedEquipment().at(0));
    BOOST_CHECK(equip2 == resource.GetRequestedEquipment().at(1));
}

BOOST_AUTO_TEST_CASE(ToStringReturnsExpectedResult)
{
    string name = "Ausrüstung";
    boost::posix_time::ptime tt(boost::gregorian::date(2002, boost::gregorian::Jan, 10), boost::posix_time::time_duration(1,2,3));
    string equip1 = "THL";
    string equip2 = "Überdruckbelüftung";
    
    OperationResource resource;
    resource.SetFullName(name);
    resource.SetTimestamp(tt);
    resource.AddRequestedEquipment(equip1);
    resource.AddRequestedEquipment(equip2);
    
    string expected = "Ausrüstung\t(Time: 2002-Jan-10 01:02:03\tEquipments:THL, Überdruckbelüftung)";
    string result = resource.ToString();
    
    BOOST_CHECK(expected == result);
}

BOOST_AUTO_TEST_CASE(CopyConstructorCreatesEqualObject)
{
    string name = "Ausrüstung";
    boost::posix_time::ptime tt(boost::gregorian::date(2002, boost::gregorian::Jan, 10), boost::posix_time::time_duration(1,2,3));
    string equip1 = "THL";
    string equip2 = "Überdruckbelüftung";
    
    OperationResource resource;
    resource.SetFullName(name);
    resource.SetTimestamp(tt);
    resource.AddRequestedEquipment(equip1);
    resource.AddRequestedEquipment(equip2);
    
    OperationResource copy = resource;
    
    BOOST_CHECK(addressof<OperationResource>(copy) != addressof<OperationResource>(resource));
    
    BOOST_CHECK(copy.GetFullName() == resource.GetFullName());
    BOOST_CHECK(copy.GetTimestamp() == resource.GetTimestamp());
    BOOST_CHECK(copy.GetRequestedEquipment().size() == resource.GetRequestedEquipment().size());
    BOOST_CHECK(copy.GetRequestedEquipment().at(0) == resource.GetRequestedEquipment().at(0));
    BOOST_CHECK(copy.GetRequestedEquipment().at(1) == resource.GetRequestedEquipment().at(1));
    
    BOOST_CHECK(addressof(copy.GetRequestedEquipment()) != addressof(resource.GetRequestedEquipment()));
    BOOST_CHECK(addressof(copy.GetRequestedEquipment().at(0)) != addressof(resource.GetRequestedEquipment().at(0)));
}

BOOST_AUTO_TEST_CASE(AssignmentOperatorCreatesEqualObject)
{
    string name = "Ausrüstung";
    boost::posix_time::ptime tt(boost::gregorian::date(2002, boost::gregorian::Jan, 10), boost::posix_time::time_duration(1,2,3));
    string equip1 = "THL";
    string equip2 = "Überdruckbelüftung";
    
    OperationResource resource;
    resource.SetFullName(name);
    resource.SetTimestamp(tt);
    resource.AddRequestedEquipment(equip1);
    resource.AddRequestedEquipment(equip2);
    
    OperationResource copy;
    copy = resource;
    
    BOOST_CHECK(addressof<OperationResource>(copy) != addressof<OperationResource>(resource));
    
    BOOST_CHECK(copy.GetFullName() == resource.GetFullName());
    BOOST_CHECK(copy.GetTimestamp() == resource.GetTimestamp());
    BOOST_CHECK(copy.GetRequestedEquipment().size() == resource.GetRequestedEquipment().size());
    BOOST_CHECK(copy.GetRequestedEquipment().at(0) == resource.GetRequestedEquipment().at(0));
    BOOST_CHECK(copy.GetRequestedEquipment().at(1) == resource.GetRequestedEquipment().at(1));
    
    BOOST_CHECK(addressof(copy.GetRequestedEquipment()) != addressof(resource.GetRequestedEquipment()));
    BOOST_CHECK(addressof(copy.GetRequestedEquipment().at(0)) != addressof(resource.GetRequestedEquipment().at(0)));
}

BOOST_AUTO_TEST_CASE( VerifyAssignmentOperatorWithSamePointerShouldHandleSelfAssignment )
{
    OperationResource resource;
    resource.SetFullName("Name");
    
    resource = resource;
    
    BOOST_CHECK(addressof(resource) == addressof(resource));
}

BOOST_AUTO_TEST_CASE(EqualOperatorReturnsTrue)
{
    string name = "Ausrüstung";
    boost::posix_time::ptime tt(boost::gregorian::date(2002, boost::gregorian::Jan, 10), boost::posix_time::time_duration(1,2,3));
    string equip1 = "THL";
    string equip2 = "Überdruckbelüftung";
    
    OperationResource resource1;
    resource1.SetFullName(name);
    resource1.SetTimestamp(tt);
    resource1.AddRequestedEquipment(equip1);
    resource1.AddRequestedEquipment(equip2);
    
    OperationResource resource2;
    resource2.SetFullName(name);
    resource2.SetTimestamp(tt);
    resource2.AddRequestedEquipment(equip1);
    resource2.AddRequestedEquipment(equip2);
    
    BOOST_CHECK(resource1 == resource2);
}

BOOST_AUTO_TEST_CASE(EqualOperatorReturnsFalse)
{
    string name = "Ausrüstung";
    boost::posix_time::ptime tt(boost::gregorian::date(2002, boost::gregorian::Jan, 10), boost::posix_time::time_duration(1,2,3));
    string equip1 = "THL";
    string equip2 = "Überdruckbelüftung";
    
    OperationResource resource1;
    resource1.SetFullName(name);
    resource1.SetTimestamp(tt);
    resource1.AddRequestedEquipment(equip1);
    resource1.AddRequestedEquipment(equip2);
    
    OperationResource resource2;
    resource2.SetFullName(name + "__");
    resource2.SetTimestamp(tt);
    resource2.AddRequestedEquipment(equip1);
    resource2.AddRequestedEquipment(equip2);
    
    BOOST_CHECK(resource1 != resource2);
}
