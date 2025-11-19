#include <iostream>
#include <limits> //per pulire il buffer di input
#include "BankAccount.h"

int main() {
    std::string name;
    double initialBalance;

    std::cout << "Welcome to the Bank!" << std::endl;
    std::cout << "Please enter your Name : ";
    std::getline(std::cin, name); //con solo cin il secondo nome non viene inserito e crasha

    std::cout << "Enter initial balance: ";
    std::cin >> initialBalance;

    BankAccount myAccount(name, initialBalance);
    BankAccount externalAccount("Guest User", 0.0);     // guest user per testare i bonifici

    int choice;
    do {
        std::cout << "Owner: " << myAccount.getOwnerName() << " | Current Balance: " << myAccount.getBalance() << " EUR" << std::endl;
        std::cout << "Choose an operation:" << std::endl;
        std::cout << "1. Deposit (Versamento)" << std::endl;
        std::cout << "2. Withdraw (Prelievo)" << std::endl;
        std::cout << "3. Transfer (Bonifico)" << std::endl;
        std::cout << "4. View Transaction History (Storico)" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Your choice: ";
        std::cin >> choice;

        double amount;

        switch (choice) {
            case 1: // deposito
                std::cout << "Enter amount to deposit: ";
                std::cin >> amount;
                myAccount.deposit(amount);
                std::cout << "Deposit completed!" << std::endl;
                break;

            case 2: // prelievo
                std::cout << "Enter amount to withdraw: ";
                std::cin >> amount;
                if (myAccount.withdraw(amount)) {
                    std::cout << "Withdrawal successful!" << std::endl;
                } else {
                    std::cout << "Error: you don't have enough money." << std::endl;
                }
                break;

            case 3: // bonifico
                std::cout << "Enter amount to transfer to " << externalAccount.getOwnerName() << ": ";
                std::cin >> amount;
                if (myAccount.transfer(externalAccount, amount)) {
                    std::cout << "Transfer successful!" << std::endl;
                } else {
                    std::cout << "Error: you don't have enough money to transfer. " << std::endl;
                }
                break;

            case 4: { // cronologia
                std::cout << "\n--- TRANSACTION HISTORY ---" << std::endl;
                // for che scorrere il vettore delle transaz
                for (const auto& t : myAccount.getTransactionHistory()) {   // chiama il metodo che restituisce il vettore con le transazioni
                    std::string typeStr = (t.getType() == Transaction::INCOME) ? "+" : "-";   // + se deposito, - se bonifico o prelievo
                    std::cout << "[" << t.getDate() << "] "
                              << typeStr << t.getAmount() << " EUR "
                              << "(" << t.getDescription() << ")" << std::endl;
                }
                std::cout << "---------------------------" << std::endl;
                std::cout << "Press any key and Enter to continue..."; // pausa
                std::string dummy; std::cin >> dummy; // var x aspettare che utente scriva qualcosa
                break;
            }

            case 0:
                std::cout << "Goodbye!" << std::endl;
                break;

            default:
                std::cout << "Invalid choice, please try again." << std::endl;
        }

    } while (choice != 0);

    return 0;
}
