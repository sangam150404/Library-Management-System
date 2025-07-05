#include "users.h"
#include "library.h"
#include <algorithm>

bool Student::borrowBook(Book& book) {
    if (fine_due > 0) {
        cout << "Cannot borrow books until fine of ₹" << fine_due << " is paid.\n";
        return false;
    }
    if (borrowed_books_isbn.size() >= MAX_BORROWABLE_BOOKS) {
        cout << "Cannot borrow more than " << MAX_BORROWABLE_BOOKS << " books.\n";
        return false;
    }
    if (book.status != "available") {
        cout << "Book is not available.\n";
        return false;
    }

    borrowed_books_isbn.push_back(book.isbn);

    book.status = "borrowed";
    book.borrower_id = id;
    book.due_date = 0 + MAX_DAYS;  // Placeholder for today's date

    return true;
}

void Student::returnBook(Book& book, int today_date) {
    auto it = find(borrowed_books_isbn.begin(), borrowed_books_isbn.end(), book.isbn);
    if (it != borrowed_books_isbn.end()) {
        borrowed_books_isbn.erase(it);
        borrow_history.push_back(book.isbn);
        
        int overdue_days = today_date - book.due_date;
        if (overdue_days > 0) {
            fine_due += overdue_days * FINE_PER_DAY;
            cout << "Book returned late! Fine of ₹" << (overdue_days * FINE_PER_DAY) << " applied.\n";
        }

        book.status = "available";
        book.borrower_id = -1;
    } else {
        cout << "Error: You have not borrowed this book.\n";
    }
}


bool Faculty::borrowBook(Book &book) {
    if (borrowed_books_isbn.size() >= MAX_BOOKS) {
        cout << "Cannot borrow more than " << MAX_BOOKS << " books.\n";
        return false;
    }
    if (book.status != "available") {
        cout << "Book is not available.\n";
        return false;
    }

    borrowed_books_isbn.push_back(book.isbn);

    book.status = "borrowed";
    book.borrower_id = id;
    book.borrow_date = 0; // Assume 0 is today's date
    book.due_date = book.borrow_date + MAX_DAYS;

    return true;
}

void Faculty::returnBook(Book& book, int today_date) {
    auto it = find(borrowed_books_isbn.begin(), borrowed_books_isbn.end(), book.isbn);
    if (it != borrowed_books_isbn.end()) {
        borrowed_books_isbn.erase(it);
        borrow_history.push_back(book.isbn);

        int overdue_days = today_date - book.due_date;
        if (overdue_days > 60) {
            cout << "Warning: Faculty has returned a book overdue by more than 60 days!\n";
        }

        book.status = "available";
        book.borrower_id = -1;
    } else {
        cout << "Error: You have not borrowed this book.\n";
    }
}

bool borrowBook(User* user, Book &book, int today_date) {
    if (book.status != "available") return false;
    book.status = "borrowed";
    book.borrower_id = user->getId();
    book.borrow_date = today_date;
    
    if (dynamic_cast<Student*>(user)) {
        book.due_date = today_date + Student::MAX_DAYS;
    } else if (dynamic_cast<Faculty*>(user)) {
        book.due_date = today_date + Faculty::MAX_DAYS;
    }

    return true;
}

void returnBook(User* user, Book &book, int today_date) {
    book.status = "available";
    book.borrower_id = -1;
    book.borrow_date = -1;
    book.due_date = -1;
}

void Librarian::addBook(Library &lib) {
    string title, author, publisher, isbn;
    int year;
    
    cout << "Enter Book Details - \n";
    cout << "Title: "; cin.ignore(); getline(cin, title);
    cout << "Author: "; getline(cin, author);
    cout << "Publisher: "; getline(cin, publisher);
    cout << "ISBN: "; cin >> isbn;
    cout << "Year: "; cin >> year;

    Book newBook(title, author, publisher, isbn, year);
    lib.books[isbn] = newBook;
    cout << "Book added successfully!\n";
}

void Librarian::removeBook(Library &lib) {
    string isbn;
    cout << "Enter ISBN of the book to remove: ";
    cin >> isbn;

    if (lib.books.find(isbn) != lib.books.end()) {
        lib.books.erase(isbn);
        cout << "Book removed successfully!\n";
    } else {
        cout << "Error: Book not found.\n";
    }
}

void Account::removeBorrowedBook(const string& isbn) {
    auto it = find(borrowed_books_isbn.begin(), borrowed_books_isbn.end(), isbn);
    if (it != borrowed_books_isbn.end()) {
        borrow_history.push_back(isbn);  // Add to history before removing
        borrowed_books_isbn.erase(it);
    }
}

void Account::payFine() {
    if (fine_due == 0) {
        cout << "No pending fine to pay.\n";
        return;
    }

    cout << "Fine of Rs. " << fine_due << " has been paid.\n";
    fine_due = 0;
}

void Librarian::addUser(Library &lib) {
    string name, type;
    int id;
    cout << "Enter User ID: "; cin >> id;
    if(lib.accounts.find(id) != lib.accounts.end()){
        cout<<"User ID already exists!\n";
        return;
    }
    cout << "Enter Name: "; cin.ignore(); getline(cin, name);
    cout << "Enter Type (student/faculty): "; cin >> type;

    if (type == "student") {
        lib.users[id] = new Student(name, id, "password");
        lib.accounts[id] = Account(id, type);
        cout << "Student added successfully!\n";
    } else if (type == "faculty") {
        lib.users[id] = new Faculty(name, id, "password");
        lib.accounts[id] = Account(id, type);
        cout << "Faculty added successfully!\n";
    } else {
        cout << "Invalid user type!\n";
    }
}

void Librarian::removeUser(Library &lib) {
    int id;
    cout << "Enter User ID to remove: "; cin >> id;

    if (lib.users.find(id) != lib.users.end()) {
        delete lib.users[id];
        lib.users.erase(id);
        lib.accounts.erase(id);
        cout << "User removed successfully!\n";
    } else {
        cout << "User not found!\n";
    }
}
