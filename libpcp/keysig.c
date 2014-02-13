/*
    This file is part of Pretty Curved Privacy (pcp1).

    Copyright (C) 2013-2014 T.v.Dein.

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

    You can contact me by mail: <tom AT vondein DOT org>.
*/


#include "keysig.h"

pcp_keysig_t * keysig2be(pcp_keysig_t *s) {
#ifdef __CPU_IS_BIG_ENDIAN
  return s;
#else
  uint32_t size = s->size;
  unsigned char* p = (unsigned char*)&size;
  if(p[0] != 0) {
    s->size = htobe32(s->size);
  }
  return s;
#endif
}

pcp_keysig_t *keysig2native(pcp_keysig_t *s) {
#ifdef __CPU_IS_BIG_ENDIAN
  return s;
#else
  s->size = be32toh(s->size);
  return s;
#endif
}

Buffer *pcp_keysig2blob(pcp_keysig_t *s) {
  Buffer *b = buffer_new(256, "keysig2blob");
  buffer_add8(b, s->type);
  buffer_add32be(b, s->size);
  buffer_add(b, s->belongs, 17);
  buffer_add(b, s->checksum, 32);
  buffer_add(b, s->blob, s->size);
  return b;
}

pcp_keysig_t *pcp_keysig_new(Buffer *blob) {
  pcp_keysig_t *sk = ucmalloc(sizeof(pcp_keysig_t));

  uint8_t type = buffer_get8(blob);
  uint32_t size = buffer_get32na(blob);
  
  byte *checksum = ucmalloc(32);
  buffer_get_chunk(blob, checksum, 32);
  
  sk->blob = ucmalloc(size);
  buffer_get_chunk(blob, sk->blob, size);

  sk->size = size;
  sk->type = type;
  memcpy(sk->checksum, checksum, 32);

  ucfree(checksum, 32);

  return sk;
}