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

#ifndef CURRENTSECTION_H
#define CURRENTSECTION_H

#include <iostream>

enum CurrentSection
{
    Undefined = 0,
    AHeader,
    BMitteiler,
    CEinsatzort,
    DZielort,
    EEinsatzgrund,
    FEinsatzmittel,
    GBemerkung,
    // Footer text. Introduced by "ENDE FAX". Can be ignored completely.
    HFooter
};

inline std::ostream& operator<<(std::ostream& os, const CurrentSection& section )
{
   switch( section )
   {
      case Undefined: os << "Undefined"; break;
      case AHeader: os << "Header"; break;
      case BMitteiler: os << "Mitteiler"; break;
      case CEinsatzort: os << "Einsatzort"; break;
      case DZielort: os << "Zielort"; break;
      case EEinsatzgrund: os << "Einsatzgrund"; break;
      case FEinsatzmittel: os << "Einsatzmittel"; break;
      case GBemerkung: os << "Bemerkung"; break;
      case HFooter: os << "Footer"; break;
   }
   
   return os;
}

#endif // CURRENTSECTION_H
