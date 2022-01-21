/*
 * Library to access the BitLocker Drive Encryption (BVE) format
 *
 * Copyright (C) 2011-2022, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined( _LIBBDE_H )
#define _LIBBDE_H

#include <libbde/codepage.h>
#include <libbde/definitions.h>
#include <libbde/error.h>
#include <libbde/extern.h>
#include <libbde/features.h>
#include <libbde/types.h>

#include <stdio.h>

#if defined( LIBBDE_HAVE_BFIO )
#include <libbfio.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

/* -------------------------------------------------------------------------
 * Support functions
 * ------------------------------------------------------------------------- */

/* Returns the library version
 */
LIBBDE_EXTERN \
const char *libbde_get_version(
             void );

/* Returns the access flags for reading
 */
LIBBDE_EXTERN \
int libbde_get_access_flags_read(
     void );

/* Retrieves the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_get_codepage(
     int *codepage,
     libbde_error_t **error );

/* Sets the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_set_codepage(
     int codepage,
     libbde_error_t **error );

/* Determines if a file contains a BDE volume signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBBDE_EXTERN \
int libbde_check_volume_signature(
     const char *filename,
     libbde_error_t **error );

#if defined( LIBBDE_HAVE_WIDE_CHARACTER_TYPE )

/* Determines if a file contains a BDE volume signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBBDE_EXTERN \
int libbde_check_volume_signature_wide(
     const wchar_t *filename,
     libbde_error_t **error );

#endif /* defined( LIBBDE_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBBDE_HAVE_BFIO )

/* Determines if a file contains a BDE volume signature using a Basic File IO (bfio) handle
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBBDE_EXTERN \
int libbde_check_volume_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libbde_error_t **error );

#endif /* defined( LIBBDE_HAVE_BFIO ) */

/* -------------------------------------------------------------------------
 * Notify functions
 * ------------------------------------------------------------------------- */

/* Sets the verbose notification
 */
LIBBDE_EXTERN \
void libbde_notify_set_verbose(
      int verbose );

/* Sets the notification stream
 * Returns 1 if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_notify_set_stream(
     FILE *stream,
     libbde_error_t **error );

/* Opens the notification stream using a filename
 * The stream is opened in append mode
 * Returns 1 if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_notify_stream_open(
     const char *filename,
     libbde_error_t **error );

/* Closes the notification stream if opened using a filename
 * Returns 0 if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_notify_stream_close(
     libbde_error_t **error );

/* -------------------------------------------------------------------------
 * Error functions
 * ------------------------------------------------------------------------- */

/* Frees an error
 */
LIBBDE_EXTERN \
void libbde_error_free(
      libbde_error_t **error );

/* Prints a descriptive string of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_error_fprint(
     libbde_error_t *error,
     FILE *stream );

/* Prints a descriptive string of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_error_sprint(
     libbde_error_t *error,
     char *string,
     size_t size );

/* Prints a backtrace of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_error_backtrace_fprint(
     libbde_error_t *error,
     FILE *stream );

/* Prints a backtrace of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_error_backtrace_sprint(
     libbde_error_t *error,
     char *string,
     size_t size );

/* -------------------------------------------------------------------------
 * Volume functions
 * ------------------------------------------------------------------------- */

/* Creates a volume
 * Make sure the value volume is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_initialize(
     libbde_volume_t **volume,
     libbde_error_t **error );

/* Frees a volume
 * Returns 1 if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_free(
     libbde_volume_t **volume,
     libbde_error_t **error );

/* Signals the volume to abort its current activity
 * Returns 1 if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_signal_abort(
     libbde_volume_t *volume,
     libbde_error_t **error );

/* Opens a volume
 * Returns 1 if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_open(
     libbde_volume_t *volume,
     const char *filename,
     int access_flags,
     libbde_error_t **error );

#if defined( LIBBDE_HAVE_WIDE_CHARACTER_TYPE )

/* Opens a volume
 * Returns 1 if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_open_wide(
     libbde_volume_t *volume,
     const wchar_t *filename,
     int access_flags,
     libbde_error_t **error );

#endif /* defined( LIBBDE_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBBDE_HAVE_BFIO )

/* Opens a volume using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_open_file_io_handle(
     libbde_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libbde_error_t **error );

#endif /* defined( LIBBDE_HAVE_BFIO ) */

/* Closes a volume
 * Returns 0 if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_close(
     libbde_volume_t *volume,
     libbde_error_t **error );

/* Determines if the volume is locked
 * Returns 1 if locked, 0 if not or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_is_locked(
     libbde_volume_t *volume,
     libbde_error_t **error );

/* Unlocks the volume
 * Returns 1 if the volume is unlocked, 0 if not or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_unlock(
     libbde_volume_t *volume,
     libbde_error_t **error );

/* Reads unencrypted data at the current offset into a buffer
 * Returns the number of bytes read or -1 on error
 */
LIBBDE_EXTERN \
ssize_t libbde_volume_read_buffer(
         libbde_volume_t *volume,
         void *buffer,
         size_t buffer_size,
         libbde_error_t **error );

/* Reads unencrypted data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
LIBBDE_EXTERN \
ssize_t libbde_volume_read_buffer_at_offset(
         libbde_volume_t *volume,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libbde_error_t **error );

/* Seeks a certain offset within the unencrypted data
 * Returns the offset if seek is successful or -1 on error
 */
LIBBDE_EXTERN \
off64_t libbde_volume_seek_offset(
         libbde_volume_t *volume,
         off64_t offset,
         int whence,
         libbde_error_t **error );

/* Retrieves the current offset within the unencrypted data
 * Returns 1 if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_get_offset(
     libbde_volume_t *volume,
     off64_t *offset,
     libbde_error_t **error );

/* Retrieves the size
 * Returns 1 if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_get_size(
     libbde_volume_t *volume,
     size64_t *size,
     libbde_error_t **error );

/* Retrieves the encryption method
 * Returns 1 if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_get_encryption_method(
     libbde_volume_t *volume,
     uint16_t *encryption_method,
     libbde_error_t **error );

/* Retrieves the volume identifier
 * The identifier is a GUID and is 16 bytes of size
 * Returns 1 if successful, 0 if not or or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_get_volume_identifier(
     libbde_volume_t *volume,
     uint8_t *guid_data,
     size_t guid_data_size,
     libbde_error_t **error );

/* Retrieves the creation date and time
 * Returns 1 if successful, 0 if not or or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_get_creation_time(
     libbde_volume_t *volume,
     uint64_t *filetime,
     libbde_error_t **error );

/* Retrieves the UTF-8 string size of the volume description
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_get_utf8_description_size(
     libbde_volume_t *volume,
     size_t *utf8_string_size,
     libbde_error_t **error );

/* Retrieves the UTF-8 string value of the volume description
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_get_utf8_description(
     libbde_volume_t *volume,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libbde_error_t **error );

/* Retrieves the UTF-16 string size of the volume description
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_get_utf16_description_size(
     libbde_volume_t *volume,
     size_t *utf16_string_size,
     libbde_error_t **error );

/* Retrieves the UTF-16 string value of the volume description
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_get_utf16_description(
     libbde_volume_t *volume,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libbde_error_t **error );

/* Retrieves the number of volume master key protectors
 * Returns 1 if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_get_number_of_key_protectors(
     libbde_volume_t *volume,
     int *number_of_key_protectors,
     libbde_error_t **error );

/* Retrieves a specific volume master key protector
 * Returns 1 if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_get_key_protector_by_index(
     libbde_volume_t *volume,
     int key_protector_index,
     libbde_key_protector_t **key_protector,
     libbde_error_t **error );

/* Retrieves a specific volume master key protector
 *
 * This function deprecated use libbde_volume_get_key_protector_by_index instead
 *
 * Returns 1 if successful or -1 on error
 */
LIBBDE_DEPRECATED \
LIBBDE_EXTERN \
int libbde_volume_get_key_protector(
     libbde_volume_t *volume,
     int key_protector_index,
     libbde_key_protector_t **key_protector,
     libbde_error_t **error );

/* Sets the keys
 * This function needs to be used before one of the open functions
 * Returns 1 if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_set_keys(
     libbde_volume_t *volume,
     const uint8_t *full_volume_encryption_key,
     size_t full_volume_encryption_key_size,
     const uint8_t *tweak_key,
     size_t tweak_key_size,
     libbde_error_t **error );

/* Sets an UTF-8 formatted password
 * This function needs to be used before one of the open functions
 * Returns 1 if successful, 0 if password is invalid or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_set_utf8_password(
     libbde_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libbde_error_t **error );

/* Sets an UTF-16 formatted password
 * This function needs to be used before one of the open functions
 * Returns 1 if successful, 0 if password is invalid or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_set_utf16_password(
     libbde_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libbde_error_t **error );

/* Sets an UTF-8 formatted recovery password
 * This function needs to be used before one of the open functions
 * Returns 1 if successful, 0 if recovery password is invalid or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_set_utf8_recovery_password(
     libbde_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libbde_error_t **error );

/* Sets an UTF-16 formatted recovery password
 * This function needs to be used before one of the open functions
 * Returns 1 if successful, 0 if recovery password is invalid or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_set_utf16_recovery_password(
     libbde_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libbde_error_t **error );

/* Reads the startup key from a .BEK file
 * This function needs to be used before one of the open functions
 * Returns 1 if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_read_startup_key(
     libbde_volume_t *volume,
     const char *filename,
     libbde_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Reads the startup key from a .BEK file
 * This function needs to be used before one of the open functions
 * Returns 1 if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_read_startup_key_wide(
     libbde_volume_t *volume,
     const wchar_t *filename,
     libbde_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBBDE_HAVE_BFIO )

/* Reads the startup key from a .BEK file using a Basic File IO (bfio) handle
 * This function needs to be used before one of the open functions
 * Returns 1 if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_read_startup_key_file_io_handle(
     libbde_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     libbde_error_t **error );

#endif /* defined( LIBBDE_HAVE_BFIO ) */

/* -------------------------------------------------------------------------
 * Key protector functions
 * ------------------------------------------------------------------------- */

/* Frees a key protector
 * Returns 1 if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_key_protector_free(
     libbde_key_protector_t **key_protector,
     libbde_error_t **error );

/* Retrieves the identifier
 * The identifier is a GUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_key_protector_get_identifier(
     libbde_key_protector_t *key_protector,
     uint8_t *guid_data,
     size_t guid_data_size,
     libbde_error_t **error );

/* Retrieves the type
 * Returns 1 if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_key_protector_get_type(
     libbde_key_protector_t *key_protector,
     uint16_t *type,
     libbde_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBDE_H ) */
