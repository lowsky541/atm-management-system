#if !defined(__MENU_H__)
#define __MENU_H__

#include <time.h>

#include "helper.h"
#include "database.h"

bool menu_user_login(struct user_t* user);
bool menu_user_register(struct user_t* user);

void menu_account_create(struct user_t user);
void menu_account_update(struct user_t user);
void menu_account_check_details(struct user_t user);
void menu_account_check_all(struct user_t user);
void menu_account_remove(struct user_t user);
void menu_account_transfer_owner(struct user_t user);

void menu_transaction_create(struct user_t user);

bool ask_continuation(const char* message);

#endif // __MENU_H__
