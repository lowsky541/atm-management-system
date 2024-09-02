#include "menu.h"

bool menu_user_login(struct user_t* user)
{
    char username[50];
    char password[50];

    system("clear");

    printf("\n Bank Management System / Login\n");
    printf("\n Username: ");
    
    scanf("%50s", (char*) &username);

    struct termios context;
    terminal_echo(false, &context);

    printf(" Password: ");
    scanf("%50s", (char*) &password);

    terminal_echo(true, &context);

    struct user_t match;
    if (!find_user_by_name(username, &match))
        return false;

    if (strcmp(match.password, password) == 0) {
        user->id = match.id;
        strcpy(user->username, username);
        strcpy(user->password, password);

        return true;
    }

    return false;
}   

bool menu_user_register(struct user_t* user) {
    char username[50];
    char password[50];

    system("clear");

    printf("\n Bank Management System / Register\n");
    printf("\n Username: ");
    
    scanf("%50s", (char*) &username);

    struct termios context;
    terminal_echo(false, &context);   

    printf(" Password: ");
    scanf("%50s", (char*) &password);

    terminal_echo(true, &context);

    if (find_user_by_name(username, NULL))
        return false;

    strcpy(user->username, username);
    strcpy(user->password, password);

    user_id_t user_id;
    insert_user(*user, &user_id);

    user->id = user_id;

    return true;
}


void menu_account_create(struct user_t user)
{
    struct record_t r;

    system("clear");

    printf("\t\t\t===== New record =====\n");

    /*
    time_t now = time(NULL);
    struct tm* lnow = localtime(&now);

    r.creationDate.day   = lnow->tm_mday;
    r.creationDate.month = lnow->tm_mon + 1;
    r.creationDate.year  = lnow->tm_year + 1900;
    */

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &r.creationDate.month, &r.creationDate.day, &r.creationDate.year);

    printf("\nEnter the account number:");
    scanf("%d", &r.accountId);

    struct record_t match;
    if (find_record_by_aid(user.id, r.accountId, &match)) {
        if (strcmp(match.username, user.username) == 0) {
            printf("✖ This Account already exists for this user\n\n");
            return;
        }
    }

    printf("Enter the country:");
    scanf("%20s", r.country);
    printf("Enter the phone number:");
    scanf("%d", &r.phone);
    printf("Enter amount to deposit: $");
    scanf("%lf", &r.balance);
    
    printf("Choose the type of account: \n");
    printf(" [1] → fixed 01 (interest after one/1 year)\n");
    printf(" [2] → fixed 02 (interest after two/2 year)\n");
    printf(" [3] → fixed 03 (interest after three/3 year)\n");
    printf(" [4] → saving\n");
    printf(" [5] → current\n");
    printf("\n");
    
    const char option = immediate_getch();
    switch (option)
    {
    case '1': strcpy(r.type, "fixed01"); break;
    case '2': strcpy(r.type, "fixed02"); break;
    case '3': strcpy(r.type, "fixed03"); break;
    case '4': strcpy(r.type, "saving");  break;
    case '5': strcpy(r.type, "current"); break;
    default:
        printf("invalid account type; defaulting to 'current'.");
        strcpy(r.type, "current");
    }

    r.userId = user.id;
    strcpy(r.username, user.username);

    insert_record(r, NULL);

    struct transaction_t transaction = {
        .aid = r.accountId,
        .credit = 0,
        .debit = r.balance
    };

    insert_transaction(transaction, NULL);
}

void menu_account_update(struct user_t user)
{
    account_id_t aid;

    system("clear");

    printf("===== Update account information =====\n");
    printf("Enter account number: ");
    scanf("%d", &aid);

    if (!find_record_by_aid(user.id, aid, NULL)) {
        printf("Invalid account number.\n");
        return;
    }

    printf("\n");
    printf("What do you want to update ?\n");
    printf(" [1] Your phone number\n");
    printf(" [2] Your country\n");

    bool success = false;
    while (!success)
    {
        char option = immediate_getch();

        switch (option)
        {
        case '1':
            {
                int phone;
                printf("Phone: ");
                scanf("%d", &phone);
                update_record_phone(user.id, aid, phone);
                success = true;
                break;
            }
        case '2':
            {
                char country[10];
                printf("Country: ");
                scanf("%10s", (char*) &country);
                update_record_country(user.id, aid, country);
                success = true;
                break;
            }
        default:
            printf("Invalid option.\n");
        }
    }
}

void menu_account_check_details(struct user_t user)
{
    account_id_t aid;
    record_t record;

    system("clear");

    printf("===== Check account details =====\n");
    printf("Enter account number: ");
    scanf("%d", &aid);

    printf("\n");

    if (find_record_by_aid(user.id, aid, &record)) {
        printf("Account number:%d\nDeposit Date: %d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
            record.accountId,
            record.creationDate.month,
            record.creationDate.day,
            record.creationDate.year,
            record.country,
            record.phone,
            record.balance,
            record.type
        );
        
        printf("\n");

        if (strcmp(record.type, "current") == 0) {
            printf("You will not get interests because the account is of type current.");
        } else if (strcmp(record.type, "saving") == 0) {
            double interest = 0.07;
            double yearly_interest = (interest * record.balance);
            double montly_interest = (interest * record.balance) / 12;
            
            printf("Interest rate: %.2f%%\n", interest);
            printf("Interest gain: $%.2f monthly (the %02d), $%.2f yearly.", montly_interest, record.creationDate.day, yearly_interest);
        } else if (strcmp(record.type, "fixed01") == 0) {
            double interest = 0.04;
            double fixed_interest = (interest * record.balance) * 1;
            
            struct date_t interest_date = record.creationDate;
            interest_date.year = interest_date.year + 1;

            printf("Interest rate: %.2f%%\n", interest);
            printf("Interest gain: $%.2f on %02d/%02d/%04d.",
                fixed_interest,
                interest_date.month,
                interest_date.day,
                interest_date.year
            );
        } else if (strcmp(record.type, "fixed02") == 0) {
            double interest = 0.05;
            double fixed_interest = (interest * record.balance) * 2;

            struct date_t interest_date = record.creationDate;
            interest_date.year = interest_date.year + 2;
            
            printf("Interest rate: %.2f%%\n", interest);
            printf("Interest gain: $%.2f on %02d/%02d/%04d.",
                fixed_interest,
                interest_date.month,
                interest_date.day,
                interest_date.year
            );
        } else if (strcmp(record.type, "fixed03") == 0) {
            double interest = 0.08;
            double fixed_interest = (interest * record.balance) * 3;
            
            struct date_t interest_date = record.creationDate;
            interest_date.year = interest_date.year + 3;

            printf("Interest rate: %.2f%%\n", interest);
            printf("Interest gain: $%.2f on %02d/%02d/%04d.",
                fixed_interest,
                interest_date.month,
                interest_date.day,
                interest_date.year
            );
        }
    } else {
        printf("Invalid account number.");
    }

    printf("\n");
}

void menu_account_check_all(struct user_t user)
{
    system("clear");
    
    printf("===== All accounts from user, %s =====\n\n", user.username);

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "SELECT AccountId, CreationDate, Country, Phone, Balance, AccountType FROM Records WHERE UserId = ?", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, user.id);
    
    int index = 1;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        char date[10];
        strcpy(date, sqlite3_column_text(stmt, 3));

        printf("\e[1;33mAccount #%d\e[0m\n", index);
        printf("Account number:%d\nDeposit Date: %s \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
            sqlite3_column_int(stmt, 0),
            sqlite3_column_text(stmt, 1),
            sqlite3_column_text(stmt, 2),
            sqlite3_column_int(stmt, 3),
            sqlite3_column_double(stmt, 4),
            sqlite3_column_text(stmt, 5)
        );
        printf("\n");

        index++;
    }

    printf("Accounts in your possession → %d\n", index-1);

    sqlite3_finalize(stmt);
}

void menu_account_remove(struct user_t user)
{
    account_id_t aid;

    system("clear");

    printf("===== Delete record from user, %s =====\n", user.username);
    printf("Enter account number: ");
    scanf("%d", &aid);

    if (!find_record_by_aid(user.id, aid, NULL)) {
        printf("Invalid account number; operation aborted.\n");
        return;
    }

    printf("This operation is irreversible; continue [y] ? ");
    char choice = immediate_getch();
    if (choice == 'y') {
        delete_record_by_aid(user.id, aid);
        delete_transaction_by_aid(user.id, aid);
        printf("\nAccount has been successfully deleted.\n");
    } else printf("\nOperation aborted.\n");
}

void menu_account_transfer_owner(struct user_t user)
{
    char recipient_name[FIELD_MAX];
    account_id_t aid;

    system("clear");

    printf("===== Transfer owner from user, %s =====\n", user.username);

    printf("\nRecipient's name: ");
    scanf("%50s", (char*) &recipient_name);

    struct user_t recipient;
    if (!find_user_by_name(recipient_name, &recipient)) {
        printf("Invalid recipient; operation aborted.\n");
        return;
    }

    printf("\nYour account number: ");
    scanf("%d", &aid);

    if (!find_record_by_aid(user.id, aid, NULL)) {
        printf("Invalid account number; operation aborted.\n");
        return;
    }

    if (find_record_by_aid(recipient.id, aid, NULL)) {
        printf("Account number conflict; please contact an administrator.\n");
        return;
    }

    printf("This operation is irreversible; continue [y] ? ");
    char choice = immediate_getch();
    if (choice == 'y') {
        update_record_owner(user.id, aid, recipient.username, recipient.id);
        printf("\nAccount has been successfully transfered to user %s.\n", recipient.username);
    } else printf("\nOperation aborted.\n");
}

void menu_transaction_create(struct user_t user)
{
    account_id_t aid;
    record_t record;

    system("clear");

    printf("===== Transaction =====\n");

    printf("Enter account number: ");
    scanf("%d", &aid);

    if (!find_record_by_aid(user.id, aid, &record)) {
        printf("Invalid account number");
        return;
    }

    printf("Choose one of these operations: \n");
    printf("  [1] withdraw\n");
    printf("  [2] deposit\n");

    char option = immediate_getch();
    switch (option)
    {
        case '1':
        {
            double amount;

            printf("Amount to withdraw: $");
            scanf("%lf", &amount);

            if (amount > record.balance) {
                printf("Amount to withdraw requested is over account balance.\n");
            } else {
                struct transaction_t transaction = {
                    .aid = aid,
                    .credit = amount,
                    .debit = 0,
                    .userId = user.id
                };
                
                update_record_balance(user.id, aid, record.balance - amount);
                insert_transaction(transaction, NULL);

                printf("Withdraw successful.\n");
            }

            break;
        }
        case '2':
        {
            double amount;

            printf("Amount to deposit: $");
            scanf("%lf", &amount);

            struct transaction_t transaction = {
                .aid = aid,
                .credit = 0,
                .debit = amount,
                .userId = user.id
            };
            
            update_record_balance(user.id, aid, record.balance + amount);
            insert_transaction(transaction, NULL);

            printf("Deposit successful.\n");

            break;
        }
        default:
        {
            printf("Invalid option\n");
        }
    }
}

bool ask_continuation(const char* message)
{
    printf("%s: ", message);
    const char option = immediate_getch();
    if (option == '1') return true;
    return false;
}