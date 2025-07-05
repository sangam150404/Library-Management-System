#ifndef LIBRARY_H
#define LIBRARY_H

#include "classes.h"
#include "users.h"
#include "json.hpp"
#include <unordered_map>
#include <string>

using namespace std;  // Ensure `string` and `unordered_map` are recognized

#include <fstream>

using json = nlohmann::json;

class Library {
public:
    unordered_map<string, Book> books;
    unordered_map<int, Account> accounts;
    unordered_map<int, User*> users;

    User* getUserById(int userId);

    void loadBooks(const string &filepath);
    void saveBooks(const string &filepath);
    void loadAccounts(const string &filepath);
    void saveAccounts(const string &filepath);
};

#endif