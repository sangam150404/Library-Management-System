#ifndef LOGIN_STUD_FAC_H
#define LOGIN_STUD_FAC_H

#include "../library.h"

void loginStudFac(int id, Library &lib) {
    auto it = lib.accounts.find(id);
    if (it == lib.accounts.end()) {
        cout << "Invalid ID.\n"; 
        return;
    }

    Account &account = it->second;
    cout << "Logged in as " << account.user_type << ".\n";

    cout << "1. Borrow Book\n2. Return Book\nChoice: ";
    int choice;
    cin >> choice;

    if (choice == 1) {
        string isbn;
        cout << "Enter ISBN: ";
        cin >> isbn;
        
        auto bookIt = lib.books.find(isbn);
        if (bookIt == lib.books.end()) {
            cout << "Book not found!\n";
            return;
        }
        
        borrowBook(nullptr, bookIt->second, 1);
    } else if (choice == 2) {
        string isbn;
        cout << "Enter ISBN: ";
        cin >> isbn;
        
        auto bookIt = lib.books.find(isbn);
        if (bookIt == lib.books.end()) {
            cout << "Book not found!\n";
            return;
        }
        
        returnBook(nullptr, bookIt->second, 1);
    }
}

#endif