/*
 * Copyright (C) 2008 Search Solution Corporation. All rights reserved by Search Solution.
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */


/*
 * Public defines for value array scans
 */

#ifndef _METHOD_SCAN_H_
#define _METHOD_SCAN_H_

#ident "$Id$"

#if !defined (SERVER_MODE) && !defined (SA_MODE)
#error Belongs to server module
#endif /* !defined (SERVER_MODE) && !defined (SA_MODE) */

#include "dbtype_def.h"
#include "method_def.hpp"
#ifndef SERVER_MODE
#include "work_space.h"
#include "cursor.h"
#endif /* SERVER_MODE */
#include "storage_common.h"
#include "thread_compat.hpp"

// forward definitions
struct qfile_list_id;
struct qproc_db_value_list;
struct method_sig_list;
typedef struct method_sig_list METHOD_SIG_LIST;
struct val_list_node;

#ifdef SERVER_MODE
#define VACOMM_BUFFER_SIZE 4096

typedef struct vacomm_buffer VACOMM_BUFFER;
struct vacomm_buffer
{				/* */
  int length;			/* trans length */
  int status;			/* trans status */
  int error;			/* client error */
  int num_vals;			/* number of values */
  char *area;			/* buffer + header */
  char *buffer;			/* buffer */
  int cur_pos;			/* current position */
  int size;			/* size of buffer */
  int action;			/* client action */
};
#endif

#define MAX_XS_SCANBUF_DBVALS 256

typedef struct method_info METHOD_INFO;
struct method_info
{
  qfile_list_id *list_id;	/* list id for arguments */
  METHOD_SIG_LIST *method_sig_list;	/* method signatures */
};

typedef struct method_scan_buffer METHOD_SCAN_BUFFER;
struct method_scan_buffer
{				/* value array scanbuf */
  qproc_db_value_list *dbval_list;	/* ptrs into the value array */
  union
  {				/* ctl info based on type */
    METHOD_INFO method_ctl;
  } s;
#ifdef SERVER_MODE
  VACOMM_BUFFER *vacomm_buffer;
#else				/* SERVER_MODE */
  /* These are needed for calling */
  /* methods in standalone mode */
  DB_VALUE *vallist;		/* values from the input list file */
  DB_VALUE **valptrs;		/* ptrs to the above values */
  int *oid_cols;		/* OID columns in list file */
  CURSOR_ID crs_id;		/* cursor id */
  int val_cnt;			/* number of values in vallist */
#endif				/* SERVER_MODE */
};

extern int method_open_scan (THREAD_ENTRY * thread_p, METHOD_SCAN_BUFFER * scan_buf, qfile_list_id * list_id,
			     method_sig_list * method_sig_list);
extern int method_close_scan (THREAD_ENTRY * thread_p, METHOD_SCAN_BUFFER * scan_buf);
extern SCAN_CODE method_scan_next (THREAD_ENTRY * thread_p, METHOD_SCAN_BUFFER * scan_buf, val_list_node * val_list);
#endif /* _METHOD_SCAN_H_ */
