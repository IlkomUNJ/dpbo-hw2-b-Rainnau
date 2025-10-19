#pragma once

#include <string>
#include <map>
#include <vector>
#include <iostream>

#include "account.h"

using namespace std;


struct Product {
    string name;
    double price = 0.0;
    int stock = 0;
};

class Store {
    private:
        Account storeOwner;
        map<string, Product> items;
        double revenue = 0.0;

        static inline map<string, Store> stores;

    public:
        void makeStore(Account x) {
            storeOwner = x;

            stores[x.getUsername()] = *this;
        }

        void addProduct(const string& name, double price, int stock) {
            Product p;
            p.name = name;
            p.price = price;
            p.stock = stock;
            items[name] = p;
        }

        bool removeProduct(string name) {
            return items.erase(name) > 0;
        }

        bool changePrice(string name, double newPrice) {
            Product p = items[name];
            p.price = newPrice;
            items[name] = p;
            return true;
        }

        bool addStock(string name, int amount) {
            Product p = items[name];
            p.stock += amount;
            items[name] = p;
            return true;
        }

        bool minStock(string name, int amount) {
            Product p = items[name];
            p.stock += amount;
            items[name] = p;
            return true;
        }

        static map<string, Store> getStores() {return stores;}

        map<string, Product> getInventory() {
            return items;
        }

        void displayInv() const {
            if (items.empty()) {
                return;
            }
            for (const auto &pair : items) {
                cout << "|| \"" << pair.first << "\": $" << pair.second.price << " (stock: " << pair.second.stock << ")" << endl;
            }
        }

        Account getOwner() {return storeOwner;}
};