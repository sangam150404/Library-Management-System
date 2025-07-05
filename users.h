#ifndef USERS_H
#define USERS_H

#include "classes.h"
#include "library.h"
#include <iostream>

using namespace std;

// Function prototypes for borrowing and returning books
bool borrowBook(User* user, Book &book, int today_date);
void returnBook(User* user, Book &book, int today_date);

#endif // USERS_H