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

#ifndef URLENCODING_H
#define URLENCODING_H

#include <sstream>
#include <iostream>

using namespace std;

class UrlEncoding
{
private:
    static string ConvertToHex( unsigned int c )
    {
        std::ostringstream stm ;
        stm << '%' << std::hex << std::uppercase << c;
        return stm.str();
    }
    
public:
    static string Encode( const string& str )
    {
        static const std::string unreserved = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                              "abcdefghijklmnopqrstuvwxyz"
                                              "-_.~";
        
        string result;
    
        for( unsigned char c : str )
        {
            if(c == '\r')
            {
                result += "%0D";
            }
            else if(c == '\n')
            {
                result += "%0A";
            }
            else if(c == '\t')
            {
                result += "%20%20%20%20";
            }
            else if( unreserved.find(c) != string::npos ) 
            {
                result += c;
            }
            else 
            {
                result += ConvertToHex(c);
            }
        }
        
        return result;
    }
};

#endif // URLENCODING_H
