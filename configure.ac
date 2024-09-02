AC_PREREQ([2.71])
AC_INIT([atm-management-system], [1.0])

AC_LANG(C)
AC_PROG_CC

AC_CHECK_LIB(sqlite3, [
    sqlite3_bind_double,
    sqlite3_bind_int,
    sqlite3_bind_text,
    sqlite3_close,
    sqlite3_column_double,
    sqlite3_column_int,
    sqlite3_column_text,
    sqlite3_exec,
    sqlite3_finalize,
    sqlite3_last_insert_rowid,
    sqlite3_open,
    sqlite3_prepare_v2,
    sqlite3_step ], [], [
    echo "libsqlite3 is required"
    exit -1
])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
