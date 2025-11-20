#include <iostream>
#include <limits> //per pulire il buffer di input
#include "BankAccount.h"

//controlla numeri
double getValidDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            // che non sia negativo
            if (value < 0) {
                std::cout << "Error, amount can't be negative, try again.\n";
            } else {
                // pulisce tipo se scrivo numeri e lettere insieme
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }
        } else {
            // se inserisco lettere
            std::cout << "Invalid input, enter a number.\n";
            std::cin.clear(); // reset errore
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // pulisce
        }
    }
}

// controlla stringhe
std::string getValidName(const std::string& prompt) {
    std::string name;
    while (true) {
        std::cout << prompt;
        // getline x nomi doppi
        std::getline(std::cin, name);

        //se vuoto
        if (name.empty()) {
            std::cout << "Name can't be empty.\n";
            continue;
        }

        //se contiene numeri
        bool hasDigits = false;
        for (char c : name) {
            if (std::isdigit(c)) {
                hasDigits = true;
                break;
            }
        }

        if (hasDigits) {
            std::cout << "Invalid name, names can't contain numbers.\n";
        } else {
            return name; // nome ok
        }
    }
}

int main() {
    //std::string name;
    //double initialBalance;

    std::cout << "Welcome to the Bank" << std::endl;

    std::string name = getValidName("Enter your Name (solo lettere): ");
    double initialBalance = getValidDouble("Enter initial balance: ");

    BankAccount myAccount(name, initialBalance);
    BankAccount externalAccount("Guest User", 0.0);     // guest user per testare i bonifici

    int choice;
    do {
        std::cout << "Owner: " << myAccount.getOwnerName() << " | Current Balance: " << myAccount.getBalance() << " EURO" << std::endl;
        std::cout << "Choose an operation:" << std::endl;
        std::cout << "1 - Deposit (Versamento)" << std::endl;
        std::cout << "2 - Withdraw (Prelievo)" << std::endl;
        std::cout << "3 - Transfer (Bonifico)" << std::endl;
        std::cout << "4 - View Transaction History (Storico)" << std::endl;
        std::cout << "0 - Exit" << std::endl;
        std::cout << "Your choice: ";
        //std::cin >> choice;

        // controlla inserimento numeri e non lettere nel menu
        if (!(std::cin >> choice)) {
            std::cout << "Invalid choice, enter a number." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue; // ricomincia il ciclo
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        double amount;

        switch (choice) {
            case 1: // deposito
                amount = getValidDouble("Enter amount to deposit: ");
                myAccount.deposit(amount);
                std::cout << "Deposit completed" << std::endl;
                break;

            case 2: // prelievo
                amount = getValidDouble("Enter amount to withdraw: ");
                if (myAccount.withdraw(amount)) {
                    std::cout << "Withdrawal successful" << std::endl;
                } else {
                    std::cout << "Error, insufficient funds." << std::endl;
                }
                break;

            case 3: // bonifico
                amount = getValidDouble("Enter amount to transfer: ");
                if (myAccount.transfer(externalAccount, amount)) {
                    std::cout << "Transfer successful" << std::endl;
                } else {
                    std::cout << "Error, insufficient funds." << std::endl;
                }
                break;

            case 4: { // cronologia
                std::cout << "\n- TRANSACTION HISTORY -" << std::endl;
                // for che scorrere il vettore delle transaz
                for (const auto& t : myAccount.getTransactionHistory()) {   // chiama il metodo che restituisce il vettore con le transazioni
                    std::string typeStr = (t.getType() == Transaction::INCOME) ? "+" : "-";   // + se deposito, - se bonifico o prelievo
                    std::cout << "[" << t.getDate() << "] "
                              << typeStr << t.getAmount() << " EURO "
                              << "(" << t.getDescription() << ")" << std::endl;
                }
                std::cout << "Press enter to continue";
                //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // pulisce buffer
                std::cin.get(); // aspetta enter
                break;
            }

            case 0:
                std::cout << "Goodbye" << std::endl;
                break;

            default:
                std::cout << "Invalid choice, please try again." << std::endl;
        }
        if (choice != 0 && choice != 4) {
            std::cout << "Press Enter to continue";
            //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
        }

    } while (choice != 0);

    return 0;
}
