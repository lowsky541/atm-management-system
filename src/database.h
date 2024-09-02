#ifndef DATA_H
#define DATA_H

#define FIELD_MAX 50
#define FIELD_COUNTRY_MAX 20

#include <sqlite3.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef int user_id_t;
typedef int record_id_t;
typedef int account_id_t;
typedef int transaction_id_t;

extern sqlite3* db;

struct date_t
{
    int month, day, year;
};

struct user_t
{
    user_id_t id;
    char username[FIELD_MAX];
    char password[FIELD_MAX];
};

struct record_t
{
    record_id_t id;
    int userId;
    char username[FIELD_MAX];
    int accountId;
    struct date_t creationDate;
    char country[FIELD_COUNTRY_MAX];
    int phone;
    double balance;
    char type[10];
};

struct transaction_t
{
    transaction_id_t id;
    user_id_t userId;
    account_id_t aid;
    double debit;
    double credit;
};

typedef struct date_t date_t;
typedef struct user_t user_t;
typedef struct record_t record_t;
typedef struct transaction_t transaction_t;

// users

bool find_user_by_id(int userId, user_t* user);
bool find_user_by_name(char* username, user_t* user);

void insert_user(user_t user, int* rowid);

void delete_user(user_id_t userId);

// records

bool find_record_by_aid(const user_id_t uid, const account_id_t aid, record_t* record);

void update_record_balance(const user_id_t uid, const account_id_t aid, double balance);
void update_record_phone(const user_id_t uid, const account_id_t aid, int phone);
void update_record_country(const user_id_t uid, const account_id_t aid, char* country);
void update_record_owner(const user_id_t uid, const account_id_t aid, char* username, user_id_t userId);

void insert_record(record_t record, int* rowid);

void delete_record_by_aid(const user_id_t uid, const account_id_t aid);
void delete_record(record_id_t recordId);

// transactions

void insert_transaction(transaction_t transaction, int* rowid);

void delete_transaction_by_aid(user_id_t uid, account_id_t aid);

#endif // DATA_H
