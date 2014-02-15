/*
    This file is part of Pretty Curved Privacy (pcp1).

    Copyright (C) 2013 T.Linden.

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


#ifndef _HAVE_PCP_CRYPTO_H
#define _HAVE_PCP_CRYPTO_H

#include <sodium.h>
#include <string.h>
#include <stdio.h>
#include <sodium.h>
#include <stdlib.h>

#include "defines.h"
#include "mem.h"
#include "key.h"
#include "keyhash.h"
#include "ed.h"
#include "pcpstream.h"

size_t pcp_sodium_box(unsigned char **cipher,
                      unsigned char *cleartext,
                      size_t clearsize,
                      unsigned char *nonce,
                      unsigned char *secret,
                      unsigned char *pub);

int pcp_sodium_verify_box(unsigned char **cleartext, unsigned char* message,
                          size_t messagesize, unsigned char *nonce,
                          unsigned char *secret, unsigned char *pub);

unsigned char *pcp_box_encrypt(pcp_key_t *secret, pcp_pubkey_t *pub,
                               unsigned char *message, size_t messagesize,
			       size_t *csize);

unsigned char *pcp_box_decrypt(pcp_key_t *secret, pcp_pubkey_t *pub,
                               unsigned char *cipher, size_t ciphersize,
			       size_t *dsize);

size_t pcp_encrypt_stream(Pcpstream *in, Pcpstream* out, pcp_key_t *s, pcp_pubkey_t *p, int signcrypt);
size_t pcp_encrypt_stream_sym(Pcpstream *in, Pcpstream* out, unsigned char *symkey, int havehead, pcp_rec_t *recsign);

size_t pcp_decrypt_stream(Pcpstream *in, Pcpstream* out, pcp_key_t *s, unsigned char *symkey, int verify);
size_t pcp_decrypt_stream_sym(Pcpstream *in, Pcpstream* out, unsigned char *symkey, pcp_rec_t *recverify);

pcp_rec_t *pcp_rec_new(unsigned char *cipher, size_t clen, pcp_key_t *secret, pcp_pubkey_t *pub);
void pcp_rec_free(pcp_rec_t *r);

#endif /*  _HAVE_PCP_CRYPTO_H */
