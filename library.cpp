#include "library.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
using namespace std;

void Library::loadBooks(const string &filepath) {
    ifstream file(filepath);
    if (!file) {
        cerr << "[ERROR] Could not open books file!" << endl;
        return;
    }

    json j;
    file >> j;

    for (auto& el : j.items()) {
        auto b = el.value();

        // Check required fields clearly
        if (!b.contains("title") || !b.contains("author") || !b.contains("publisher") ||
            !b.contains("isbn") || !b.contains("year") || !b.contains("status")) {
            cerr << "[ERROR] Missing fields in books.json for ISBN: "<< el.key() << endl;
            continue; // Skip invalid entries
        }

        Book book(b["title"], b["author"], b["publisher"], b["isbn"], b["year"]);
        
        book.status = b["status"];
        book.borrower_id = b.value("borrower_id", -1);
        book.borrow_date = b.value("borrow_date", -1);
        book.due_date = b.value("due_date", -1);

        books[el.key()] = book;
    }
}


void Library::saveBooks(const string &filepath) {
    json j;

    for (auto& book_pair : books) {
        const string& isbn = book_pair.first;
        Book& b = book_pair.second;

        j[isbn] = {{"title", b.title}, {"author", b.author}, {"publisher", b.publisher},
                   {"isbn", b.isbn}, {"year", b.year}, {"status", b.status},
                   {"borrower_id", b.borrower_id}, {"borrow_date", b.borrow_date}, {"due_date", b.due_date}};
    }

    ofstream file(filepath);
    if (!file) {
        cerr << "Error: Could not open books file for writing!" << endl;
        return;
    }
    file << setw(4) << j << endl;
}

void Library::loadAccounts(const string &filepath) {
    ifstream file(filepath);
    if (!file) {
        cerr << "Error: Could not open accounts file!" << endl;
        return;
    }

    json j;
    file >> j;

    for (auto& el : j.items()) {
        int id = stoi(el.key());
        auto a = el.value();

        // Check that required fields exist and are correct types
        if (!a.contains("name") || !a["name"].is_string() ||
            !a.contains("type") || !a["type"].is_string() ||
            !a.contains("fine_due") || !a["fine_due"].is_number_integer() ||
            !a.contains("borrowed_books_isbn") || !a["borrowed_books_isbn"].is_array()) {
            cerr << "[ERROR] Account ID: " << id << ". Missing or incorrect fields in accounts.json.\n";
            continue; // Skip this account entry
        }

        string name = a["name"];
        string type = a["type"];
        
        // Create user based on type
        User* u = nullptr;
        if (type == "student")
            u = new Student(name, id, "password");
        else if (type == "faculty")
            u = new Faculty(name, id, "password");
        else if (type == "librarian")
            u = new Librarian(name, id, "password");
        else {
            cerr << "[ERROR] Invalid user type for ID: "<< id << endl;
            continue;
        }

        users[id] = u;

        Account acc(id, type);
        acc.fine_due = a["fine_due"];
        acc.borrowed_books_isbn.clear();
        for(auto &isbn : a["borrowed_books_isbn"])
            acc.borrowed_books_isbn.push_back(isbn.get<string>());

        accounts[id] = acc;
    }
}

void Library::saveAccounts(const string &filepath) {
    json j;
    for (auto& account_pair : accounts) {
        const int id = account_pair.first;
        Account& acc = account_pair.second;

        j[to_string(id)] = {
            {"type", acc.getUserType()},
            {"fine_due", acc.getFineDue()},
            {"borrowed_books", acc.getBorrowedBooks()},
            {"borrow_history", acc.getBorrowHistory()}
        };
    }

    ofstream file(filepath);
    if (!file) {
        cerr << "Error: Could not open accounts file for writing!" << endl;
        return;
    }
    file << setw(4) << j << endl;
}

User* Library::getUserById(int userId) {
    if (users.find(userId) != users.end()) {
        return users[userId];  //  Return User object
    }
    return nullptr;  //  User not found
}
