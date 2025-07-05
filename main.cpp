int current_date = 0; // Global simulated date variable

#include <iostream>
using namespace std;

void incrementDate() {
    current_date++;
    cout << "Date incremented! Current simulated date: Day " << current_date << ".\n";
}

#include "library.h"
#include "cli/home.h"  // Ensure this is included

void displayHome(Library &lib) {
    int choice;
    do {
        cout << "\nLibrary Management System\n";
        cout << "1. Borrow Book\n";
        cout << "2. Return Book\n";
        cout << "3. Pay Fine\n";
        cout << "4. Add Book (Librarian)\n";
        cout << "5. Remove Book (Librarian)\n";
        cout << "6. Exit\n";
        cout << "7. Increment Simulated Date\n";
        cout << "8. View Borrowing History\n";
        cout<<"9. Add User (Librarian)\n10. Remove User (Librarian)\n";
        cout << "Enter choice: ";
        cin >> choice;

        int userId;
        cout << "Enter your User ID: ";
        cin >> userId;

        if (lib.accounts.find(userId) == lib.accounts.end()) {
            cout << "Invalid User ID!\n";
            continue;
        }

        Account &acc = lib.accounts[userId];

        // Get the corresponding User object
        User* user = lib.getUserById(userId);
        if (!user) {
            cout << "Error: User not found!\n";
            continue;
        }

        switch (choice) {
            case 1: {
                    if (acc.getUserType() == "librarian") {
                        cout << "Librarians cannot borrow books.\n";
                        break;
                    }
                
                    if (acc.getUserType() == "student" && acc.getFineDue() > 0) {
                        cout << "You have unpaid fines of ₹" << acc.getFineDue() << ". Please pay fine first.\n";
                        break;
                    }
                
                    int max_books = (acc.getUserType() == "student") ? Student::MAX_BORROWABLE_BOOKS : Faculty::MAX_BOOKS;
                    if (acc.getBorrowedBooks().size() >= max_books) {
                        cout << "You have reached your borrowing limit (" << max_books << " books).\n";
                        break;
                    }
                
                    cout << "Enter ISBN of book you want to borrow: ";
                    string isbn; cin >> isbn;

                    if(lib.books.find(isbn)==lib.books.end()){
                        cout<<"Invalid ISBN entered!\n"; return;
                    }
                
                    auto bookIt = lib.books.find(isbn);
                    if (bookIt == lib.books.end()) {
                        cout << "Book not found!\n";
                        break;
                    }
                    
                    Book &book = bookIt->second;
                
                    if (book.status != "available") {
                        cout << "Book is currently not available.\n";
                        break;
                    }
                
                    int today_date = 0;
                    bool success = user->borrowBook(book);
                
                    if(success) {
                        acc.addBorrowedBook(isbn);
                        book.borrow_date = today_date;
                        book.due_date = today_date + ((acc.getUserType() == "student") ? Student::MAX_DAYS : Faculty::MAX_DAYS);
                        cout << "Book borrowed successfully! Due date: Day " << book.due_date << ".\n";
                    } else {
                        cout << "Failed to borrow book.\n";
                    }
                }
                break;
            case 2:
                { // Return Book
                    if (acc.getUserType() == "librarian") {
                        cout << "Librarians cannot return books as they don't borrow.\n";
                        break;
                    }

                    cout << "Enter ISBN of book you want to return: ";
                    string isbn; cin >> isbn;

                    auto bookIt = lib.books.find(isbn);
                    if (bookIt == lib.books.end()) {
                        cout << "Book not found in library database!\n";
                        break;
                    }

                    Book &book = bookIt->second;

                    if (book.borrower_id != userId) {
                        cout << "You haven't borrowed this book!\n";
                        break;
                    }

                    int today_date = 0;

                    user->returnBook(book, today_date);

                    // Remove book from user's borrowed_books_isbn list
                    acc.removeBorrowedBook(isbn);

                    // Check if fine is applicable (for students)
                    if (acc.getUserType() == "student") {
                        int overdue_days = today_date - book.due_date;
                        if (overdue_days > 0) {
                            int fine_amount = overdue_days * Student::FINE_PER_DAY;
                            acc.addFine(fine_amount);
                            cout << "Book returned late! Fine of ₹" << fine_amount << " added to your account.\n";
                        } else {
                            cout << "Book returned successfully. No fines.\n";
                        }
                    } else if (acc.getUserType() == "faculty") {
                        int overdue_days = today_date - book.due_date;
                        if(overdue_days > 60){
                            cout<<"Warning: Returned overdue by more than 60 days!\n";
                        } else {
                            cout<<"Book returned successfully.\n";
                        }
                    }

                }
                break;
            case 3:
                acc.payFine();
                break;
            case 4:
                if (acc.getUserType() == "librarian") {
                    Librarian* librarian = dynamic_cast<Librarian*>(user);
                    if (librarian) {
                        librarian->addBook(lib);
                    } else {
                        cout << "Error: Librarian object not found!\n";
                    }
                } else {
                    cout << "Permission denied! Only librarians can add books.\n";
                }
                break;
            case 5:
                if (acc.getUserType() == "librarian") {
                    Librarian* librarian = dynamic_cast<Librarian*>(user);
                    if (librarian) {
                        librarian->removeBook(lib);
                    } else {
                        cout << "Error: Librarian object not found!\n";
                    }
                } else {
                    cout << "Permission denied! Only librarians can remove books.\n";
                }
                break;
            case 6:
                cout << "Exiting...\n";
                return;
            case 7:
                incrementDate();
                break;
            case 8: {
                vector<string> history = acc.getBorrowHistory();
                if (history.empty()) {
                    cout << "No borrowing history.\n";
                } else {
                    cout << "Borrowing History:\n";
                    for (const string& isbn : history) {
                        if(lib.books.find(isbn) != lib.books.end()){
                            cout << "- " << lib.books[isbn].title << " (ISBN: " << isbn << ")\n";
                        } else {
                            cout << "- ISBN: " << isbn << "\n";
                        }
                    }
                }
            }
            break;
            case 9:
                if(acc.getUserType()=="librarian"){
                    Librarian* librarian=dynamic_cast<Librarian*>(user);
                    librarian->addUser(lib);
                }else{
                    cout<<"Permission denied! Only librarians can add users.\n";
                }
            break;

            case 10:
                if(acc.getUserType()=="librarian"){
                    Librarian* librarian=dynamic_cast<Librarian*>(user);
                    librarian->removeUser(lib);
                }else{
                    cout<<"Permission denied! Only librarians can remove users.\n";
                }
            break;

            default:
                cout << "Invalid choice!\n";
        }
    } while (true);
}


int main() {
    Library lib;

    lib.loadBooks("data/books.json");
    lib.loadAccounts("data/accounts.json");

    displayHome(lib);

    lib.saveBooks("data/books.json");
    lib.saveAccounts("data/accounts.json");

    return 0;
}