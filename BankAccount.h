//
// Created by Andrea Di Pietro on 19/11/25.
//

#ifndef BANKACCOUNT_BANKACCOUNT_H
#define BANKACCOUNT_BANKACCOUNT_H

#include <string>
#include <vector>
#include "Transaction.h"

class BankAccount {
public:
    explicit BankAccount(std::string ownerName, double initialBalance = 0.0);

    double getBalance() const;
    std::string getOwnerName() const;
    std::vector<Transaction> getTransactionHistory() const;

    // operazioni possibili
    void deposit(double amount);
    bool withdraw(double amount);
    bool transfer(BankAccount& receiver, double amount);

private:
    std::string ownerName;
    double balance;
    std::vector<Transaction> transactions; // storico movimenti

    // metodo helper privato per registrare la transazione
    void addTransaction(double amount, Transaction::Type type, std::string description);
};


#endif //BANKACCOUNT_BANKACCOUNT_H
