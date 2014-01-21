/*
    This file is part of Pretty Curved Privacy (pcp1).

    Copyright (C) 2013 T. von Dein.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    You can contact me by mail: <tlinden AT cpan DOT org>.
*/


// various helpers

#ifndef _HAVE_PCP_UTIL_H
#define _HAVE_PCP_UTIL_H

#include <ctype.h>

static inline char *_lc(char *in) {
  size_t len = strlen(in);
  int i;
  for(i=0; i<len; ++i)
    in[i] = towlower(in[i]);
  return in;
}

#endif // _HAVE_PCP_UTIL_H