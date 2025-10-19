#pragma once

#include <iostream>
#include <ctime>
#include <vector>
#include <string>

using std::string;
using namespace std;


class Bank {
    private:
        time_t timestamp = time(NULL);
        float balance;

        vector<vector<int>> cashflow;
        struct tm datetime = *localtime(&timestamp);

        
        public:
        Bank() {
            balance = 0;
        }
        
        float getBalance() {return balance;}
        
        void addBalance(int x) {
            balance += x;
            cashflow.push_back({datetime.tm_mday, datetime.tm_mon, x});
        }
        
        void minBalance(int x) {
            if (balance < x) {cout << "|| Not enough balance\t~" << endl;} else {
                balance -= x;
                cashflow.push_back({datetime.tm_mday, datetime.tm_mon, -x});
            }
        }
        
        void getCashflow() {
            for (int i = 0; i < cashflow.size(); i++) {
                cout << "|| (" << cashflow[i][0] << "/" << cashflow[i][1] << ") $" << cashflow[i][2] << endl;
            }
        }
    };
