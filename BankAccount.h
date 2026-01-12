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
    explicit BankAccount(std::string ownerName, double initialBalance = 0.0, std::string iban = "");

    double getBalance() const;

    std::string getOwnerName() const;

    std::vector<Transaction> getTransactionHistory() const;

    void deposit(double amount, std::string description = "Deposito");
    bool withdraw(double amount, std::string description = "Prelievo");

    bool transfer(double amount, const std::string& recipientIban, const std::string& recipientName);
    std::string getIban() const;

    void setIban(const std::string &newIban);
    void setFilePath(const std::string& path);
    std::string getFilePath() const;
    void loadTransactionFromDB(const Transaction& t);

private:
    std::string ownerName;
    double balance;
    std::string iban;
    std::string filePath;
    std::vector<Transaction> transactions; // storico movimenti

    // metodo privato per registrare la transazione
    void addTransaction(const Transaction& transaction);
};


#endif //BANKACCOUNT_BANKACCOUNT_H
