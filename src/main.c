#include "database.h"
#include "menu.h"

#define SQL_SCHEMA_USERS    "CREATE TABLE IF NOT EXISTS Users (" \
                            " Id INTEGER PRIMARY KEY," \
                            " Username TEXT UNIQUE," \
                            " Password TEXT" \
                            " );"

#define SQL_SCHEMA_RECORDS  "CREATE TABLE IF NOT EXISTS Records ("  \
                            " Id INTEGER PRIMARY KEY,"  \
                            " UserId INT,"  \
                            " Username TEXT,"  \
                            " AccountId INT,"  \
                            " CreationDate TEXT," \
                            " Country TEXT,"  \
                            " Phone TEXT,"  \
                            " Balance REAL,"  \
                            " AccountType TEXT"  \
                            " );" 

#define SQL_SCHEMA_TRANSTS  "CREATE TABLE IF NOT EXISTS Transactions (" \
                            " Id INTEGER PRIMARY KEY," \
                            " UserId INT" \
                            " AccountId INT," \
                            " Debit REAL," \
                            " Credit REAL " \
                            " );"

// debit  $+ account / flow in of money
// credit $- account / flow out of money

void mainMenu(const struct user_t* user)
{
    bool quit = false;

    while (!quit)
    {
        system("clear");
    
        printf("\n\t===================== ATM ========================\n");
        printf("\n\t↓↓ Feel free to choose one of the options below ↓↓\n");
        printf("\n\t[1] →  Create a new account\n");
        printf("\n\t[2] →  Update account information\n");
        printf("\n\t[3] →  Check accounts\n");
        printf("\n\t[4] →  Check list of owned account\n");
        printf("\n\t[5] →  Make Transaction\n");
        printf("\n\t[6] →  Remove existing account\n");
        printf("\n\t[7] →  Transfer ownership\n");
        printf("\n\t[8] →  Exit\n");

        const char option = immediate_getch();

        switch (option)
        {
            case '1':
                do
                { menu_account_create(*user); }
                while (ask_continuation("\nCreate an account [1] or cancel [0] ?"));
                break;
            case '2':
                do
                { menu_account_update(*user); }
                while (ask_continuation("\nUpdate an information [1] or cancel [0] ?"));
                break;
            case '3':
                menu_account_check_details(*user);
                immediate_getch();
                break;
            case '4':
                menu_account_check_all(*user);
                immediate_getch();
                break;
            case '5':
                menu_transaction_create(*user);
                immediate_getch();
                break;
            case '6':
                menu_account_remove(*user);
                immediate_getch();
                break;
            case '7':
                menu_account_transfer_owner(*user);
                immediate_getch();
                break;
            case '8':
                quit = true;
                break;
        }
    }
};

void initMenu(struct user_t *user)
{
    int success = false;
    
    system("clear");

    printf("\n\t===================== ATM ========================\n");
    printf("\n\t↓↓        Feel free to login or register        ↓↓\n");
    printf("\n\t[1] →  Login\n");
    printf("\n\t[2] →  Register\n");
    printf("\n\t[3] →  Exit\n");

    while (!success)
    {
        const char option = immediate_getch();

        switch (option)
        {
        case '1':
            if (menu_user_login(user))
                success = true;
            else {
                printf("\n ✖ Incorrect username or password.\n");
                exit(0);
            }
            break;
        case '2':
            if (menu_user_register(user))
                success = true;
            else {
                printf("\n ✖ Username already taken or an error occured.\n");
                exit(0);
            }
            break;
        case '3':
            exit(0);
            break;
        }
    }
};

int main()
{   
    if (sqlite3_open("atm.sqlite3", &db) != SQLITE_OK)
        return 1;

    if (sqlite3_exec(db, SQL_SCHEMA_USERS, NULL, NULL, NULL) != SQLITE_OK)
        return 1;

    if (sqlite3_exec(db, SQL_SCHEMA_RECORDS, NULL, NULL, NULL) != SQLITE_OK)
        return 1;

    if (sqlite3_exec(db, SQL_SCHEMA_TRANSTS, NULL, NULL, NULL) != SQLITE_OK)
        return 1;

    setbuf(stdout, NULL);

    struct user_t loggedIn;
    initMenu(&loggedIn);
    mainMenu(&loggedIn);

    sqlite3_close(db);

    return 0;
}
