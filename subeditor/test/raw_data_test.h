/*
 * Copyright (C) 2016 Andre Q. Barbosa
 *
 * This file is part of Craped.
 *
 * Craped is free software: you can redistribute it and/or modify
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
 */

#ifndef RAW_DATA_TEST_H
#define RAW_DATA_TEST_H

#include <libsys/def.h>

#ifdef TEST_RAW_DATA

#include "raw_data.h"

#define TestRawData RawData
#define initTestRawData(b) rawDataInit(b, 50)
#define destroyTestRawData rawDataDestroy
#define testRawDataInsert rawDataInsert
#define testRawDataDelete rawDataDelete
#define testRawDataRead rawDataRead
#define testRawDataSize rawDataSize

#endif
#ifdef TEST_PAGED_RAW_DATA

#include "paged_raw_data.h"

#define TestRawData PagedRawData
#define initTestRawData pagedRawDataInit
#define destroyTestRawData pagedRawDataDestroy
#define testRawDataInsert pagedRawDataInsert
#define testRawDataDelete pagedRawDataDelete
#define testRawDataRead pagedRawDataRead
#define testRawDataSize pagedRawDataSize

#endif

#endif
