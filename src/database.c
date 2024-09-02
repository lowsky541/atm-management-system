#include "database.h"

sqlite3* db;

// find_user_by_id
//   userId: the id of the user to find
//   user: structure that receive the user informations;
//         can be NULL for a check of existence
//   returns: true if there is a match else false
bool find_user_by_id(int userId, user_t* user)
{
    sqlite3_stmt* stmt;
    int code;
    bool found = false;

    sqlite3_prepare_v2(db, "SELECT * FROM Users WHERE Id = ?", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, userId);
    code = sqlite3_step(stmt);

    if (code == SQLITE_ROW) {
        if (user != NULL) {
            user->id = sqlite3_column_int(stmt, 0);
            strcpy(user->username, sqlite3_column_text(stmt, 1));
            strcpy(user->password, sqlite3_column_text(stmt, 2));
        }
        found = true;
    } else if (code == SQLITE_DONE) found = false;
    
    sqlite3_finalize(stmt);

    return found;
}

// find_user_by_username
//   username: the username of the user to find
//   user: structure that receive the user informations;
//         can be NULL for a check of existence
//   returns: true if there is a match else false
bool find_user_by_name(char* username, user_t* user)
{
    sqlite3_stmt* stmt;
    int code;
    bool found = false;

    sqlite3_prepare_v2(db, "SELECT * FROM Users WHERE Username = ?", -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, username, -1, NULL);
    code = sqlite3_step(stmt);

    if (code == SQLITE_ROW) {
        if (user != NULL) {
            user->id = sqlite3_column_int(stmt, 0);
            strcpy(user->username, sqlite3_column_text(stmt, 1));
            strcpy(user->password, sqlite3_column_text(stmt, 2));
        }
        found = true;
    } else if (code == SQLITE_DONE) found = false;
    
    sqlite3_finalize(stmt);

    return found;
}

void insert_user(user_t user, int* rowid)
{
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "INSERT INTO Users(Username, Password) VALUES (?, ?);", -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, user.username, -1, NULL);
    sqlite3_bind_text(stmt, 2, user.password, -1, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rowid != NULL) *rowid = sqlite3_last_insert_rowid(db);
}

void delete_user(user_id_t userId)
{
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "DELETE FROM Users WHERE Id = ?", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

bool find_record_by_aid(const user_id_t uid, const account_id_t aid, record_t* record)
{
    sqlite3_stmt* stmt;
    int code;
    bool found = false;

    sqlite3_prepare_v2(db, "SELECT * FROM Records WHERE UserId = ? AND AccountId = ?", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, uid);
    sqlite3_bind_int(stmt, 2, aid);
    code = sqlite3_step(stmt);

    if (code == SQLITE_ROW) {
        if (record != NULL) {
            char date[10];

            record->id = sqlite3_column_int(stmt, 0);
            record->userId = sqlite3_column_int(stmt, 1);
            strcpy(record->username, sqlite3_column_text(stmt, 2));
            record->accountId = sqlite3_column_int(stmt, 3);
            strcpy(date, sqlite3_column_text(stmt, 4));
            sscanf(date, "%d/%d/%d", &record->creationDate.month, &record->creationDate.day, &record->creationDate.year);
            strcpy(record->country, sqlite3_column_text(stmt, 5));
            record->phone = sqlite3_column_int(stmt, 6);
            record->balance = sqlite3_column_double(stmt, 7);
            strcpy(record->type, sqlite3_column_text(stmt, 8));
        }
        found = true;
    } else if (code == SQLITE_DONE) found = false;
    
    sqlite3_finalize(stmt);

    return found;
}

void update_record_balance(const user_id_t uid, const account_id_t aid, double balance)
{
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "UPDATE Records SET Balance = ? WHERE UserId = ? AND AccountId = ?", -1, &stmt, NULL);
    sqlite3_bind_double(stmt, 1, balance);
    sqlite3_bind_int(stmt, 2, uid);
    sqlite3_bind_int(stmt, 3, aid);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void update_record_phone(const user_id_t uid, const account_id_t aid, int phone)
{
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "UPDATE Records SET Phone = ? WHERE UserId = ? AND AccountId = ?", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, phone);
    sqlite3_bind_int(stmt, 2, uid);
    sqlite3_bind_int(stmt, 3, aid);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void update_record_country(const user_id_t uid, const account_id_t aid, char* country)
{
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "UPDATE Records SET Country = ? WHERE UserId = ? AND AccountId = ?", -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, country, -1, NULL);
    sqlite3_bind_int(stmt, 2, uid);
    sqlite3_bind_int(stmt, 3, aid);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void update_record_owner(const user_id_t uid, const account_id_t aid, char* username, user_id_t userId)
{
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "UPDATE Records SET UserId = ?, Username = ? WHERE UserId = ? AND AccountId = ?", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_bind_text(stmt, 2, username, -1, NULL);
    sqlite3_bind_int(stmt, 3, uid);
    sqlite3_bind_int(stmt, 4, aid);
    sqlite3_step(stmt);

    sqlite3_finalize(stmt);
}

void insert_record(record_t record, int* rowid)
{
    sqlite3_stmt* stmt;

    char date[11];
    sprintf(date, "%02d/%02d/%04d", record.creationDate.month, record.creationDate.day, record.creationDate.year);

    sqlite3_prepare_v2(db, "INSERT INTO Records(UserId, Username, AccountId, CreationDate, Country, Phone, Balance, AccountType)"
                           "VALUES (?, ?, ?, ?, ?, ?, ?, ?);", -1, &stmt, NULL);

    sqlite3_bind_int(stmt,    1, record.userId);
    sqlite3_bind_text(stmt,   2, record.username, -1, NULL);
    sqlite3_bind_int(stmt,    3, record.accountId);
    sqlite3_bind_text(stmt,   4, date, -1, NULL);
    sqlite3_bind_text(stmt,   5, record.country, -1, NULL);
    sqlite3_bind_int(stmt,    6, record.phone);
    sqlite3_bind_double(stmt, 7, record.balance);
    sqlite3_bind_text(stmt,   8, record.type, -1, NULL);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rowid != NULL) *rowid = sqlite3_last_insert_rowid(db);
}

void delete_record(record_id_t rid)
{
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "DELETE FROM Records WHERE Id = ?", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, rid);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void delete_record_by_aid(const user_id_t uid, const account_id_t aid)
{
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "DELETE FROM Records WHERE UserId = ? AND AccountId = ?", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, uid);
    sqlite3_bind_int(stmt, 2, aid);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void insert_transaction(transaction_t transaction, int* rowid)
{
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "INSERT INTO Transactions (UserId, AccountId, Debit, Credit) VALUES (?, ?, ?, ?);", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, transaction.userId);
    sqlite3_bind_int(stmt, 2, transaction.aid);
    sqlite3_bind_int(stmt, 3, transaction.debit);
    sqlite3_bind_int(stmt, 4, transaction.credit);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rowid != NULL) *rowid = sqlite3_last_insert_rowid(db);
}

void delete_transaction(transaction_id_t tid)
{
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "DELETE FROM Transactions WHERE Id = ?", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, tid);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void delete_transaction_by_aid(user_id_t uid, account_id_t aid)
{
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "DELETE FROM Transactions WHERE UserId = ? AND AccountId = ?", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, uid);
    sqlite3_bind_int(stmt, 2, aid);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}