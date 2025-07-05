#ifndef LOGIN_LIB_H
#define LOGIN_LIB_H

#include "../library.h"

void loginLib(int id, Library &lib) {
    if(lib.accounts[id].user_type != "librarian") {
        cout<<"Invalid ID.\n"; return;
    }
    
    Librarian libUser("Librarian", id, "default_passwprd");
    
    cout<<"Logged in as Librarian.\n";
    cout<<"1.Add Book\n2.Remove Book\nChoice: ";
    
    int choice; cin>>choice;
    
    if(choice==1){
        Book b; cout<<"ISBN:"; cin>>b.isbn; cout<<"Title:"; cin>>b.title;
        lib.books[b.isbn]=b; cout<<"Added.\n";
    }
    
    else if(choice==2){
        string isbn; cout<<"ISBN to remove:"; cin>>isbn;
        lib.books.erase(isbn); cout<<"Removed.\n";
    }
}

#endif