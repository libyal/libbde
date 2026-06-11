/*
 * FVEAutoUnlock blob functions
 *
 * Copyright (C) 2011-2026, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBBDE_FVE_AUTO_UNLOCK_H )
#define _LIBBDE_FVE_AUTO_UNLOCK_H

#include <common.h>
#include <types.h>

#include "libbde_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

/* Reads a FVEAutoUnlock registry blob (the binary content of the
 * HKLM\SYSTEM\CurrentControlSet\Control\FVEAutoUnlock\{volume-GUID} "Data" value)
 * and recovers the plaintext external key of the secondary (auto-unlocked) volume
 * by AES-CCM decrypting the embedded encrypted key with the auto-unlock key of the
 * operating system volume.
 *
 * The recovered external key (typically 32 bytes) is copied into external_key and
 * its size returned through external_key_size. When vmk_identifier is provided the
 * 16-byte VMK slot identifier (GUID) carried by the blob is copied into it.
 *
 * Returns 1 if successful, 0 if the external key could not be recovered or -1 on error
 */
int libbde_fve_auto_unlock_read_blob(
     const uint8_t *blob_data,
     size_t blob_data_size,
     const uint8_t *auto_unlock_key,
     size_t auto_unlock_key_size,
     uint8_t *external_key,
     size_t external_key_size,
     size_t *external_key_data_size,
     uint8_t *vmk_identifier,
     size_t vmk_identifier_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBDE_FVE_AUTO_UNLOCK_H ) */
