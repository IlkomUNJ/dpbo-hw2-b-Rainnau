#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "bank.h"

using namespace std;
using std::string;


struct Order {
    string name;
    double price;
    int amount;
};

class Account: public Bank{
    private:
        string username;
        string password;
        int type;
        static vector<Account> accounts;
        map<string, Order> orders;

    public:
        void accountRegister(string x, string y, int i) {
            username = x;
            password = y;
            setUserType(i);

            accounts.push_back(*this);
        }

        string getUsername() {return username;}
        string getPassword() {return password;}
        int getUserType() {return type;}
        void setUserType(int i) {type = i;}
        void addOrder(string x, double y, int z) {
            Order o;
            o.name = x;
            o.price = y;
            o.amount = z;
            orders[x] = o;
        }
        void displayOrder() {
            for (auto pair : orders) {
                cout << "|| \"" << pair.first << "\": $" << pair.second.price << " (" << pair.second.amount << ")" << endl;
            }
        }
        static vector<Account> getAccounts() {return accounts;}

        void serialize(const string& filename) const {
            ofstream file(filename);
            if (!file.is_open()) {
                cerr << "Error: Failed to open file for writing." << endl;
                return;}

            file << username << '\n' << password << '\n' << type << '\n';
            file << orders.size() << '\n';

            for (const auto& kv : orders) {
                const Order& o = kv.second;
                file << o.name.size() << '\n'
                     << o.name << '\n'
                     << o.price << ' ' << o.amount << '\n';}

            file.close();
        }

        static Account deserialize(const string& filename) {
            Account obj;
            ifstream file(filename);
            if (!file.is_open()) {
                cerr << "Error: Failed to open file for reading." << endl;
                obj.accountRegister("", "", 0);
                return obj;}

            string line;

            if (!getline(file, obj.username)) {obj.accountRegister("", "", 0); return obj;}
            if (!getline(file, obj.password)) {obj.accountRegister("", "", 0); return obj;}
            if (!getline(file, line)) {obj.accountRegister("", "", 0); return obj;}
            obj.type = stoi(line);

            if (!getline(file, line)) {accounts.push_back(obj); return obj;}
            size_t ordersCount = stoul(line);

            for (size_t i = 0; i < ordersCount; ++i) {                
                if (!getline(file, line)) {break;}
                size_t nameLen = stoul(line);
                string name;
                if (!getline(file, name)) {break;}
                (void)nameLen;

                if (!getline(file, line)) {break;}
                size_t pos = line.find(' ');
                double price = 0.0;
                int amount = 0;
                if (pos != string::npos) {
                    price = stod(line.substr(0, pos));
                    amount = stoi(line.substr(pos + 1));
                } else {price = stod(line);}

                obj.addOrder(name, price, amount);
            }

            accounts.push_back(obj);
            file.close();
            return obj;
        }
};