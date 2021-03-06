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

#ifndef HAVE_PCP_PCPSTEAM_H
#define HAVE_PCP_PCPSTEAM_H

#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include "mem.h"
#include "structs.h"
#include "util.h"
#include "defines.h"
#include "context.h"
#include "buffer.h"
#include "z85.h"

/**
 * \defgroup Pcpstream PCPSTREAMS
 * @{
 
 I/O wrapper for files or buffers.

  Simple wrapper around FILE streams or Buffers, depending
  how the user initialized them. The Pcpstream object behaves
  always the same and it doesn't matter how it's backed.

  We use it in the lib, e.g. in the crypto routines. That way
  we can support blockwise crypto on buffers or files.

  Streams are, just like iostreams in c++, either output or
  input mode.

  Sample usage:

  \include tests/streamtest.c

 */




/* initialize a new empty stream */
Pcpstream *ps_init(void);

/** Create a new stream, backed with an open file.

    The stream used for in- or output.

    \param[in] backendfd An open FILE stream.

    \return Returns a Pcpstream structure.
*/
Pcpstream *ps_new_file(FILE *backendfd);

/** Create a new input stream, backed with filled a buffer.

    This kind of stream can be used for reading only.

    \param[in] b A Buffer object filled with data.

    \return Returns a Pcpstream structure.
 */
Pcpstream *ps_new_inbuffer(Buffer *b);

/** Create a new output stream, backed with a buffer.

    The buffer used to write data to will be allocated
    and filled by the class. You can retrieve it later.

    \return Returns a Pcpstream structure.
 */
Pcpstream *ps_new_outbuffer();


/** Read bytes into the given buffer from the current stream.

    This function reads 'readbytes' bytes from the stream
    into given buf. The buffer needs to be properly allocated
    by the caller in advance to have at least readbytes len.

    Sets eof=1 if end of file or end of buffer has been reached.

    Sets err=1 if an error occurred, fatals() maybe set, or errno.

    See ps_end() and ps_err().

    \param[in] stream The input stream to read from.

    \param[out] buf The buffer where to put read bytes.

    \param[in] readbytes The number of bytes to read.

    \return Returns the bytes read, if there's nothing more to read, it returns 0.
*/
size_t ps_read(Pcpstream *stream, void *buf, size_t readbytes);

/** Write bytes from the given buffer into the current stream.

    This function writes 'writebytes' bytes from the given buf
    into the stream

    Sets err in case of an error. See ps_err().

    \param[out] stream The input stream to write to.

    \param[in] buf The buffer containing data to put into the stream.

    \param[in] writebytes The number of bytes to write.

    \return Returns the number of bytes written. in case of errors it returns 0.
*/
size_t ps_write(Pcpstream *stream, void *buf, size_t writebytes);

/** Finalize writing the stream.

    You NEED to call this function after you're done writing
    to the stream if you enabled Z85 armoring using ps_armor().

    It writes the remaining bytes (encoded) to the stream destination.

    \param[in] stream The input stream to write to.

    \return Returns the number of bytes written. in case of errors it returns 0.
 */
size_t ps_finish(Pcpstream *stream);

/** Write a formatted string to the stream.

    Use an printf() style format string to print something out
    to a stream.

    Sets err in case of an error. See ps_err().

    \param[out] stream The input stream to read from.

    \param[in] fmt The printf() compatible format description.

    \param[in] ... A variable number of arguments for the format string.

    \return Returns the number of bytes written. in case of errors it returns 0.
*/
size_t ps_print(Pcpstream *stream, const char * fmt, ...);

/** Tell the current read or write offset.

    This function works like ftell() on a FILE stream or
    like Buffer->offset in the Buffer class.

    \param[in] stream The input stream to read from.

    \return Returns the the number of bytes read/written so far.
 */
size_t ps_tell(Pcpstream *stream);

/** Access the Buffer backend pointer.

    Use this function to access the underlying Buffer object
    of an output stream to access the contents written to it.
    Only usefull if the stream have been initialized with
    ps_new_outbuffer().

    \param[in] stream The stream object.

    \return Returns a pointer to the Buffer object.
 */
Buffer *ps_buffer(Pcpstream *stream);

/** Close the stream and frees allocated memory.

    If the backend of the stream was a FILE stream, close it, unless it is
    stdin, stdout or stderr.

    If the backend was a Buffer, clear and free it.

    \param[in] stream The stream to close.
 */
void ps_close(Pcpstream *stream);

/** Check if EOF have been reached.

    This function can be used to check if there are no more
    bytes to read. This will happen if we reach EOF with a
    FILE backed stream or buffer_done() with a Buffer backed
    stream.

    \param[in] stream The stream object.

    \return Returns 1 if we reached EOF, 0 otherwise
*/
int ps_end(Pcpstream *stream);

/** Check if an error occurred during a read or write operation.

    \param[in] stream The stream object.

    \return Returns 1 if there were any errors or 0 otherwise. Also check errno() and fatals_ifany().
*/
int ps_err(Pcpstream *stream);


/** Enable auto Z85 encoding detection for an input stream.

    If you're not sure if your input is Z85 encoded, enable
    detection.

    \param[in] stream The stream object.

    \param[in] blocksize The blocksize to for Z85 decoding (if encoded).
 */
void ps_setdetermine(Pcpstream *stream, size_t blocksize);


/** Enable Z85 encoding for an output stream.

    \param[in] stream The stream object.

    \param[in] blocksize The blocksize to for Z85 encoding.
 */
void ps_armor(Pcpstream *stream, size_t blocksize);


/** Disable Z85 encoding for an output stream.

    \param[in] stream The stream object.
 */
void ps_unarmor(Pcpstream *stream);



/* read from primary source, decode z85 and out into cache.
   if buf != NULL, consider it as the start of encoded data
   and remove headers and comments, then continue as normal. */
size_t ps_read_decode(Pcpstream *stream);

/* determine if primary source is z85 encoded, put the data
   read from it into the cache */
void ps_determine(Pcpstream *stream);

/* read and decode the next chunk and put it into stream->next */
size_t ps_read_next(Pcpstream *stream);

/* return readbytes from cache. if it is more than left in the cache
   fetch (and decode) the next chunk, append it to cache and return from
   that */
size_t ps_read_cached(Pcpstream *stream, void *buf, size_t readbytes);

/* really read from the source */
size_t ps_read_raw(Pcpstream *stream, void *buf, size_t readbytes);

/* helper, Z85 encodes current cache into the dst buffer */
void ps_write_encode(Pcpstream *stream, Buffer *dst);

/* really write the buffer z into the output stream */
size_t ps_write_buf(Pcpstream *stream, Buffer *z);

/* tell if we really reached eof, caching or not. 1=eof, 0=ok */
int ps_left(Pcpstream *stream);

/** Read a line from the stream.

    \param[in] stream The stream object.
    \param[out] line Linecontent will be written to this Buffer.

    \return Returns the number of bytes read or -1 if PSMAXLINE have been
            reached or the input doesn't have any newlines at all.
 */
int ps_readline(Pcpstream *stream, Buffer *line);

#endif // HAVE_PCP_PCPSTEAM_H


/**@}*/
