#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include "../include/GaussKruegerCoordinate.h"

BOOST_AUTO_TEST_CASE(Coordinates1ShouldTransformed)
{
    GaussKruegerCoordinate coordinate(4392249, 5488985);
    
    BOOST_CHECK(4392249 == coordinate.GetX());
    BOOST_CHECK(5488985 == coordinate.GetY());
    
//     double epsilon = 0.001;
//     
//     BOOST_CHECK_CLOSE(49.5281164861747, coordinate.GetX_WGS84(), epsilon);
//     BOOST_CHECK_CLOSE(10.510129026738161, coordinate.GetY_WGS84(), epsilon);
}

