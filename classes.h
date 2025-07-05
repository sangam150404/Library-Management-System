#ifndef CLASSES_H
#define CLASSES_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class Library;

using namespace std;

class Book {
public:
    string title, author, publisher, isbn;
    int year;
    string status; // "available", "borrowed", "reserved"
    int borrower_id;
    int borrow_date;
    int due_date;

    Book() : borrower_id(-1), borrow_date(-1), due_date(-1), status("available") {} // Default constructor
    Book(string t, string a, string p, string i, int y) : title(t), author(a), publisher(p), isbn(i), year(y),
        borrower_id(-1), borrow_date(-1), due_date(-1), status("available") {}
};

// Base class
class User {
protected:
    string name;
    int id;
    string password;

public:
    User(string n, int i, string pw) : name(n), id(i), password(pw) {} // Constructor

    virtual bool borrowBook(Book&) = 0; // Pure virtual function
    virtual void returnBook(Book&, int) = 0; // Pure virtual function

    string getName() { return name; }
    int getId() { return id; }
};

// Derived class: Student
class Student : public User {
public:
    vector<string> borrowed_books_isbn;
    vector<string> borrow_history;
    int fine_due = 0;
    static const int MAX_BORROWABLE_BOOKS = 3;
    static const int MAX_DAYS = 15;
    static const int FINE_PER_DAY = 10;

    Student(string n, int i, string pw) : User(n, i, pw) {}

    bool borrowBook(Book& b) override;
    void returnBook(Book& b, int today_date) override;
};

// Derived class: Faculty
class Faculty : public User {
public:
    vector<string> borrowed_books_isbn;
    vector<string> borrow_history;
    int fine_due = 0;
    static const int MAX_BOOKS = 5;
    static const int MAX_DAYS = 30;

    Faculty(string n, int i, string pw) : User(n, i, pw) {}

    bool borrowBook(Book& b) override;
    void returnBook(Book& b, int today_date) override;
};

// Derived class: Librarian
class Librarian : public User {
public:
    Librarian(string n, int i, string pw) : User(n, i, pw) {}

    void addBook(Library &lib);
    void removeBook(Library &lib);

    // Add these declarations
    void addUser(Library &lib);      
    void removeUser(Library &lib);    

    bool borrowBook(Book&) override { return false; }  // Librarians don’t borrow books
    void returnBook(Book&, int today_date) override {}  // Librarians don’t return books
};

// Account Class with additional fields and methods
class Account {
    public:
        int id;
        string user_type; // student, faculty, librarian
        vector<string> borrowed_books_isbn;
        vector<string> borrow_history;
        int fine_due;
        Account() : id(-1), user_type("unknown"), fine_due(0) {}

        Account(int acc_id, string type) : id(acc_id), user_type(type), fine_due(0) {}
    
        // Getters
        int getId() const { return id; }
        string getUserType() const { return user_type; }
        int getFineDue() const { return fine_due; }
        vector<string> getBorrowedBooks() const { return borrowed_books_isbn; }
        vector<string> getBorrowHistory() const { return borrow_history; }
    
        // Setters and Modifiers
        void addBorrowedBook(const string& isbn) { borrowed_books_isbn.push_back(isbn); }
        void removeBorrowedBook(const string& isbn);
        void addFine(int amount) { fine_due += amount; }
        void payFine();    
        void addToHistory(const string& isbn) { borrow_history.push_back(isbn); }
    };
    
#endif // CLASSES_H