#define BOOST_TEST_DYN_LINK


#include <boost/test/unit_test.hpp>
#include "UrlEncoding.h"

using namespace boost::unit_test;
using namespace std;

#include <sstream>
#include <iostream>

// http://meyerweb.com/eric/tools/dencoder/

BOOST_AUTO_TEST_CASE( TestUmlautAe )
{
    string input = "ä";
    string expected = "%C3%A4";
    
    string result = UrlEncoding::Encode(input);
    
    cout << "encode: " << input << " --> " << result << endl;
    BOOST_CHECK(expected == result);
    
    
    input = "Ä";
    expected = "%C3%84";
    
    result = UrlEncoding::Encode(input);
    
    cout << "encode: " << input << " --> " << result << endl;
    BOOST_CHECK(expected == result);
}

BOOST_AUTO_TEST_CASE( TestUmlautOe )
{
    string input = "ö";
    string expected = "%C3%B6";
    
    string result = UrlEncoding::Encode(input);
    
    cout << "encode: " << input << " --> " << result << endl;
    BOOST_CHECK(expected == result);
    
    
    input = "Ö";
    expected = "%C3%96";
    
    result = UrlEncoding::Encode(input);
    
    cout << "encode: " << input << " --> " << result << endl;
    BOOST_CHECK(expected == result);
}

BOOST_AUTO_TEST_CASE( TestUmlautUe )
{
    string input = "ü";
    string expected = "%C3%BC";
    
    string result = UrlEncoding::Encode(input);
    
    cout << "encode: " << input << " --> " << result << endl;
    BOOST_CHECK(expected == result);
    
    
    input = "Ü";
    expected = "%C3%9C";
    
    result = UrlEncoding::Encode(input);
    
    cout << "encode: " << input << " --> " << result << endl;
    BOOST_CHECK(expected == result);
}

BOOST_AUTO_TEST_CASE( TestSz )
{
    string input = "ß";
    string expected = "%C3%9F";
    
    string result = UrlEncoding::Encode(input);
    
    cout << "encode: " << input << " --> " << result << endl;
    BOOST_CHECK(expected == result);
}

BOOST_AUTO_TEST_CASE( TestSonderzeichen )
{
    string input = "#!\"$%&+";
    string expected = "%23%21%22%24%25%26%2B";
    
    string result = UrlEncoding::Encode(input);
    
    cout << "encode: " << input << " --> " << result << endl;
    BOOST_CHECK(expected == result);
}

BOOST_AUTO_TEST_CASE( TestLeerzeichen )
{
    string input = " ";
    string expected = "%20";
    
    string result = UrlEncoding::Encode(input);
    
    cout << "encode: " << input << " --> " << result << endl;
    BOOST_CHECK(expected == result);
}

BOOST_AUTO_TEST_CASE( TestZeilenumbruch )
{
    string input = "\r\n";
    string expected = "%0D%0A";
    
    string result = UrlEncoding::Encode(input);
    
    cout << "encode: \\r\\n --> " << result << endl;
    BOOST_CHECK(expected == result);
}

BOOST_AUTO_TEST_CASE( TestSonderzeichenUndUmlaute )
{
    // http://meyerweb.com/eric/tools/dencoder/
    string input = "äöüß #\"$%&+";
    string expected = "%C3%A4%C3%B6%C3%BC%C3%9F%20%23%22%24%25%26%2B";
    
    string result = UrlEncoding::Encode(input);
    cout << "result: " << result << endl;
    BOOST_CHECK(expected == result);
}
