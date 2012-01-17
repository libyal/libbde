/*
 * Common output functions for the bdetools
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

#if !defined( _BDEOUTPUT_H )
#define _BDEOUTPUT_H

#include <common.h>
#include <types.h>

#include <libsystem.h>

#if defined( __cplusplus )
extern "C" {
#endif

void bdeoutput_copyright_fprint(
      FILE *stream );

void bdeoutput_version_fprint(
      FILE *stream,
      const char *program );

void bdeoutput_version_detailed_fprint(
      FILE *stream,
      const char *program );

#if defined( __cplusplus )
}
#endif

#endif

