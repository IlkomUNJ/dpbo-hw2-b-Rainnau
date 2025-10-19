#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

#include "account.h"
#include "bank.h"
#include "store.h"

using namespace std;


vector<Account> Account::accounts;

enum User_Type {BUYER, SELLER};
enum Regist_Interface {R_MENU, REGISTER, LOGIN, R_EXIT};
enum Menu_Interface {MAIN_MENU, STORE, BANK, LOGOUT, EXIT};
enum Bank_Interface {TOPUP, WITHDRAW, CASHFLOW, CUSTOMERS, BACK};
enum Store_Interface {BUY, STORE_BACK, MY_STORE};
enum Seller_Interface {ADD, STOCKUP, SETPRICE, REMOVE, S_BACK};


int main() {
    Account SellerAccount;
    Account BuyerAccount;

    Account currentUser;
    Store userStore;

    User_Type LoggedInAs;
    Regist_Interface REGIST = R_MENU;
    Menu_Interface MENU = EXIT;
    Bank_Interface BANK_MENU;
    Store_Interface STORE_MENU;
    Seller_Interface SELLER_MENU;

    string usn;
    string pass;
    int user_choice;

    while (1) {
        if ((REGIST == R_EXIT) & (MENU == EXIT)) {break;}
        while (REGIST != R_EXIT) {
            switch (REGIST) {
            case R_MENU:
                cout << "|| 1) Register" << endl;
                cout << "|| 2) Login" << endl;
                cout << "|| 3) Exit" << endl;
                cout << "|| > ";
                cin >> user_choice;
                REGIST = Regist_Interface(user_choice);
                cin.ignore(INT_MAX, '\n');
                break;
    
            case REGISTER:
                cout << "|| REGISTER\t~" << endl;
                cout << "|| USERNAME\t> "; getline(cin, usn);
                cout << "|| PASSWORD\t> "; getline(cin, pass);
                int i;
                cout << "|| AS BUYER(0)/SELLER(1)\t> "; cin >> i;
                cout << i << endl;
                cin.ignore(INT_MAX, '\n');
                if ((i == 0) || (i == 1)) {
                    currentUser.accountRegister(usn, pass, i);
                    if (i == 1) {userStore.makeStore(currentUser);}
                    REGIST = LOGIN;
                    currentUser.serialize(currentUser.getUsername());
                    break;}
                break;
    
            case LOGIN:
                cout << "|| LOGIN\t~" << endl;
                cout << "|| USERNAME\t> "; getline(cin, usn);
                cout << "|| PASSWORD\t> "; getline(cin, pass);

                currentUser = Account::deserialize(usn);
    
                for (int i = 0; i < Account::getAccounts().size(); i++) {
                    if ((usn == Account::getAccounts()[i].getUsername()) & (pass == Account::getAccounts()[i].getPassword())) {
                        string name = Account::getAccounts()[i].getUsername();
                        LoggedInAs = User_Type(currentUser.getUserType());
                        MENU = MAIN_MENU;
                        REGIST = R_EXIT; 
                        break;}}
                break;
            
            case EXIT:
                REGIST = R_EXIT;
                break;
    
            default:
                break;
            }
        }

        while (MENU != EXIT) {
            switch (MENU) {
            case MAIN_MENU: {
                // for (int i = 0; i < Account::getAccounts().size(); i++) {cout << Account::getAccounts()[i].getUsername();}

                cout << "|| WELCOME, " << currentUser.getUsername() << endl;
                cout << "|| 1) STORE" << endl;
                cout << "|| 2) BANK" << endl;
                cout << "|| 3) LOGOUT" << endl;
                cout << "|| 4) EXIT" << endl;
                cout << "|| > ";

                cin >> user_choice;
                MENU = Menu_Interface(user_choice);
                cin.ignore(INT_MAX, '\n');

                break;}
            
            case BANK: {
                cout << "|| BANK\t~" << endl;
                cout << "|| YOUR BALANCE: $"; cout << fixed << setprecision(2) << currentUser.getBalance() << endl;
                cout << "|| 1) Top up" << endl;
                cout << "|| 2) Withdraw" << endl;
                cout << "|| 3) Cashflow" << endl;
                cout << "|| 4) Customers" << endl;
                cout << "|| 5) Back" << endl;
                cout << "|| > ";

                cin >> user_choice;
                BANK_MENU = Bank_Interface(user_choice-1);
                cin.ignore(INT_MAX, '\n');

                switch (BANK_MENU) {
                case TOPUP:
                    cout << "|| How much?\t~" << endl;
                    cout << "|| > ";
                    cin >> user_choice;
                    currentUser.addBalance(user_choice);
                    cin.ignore(INT_MAX, '\n');
                    break;
                    
                case WITHDRAW:
                    cout << "|| How much?\t~" << endl;
                    cout << "|| > ";
                    cin >> user_choice;
                    currentUser.minBalance(user_choice);
                    cin.ignore(INT_MAX, '\n');
                    break;

                case CASHFLOW:
                    cout << "CASHFLOW" << endl;
                    currentUser.getCashflow();
                    break;
                
                case CUSTOMERS:
                    cout << "|| Our Customers:";
                    for (Account acc : Account::getAccounts()) {
                        cout << " || " << acc.getUsername();}
                    cout << endl;
                    break;

                case BACK:
                    MENU = MAIN_MENU;
                    break;

                default:
                    break;}
                break;}
            
            case LOGOUT: {
                REGIST = R_MENU;
                MENU = EXIT;
                break;}

            case STORE: {
                cout << "|| STORE\t~" << endl;
                cout << "|| 1) Buy Items" << endl;
                cout << "|| 2) My Orders" << endl;
                cout << "|| 3) Back" << endl;
                if (currentUser.getUserType() == SELLER) {cout << "|| 4) My Store" << endl;}
                cout << "|| > ";
                
                cin >> user_choice;
                if (currentUser.getUserType() == BUYER) {user_choice = user_choice % 4;}
                STORE_MENU = Store_Interface(user_choice-1);
                cin.ignore(INT_MAX, '\n');

                switch (STORE_MENU) {
                case BUY: {
                    string productName;
                    int productAmount;
                    cout << "|| Items" << endl;
                    if (Store::getStores().empty()) {cout << "|| Store is empty." << endl; break;}
                    else {
                        for (auto i : Store::getStores()) {
                            userStore = i.second;
                            userStore.displayInv();
                        }
                        cout << "|| Name of product to buy\t> "; cin >> productName;
                        cout << "|| Amount of product to buy\t> "; cin >> productAmount;
                        cin.ignore(INT_MAX, '\n');
                        for (auto i : Store::getStores()) {
                            userStore = i.second;
                            map<string, Product> items = userStore.getInventory();
                            for (const auto &pair : items) {
                                if ((productName == pair.first) & (currentUser.getBalance() <= (pair.second.price*productAmount))) {
                                    userStore.minStock(productName, productAmount);
                                    userStore.getOwner().addBalance(pair.second.price*productAmount);
                                    userStore.makeStore(userStore.getOwner());
                                    currentUser.minBalance(pair.second.price*productAmount);
                                    currentUser.addOrder(pair.first, pair.second.price, productAmount);
                                }
                            }
                        }
                    }
                    break;}

                case STORE_BACK:
                    MENU = MAIN_MENU;  
                    break;

                case MY_STORE: {
                    string name;
                    double price;
                    int stock;
                    userStore = Store::getStores()[currentUser.getUsername()];
                    cout << "|| Your Store\t~" << endl;
                    cout << "|| 1) Add Item" << endl;
                    cout << "|| 2) Add Stock" << endl;
                    cout << "|| 3) Set Price" << endl;
                    cout << "|| 4) Remove Price" << endl;
                    cout << "|| 5) Back" << endl;
                    cout << "|| > ";
                    cin >> user_choice;
                    SELLER_MENU = Seller_Interface(user_choice-1);
                    cin.ignore(INT_MAX, '\n');

                    switch (SELLER_MENU) {
                    case ADD:
                        cout << "|| Add Product\t\t~" << endl;
                        cout << "|| Product Name\t\t> "; cin >> name;
                        cout << "|| Product Price\t> "; cin >> price;
                        cout << "|| Product Stock\t> "; cin >> stock;
                        userStore.addProduct(name, price, stock);
                        userStore.makeStore(currentUser);
                        cout << "|| Added Product: " << name << " | $" << price << " | " << stock << endl;
                        cin.ignore(INT_MAX, '\n');
                        break;

                    case STOCKUP:
                        cout << "|| Add Stock\t\t~" << endl;
                        cout << "|| Product Name\t\t> "; cin >> name;
                        cout << "|| Product Stock\t> "; cin >> stock;
                        userStore.addStock(name, stock);
                        userStore.makeStore(currentUser);
                        cout << "|| Added " << stock << " stock(s) to " << name << endl;
                        break;

                    case SETPRICE:
                        cout << "|| Change Price\t\t~" << endl;
                        cout << "|| Product Name\t\t> "; cin >> name;
                        cout << "|| Product Price\t> "; cin >> price;
                        userStore.changePrice(name, price);
                        userStore.makeStore(currentUser);
                        cout << "|| Changed price of " << name << " to $" << price << endl;
                        break;

                    case REMOVE:
                        cout << "|| Remove Product\t~" << endl;
                        cout << "|| Product Name\t> "; cin >> name;
                        userStore.removeProduct(name);
                        userStore.makeStore(currentUser);
                        cout << "|| Removed product: " << name << endl;
                        break;

                    case S_BACK:
                        break;
                    
                    default:
                        break;}
                    break;}
                
                default:
                    break;
                }
                break;}

            default:
                MENU = MAIN_MENU;
                break;
            }
        cout << "=" << endl;
        }
    }
    return 0;
};
