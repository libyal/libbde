/*
 * The libuna header wrapper
 *
 * Copyright (C) 2011-2012, Google Inc.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if !defined( _LIBBDE_LIBUNA_H )
#define _LIBBDE_LIBUNA_H

#include <common.h>

/* Define HAVE_LOCAL_LIBUNA for local use of libuna
 */
#if defined( HAVE_LOCAL_LIBUNA )

#include <libuna_base16_stream.h>
#include <libuna_base32_stream.h>
#include <libuna_base64_stream.h>
#include <libuna_byte_stream.h>
#include <libuna_error.h>
#include <libuna_unicode_character.h>
#include <libuna_url_stream.h>
#include <libuna_utf16_stream.h>
#include <libuna_utf16_string.h>
#include <libuna_utf32_stream.h>
#include <libuna_utf32_string.h>
#include <libuna_utf7_stream.h>
#include <libuna_utf8_stream.h>
#include <libuna_utf8_string.h>
#include <libuna_types.h>

#elif defined( HAVE_LIBUNA_H )

/* If libtool DLL support is enabled set LIBUNA_DLL_IMPORT
 * before including libuna.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT )
#define LIBUNA_DLL_IMPORT
#endif

#include <libuna.h>

#else
#error Missing libuna.h
#endif

#endif

