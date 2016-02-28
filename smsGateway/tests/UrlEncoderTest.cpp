#define BOOST_TEST_DYN_LINK


#include <boost/test/unit_test.hpp>
#include "UrlEncoding.h"

using namespace boost::unit_test;
using namespace std;


BOOST_AUTO_TEST_CASE( TestSonderzeichenUndUmlaute )
{
    // http://www.url-encode-decode.com/
    string input = "äöüß #!\"$%&()+*-";
    string expected = "%C3%A4%C3%B6%C3%BC%C3%9F+%23%21%5C%22%24%25%26%28%29%2B%2A-";
    
    string result = UrlEncoding::Encode(input);
        
    BOOST_CHECK(expected == result);
}