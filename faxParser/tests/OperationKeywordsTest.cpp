#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include "../include/OperationKeywords.h"

BOOST_AUTO_TEST_CASE(NotEmptyKeywordsResultsInCorrectOutput)
{
    OperationKeywords* op = new OperationKeywords();
    op->SetKeyword("eins");

    std::string expected = "Stichwort: eins";
    BOOST_CHECK(expected == op->ToString());

    delete op;
}

BOOST_AUTO_TEST_CASE(NotEmptyEmergencyKeywordsResultsInCorrectOutput)
{
    OperationKeywords* op = new OperationKeywords();
    op->SetEmergencyKeyword("eins");

    std::string expected = "Stichwort: eins";
    BOOST_CHECK(expected == op->ToString());

    delete op;
}

BOOST_AUTO_TEST_CASE(SettedAllResultsInCorrectOutput)
{
    OperationKeywords* op = new OperationKeywords();
    op->SetKeyword("eins");
    op->SetEmergencyKeyword("zwei");
    op->SetB("b1");
    op->SetR("r1");
    op->SetS("s1");
    op->SetT("t1");

    std::string expected = "Stichwort: eins, Stichwort: zwei, B: b1, R: r1, S: s1, T: t1";
    BOOST_CHECK(expected == op->ToString());

    delete op;
}

BOOST_AUTO_TEST_CASE(SettedAllWithoutRKeywordsResultsInCorrectOutput)
{
    OperationKeywords* op = new OperationKeywords();
    op->SetKeyword("eins");
    op->SetEmergencyKeyword("zwei");
    op->SetB("b1");
    op->SetS("s1");
    op->SetT("t1");

    std::string expected = "Stichwort: eins, Stichwort: zwei, B: b1, S: s1, T: t1";
    BOOST_CHECK(expected == op->ToString());

    delete op;
}

BOOST_AUTO_TEST_CASE(CopyConstructerCreatesIdenticalObject)
{
    OperationKeywords* op = new OperationKeywords();
    op->SetKeyword("eins");
    op->SetEmergencyKeyword("zwei");
    op->SetB("b1");
    op->SetS("s1");
    op->SetT("t1");

    OperationKeywords* copiedObject = new OperationKeywords(*op);

    BOOST_CHECK(op->GetB() == copiedObject->GetB());
    BOOST_CHECK(op->GetEmergencyKeyword() == copiedObject->GetEmergencyKeyword());
    BOOST_CHECK(op->GetKeyword() == copiedObject->GetKeyword());
    BOOST_CHECK(op->GetR() == copiedObject->GetR());
    BOOST_CHECK(op->GetS() == copiedObject->GetS());
    BOOST_CHECK(op->GetT() == copiedObject->GetT());

    delete op;
    delete copiedObject;
}

BOOST_AUTO_TEST_CASE(CompareEqualToTrue)
{
    OperationKeywords* op1 = new OperationKeywords();
    op1->SetKeyword("eins");
    op1->SetEmergencyKeyword("zwei");
    op1->SetB("b1");
    op1->SetS("s1");
    op1->SetT("t1");
    
    OperationKeywords* op2 = new OperationKeywords();
    op2->SetKeyword("eins");
    op2->SetEmergencyKeyword("zwei");
    op2->SetB("b1");
    op2->SetS("s1");
    op2->SetT("t1");
    
    BOOST_CHECK(*op1 == *op2);
    
    delete op1;
    delete op2;
}

BOOST_AUTO_TEST_CASE(CompareEqualToFalse)
{
    OperationKeywords* op1 = new OperationKeywords();
    op1->SetKeyword("eins");
    op1->SetEmergencyKeyword("zwei");
    op1->SetB("b1");
    op1->SetS("s1");
    op1->SetT("t1");
    
    OperationKeywords* op2 = new OperationKeywords();
    op2->SetKeyword("eins");
    op2->SetEmergencyKeyword("zwei");
    op2->SetB("b1");
    op2->SetS("s2");
    op2->SetT("t1");
    
    BOOST_CHECK(*op1 != *op2);
    
    delete op1;
    delete op2;
}

BOOST_AUTO_TEST_CASE(AssignmentCreatesIdenticalObject)
{
    OperationKeywords* op1 = new OperationKeywords();
    op1->SetKeyword("eins");
    op1->SetEmergencyKeyword("zwei");
    op1->SetB("b1");
    op1->SetS("s1");
    op1->SetT("t1");
    
    OperationKeywords* op2 = op1;
    
    BOOST_CHECK(*op1 == *op2); // compare content
    BOOST_CHECK(op1 == op2);   // compare pointer
    
    delete op1;
    // delete op2; // already free by delete op1
}

BOOST_AUTO_TEST_CASE( VerifyAssignmentOperatorWithSamePointerShouldHandleSelfAssignment )
{
    OperationKeywords op;
    op.SetKeyword("THL");
    
    op = op;
    
    BOOST_CHECK(addressof<OperationKeywords>(op) == addressof<OperationKeywords>(op));
}
