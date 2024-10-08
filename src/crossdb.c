/******************************************************************************
* Copyright (c) 2024-present JC Wang. All rights reserved
*
*   https://crossdb.org
*   https://github.com/crossdb-org/crossdb
*
* This program is free software: you can redistribute it and/or modify it under
* the terms of the GNU General Public License, version 3 or later, as published 
* by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
* FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with 
* this program. If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#undef XDB_STATIC
#define XDB_STATIC static

#include "../include/crossdb.h"
#include "core/xdb_cfg.h"
#include "lib/xdb_lib.c"
#include "core/xdb_common.h"
#include "parser/xdb_stmt.h"
#include "core/xdb_store.h"
#include "core/xdb_db.h"
#include "core/xdb_crud.h"
#include "core/xdb_hash.h"
#include "core/xdb_sql.h"
#include "core/xdb_sysdb.h"
#include "core/xdb_table.h"
#include "core/xdb_index.h"
#include "core/xdb_trans.h"
#if (XDB_ENABLE_SERVER == 1)
#include "server/xdb_server.h"
#endif
#include "core/xdb_conn.h"
#include "admin/xdb_shell.h"
#include "admin/xdb_backup.h"
#if (XDB_ENABLE_WAL == 1)
#include "core/xdb_wal.h"
#endif

#include "parser/xdb_parser.c"
#include "core/xdb_store.c"
#include "core/xdb_sysdb.c"
#include "core/xdb_db.c"
#include "core/xdb_crud.c"
#include "core/xdb_index.c"
#include "core/xdb_hash.c"
#include "core/xdb_table.c"
#include "core/xdb_trans.c"
#include "core/xdb_conn.c"
#if (XDB_ENABLE_SERVER == 1)
#include "server/xdb_client.c"
#include "server/xdb_server.c"
#endif
#include "core/xdb_sql.c"
#if (XDB_ENABLE_WAL == 1)
#include "core/xdb_wal.c"
#endif
#if (XDB_ENABLE_JNI == 1)
#include "jni/xdb_jni.c"
#endif
#include "admin/xdb_shell.c"
#include "admin/xdb_backup.c"

static const char* xdb_type2str(xdb_type_t tp) 
{
	const char *id2str[] = {
		[XDB_TYPE_NULL     ] = "NULL",
		[XDB_TYPE_TINYINT  ] = "TINYINT",
		[XDB_TYPE_SMALLINT ] = "SMALLINT",
		[XDB_TYPE_INT      ] = "INT",
		[XDB_TYPE_BIGINT   ] = "BIGINT",
		[XDB_TYPE_UTINYINT ] = "UTINYINT",
		[XDB_TYPE_USMALLINT] = "USMALLINT",
		[XDB_TYPE_UINT     ] = "UINT",
		[XDB_TYPE_UBIGINT  ] = "UBIGINT",
		[XDB_TYPE_FLOAT    ] = "FLOAT",
		[XDB_TYPE_DOUBLE   ] = "DOUBLE",
		[XDB_TYPE_TIMESTAMP] = "TIMESTAMP",
		[XDB_TYPE_CHAR     ] = "CHAR",
		[XDB_TYPE_BINARY   ] = "BINARY",
		[XDB_TYPE_VCHAR    ] = "VCHAR",
		[XDB_TYPE_VBINARY  ] = "VBINARY",
	};
	return tp <= XDB_ARY_LEN(id2str) ? id2str[tp] : "Unkonwn";
}

static bool s_xdb_bInit = false;

int 
xdb_init ()
{
	if (!s_xdb_bInit) {
#if (XDB_ENABLE_SERVER == 1)
		xdb_sock_init ();
#endif
		s_xdb_bInit = true;
	}
	return XDB_OK;
}

int 
xdb_exit ()
{
	// Close all opened DBs
	s_xdb_bInit = false;
	xdb_close_all_db (NULL);

	return XDB_OK;
}

const char* xdb_errmsg (xdb_res_t *pRes)
{
	return pRes->row_data ? (const char*)pRes->row_data : "OK";
}

const char* xdb_version ()
{
	return XDB_VERSION;
}
