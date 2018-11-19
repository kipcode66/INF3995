#ifndef COMMON_DATABASE_THREADSAFE_SQLITE_HPP
#define COMMON_DATABASE_THREADSAFE_SQLITE_HPP

#include <sqlite3/sqlite3.h>
#include <mutex>
#include <condition_variable>

namespace elevation {

/* +=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=+
 * | Implementation of a thread safe             |
 * | version of step and prepare function        |
 * | of sqlite3. Modified to use the             |
 * | standard c++ library.                       |
 * | From :                                      |
 * |   https://www.sqlite.org/unlock_notify.html |
 * +=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=+
 */

/*
** A pointer to an instance of this structure is passed as the user-context
** pointer when registering for an unlock-notify callback.
*/
typedef struct UnlockNotification UnlockNotification;
struct UnlockNotification {
    bool fired;                    /* True after unlock event has occurred */
    std::condition_variable* cond; /* Condition variable to wait on */
    std::mutex* mutex;             /* Mutex to protect structure */
};

/*
** This function is an unlock-notify callback registered with SQLite.
*/
static void unlock_notify_cb(void **apArg, int nArg) {
    int i;
    for(i=0; i<nArg; i++) {
        UnlockNotification *p = (UnlockNotification *)apArg[i];
        p->mutex->lock();
        p->fired = true;
        p->cond->notify_one();
        p->mutex->unlock();
    }
}

/*
** This function assumes that an SQLite API call (either sqlite3_prepare_v2()
** or sqlite3_step()) has just returned SQLITE_LOCKED. The argument is the
** associated database connection.
**
** This function calls sqlite3_unlock_notify() to register for an
** unlock-notify callback, then blocks until that callback is delivered
** and returns SQLITE_OK. The caller should then retry the failed operation.
**
** Or, if sqlite3_unlock_notify() indicates that to block would deadlock
** the system, then this function returns SQLITE_LOCKED immediately. In
** this case the caller should not retry the operation and should roll
** back the current transaction (if any).
*/
static int wait_for_unlock_notify(sqlite3 *db) {
    int rc;
    UnlockNotification un;
    std::mutex mutex;
    std::unique_lock uLock(mutex);
    std::condition_variable cond;

    /* Initialize the UnlockNotification structure. */
    un.fired = false;
    un.mutex = &mutex;
    un.cond = &cond;

    /* Register for an unlock-notify callback. */
    rc = sqlite3_unlock_notify(db, unlock_notify_cb, (void *)&un);
    if (!( rc==SQLITE_LOCKED || rc==SQLITE_OK )) {
        throw std::runtime_error("Invalid SQLite error code for sqlite3_unlock_notify [code=" + std::to_string(rc) + "]");
    }

    /* The call to sqlite3_unlock_notify() always returns either SQLITE_LOCKED
    ** or SQLITE_OK.
    **
    ** If SQLITE_LOCKED was returned, then the system is deadlocked. In this
    ** case this function needs to return SQLITE_LOCKED to the caller so
    ** that the current transaction can be rolled back. Otherwise, block
    ** until the unlock-notify callback is invoked, then return SQLITE_OK.
    */
    if( rc==SQLITE_OK ) {
        std::unique_lock<std::mutex> lock(*un.mutex, std::adopt_lock);
        lock.lock();
        if( !un.fired ) {
            un.cond->wait(lock);
        }
        lock.unlock();
    }

    /* The mutex and condition variables are automatically destroyed. */

    return rc;
}

/*
** This function is a wrapper around the SQLite function sqlite3_step().
** It functions in the same way as step(), except that if a required
** shared-cache lock cannot be obtained, this function may block waiting for
** the lock to become available. In this scenario the normal API step()
** function always returns SQLITE_LOCKED.
**
** If this function returns SQLITE_LOCKED, the caller should rollback
** the current transaction (if any) and try again later. Otherwise, the
** system may become deadlocked.
*/
int sqlite3_blocking_step(sqlite3_stmt *pStmt) {
    int rc;
    while( SQLITE_LOCKED==(rc = sqlite3_step(pStmt)) || SQLITE_BUSY==rc ) {
        rc = wait_for_unlock_notify(sqlite3_db_handle(pStmt));
        if( rc!=SQLITE_OK ) break;
        sqlite3_reset(pStmt);
    }
    return rc;
}

/*
** This function is a wrapper around the SQLite function sqlite3_prepare_v2().
** It functions in the same way as prepare_v2(), except that if a required
** shared-cache lock cannot be obtained, this function may block waiting for
** the lock to become available. In this scenario the normal API prepare_v2()
** function always returns SQLITE_LOCKED.
**
** If this function returns SQLITE_LOCKED, the caller should rollback
** the current transaction (if any) and try again later. Otherwise, the
** system may become deadlocked.
*/
int sqlite3_blocking_prepare_v2(
    sqlite3 *db,              /* Database handle. */
    const char *zSql,         /* UTF-8 encoded SQL statement. */
    int nSql,                 /* Length of zSql in bytes. */
    sqlite3_stmt **ppStmt,    /* OUT: A pointer to the prepared statement */
    const char **pz           /* OUT: End of parsed string */
) {
    int rc;
    while( SQLITE_LOCKED==(rc = sqlite3_prepare_v2(db, zSql, nSql, ppStmt, pz)) || SQLITE_BUSY==rc ) {
        rc = wait_for_unlock_notify(db);
        if( rc!=SQLITE_OK ) break;
    }
    return rc;
}

}; // namespace elevation

#endif // !COMMON_DATABASE_THREADSAFE_SQLITE_HPP
