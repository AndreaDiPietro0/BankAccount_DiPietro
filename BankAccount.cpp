//
// Created by Andrea Di Pietro on 19/11/25.
//

#include "BankAccount.h"
#include "Transaction.h"
#include <utility>

BankAccount::BankAccount(std::string name, double initialBalance, std::string iban) : ownerName(std::move(name)), balance(initialBalance), iban(std::move(iban)) {
    if (initialBalance > 0) {
        addTransaction(Transaction(initialBalance, Transaction::INCOME, "Saldo Iniziale"));
    }
}

double BankAccount::getBalance() const {
    return balance;
}

std::string BankAccount::getOwnerName() const {
    return ownerName;
}

std::vector<Transaction> BankAccount::getTransactionHistory() const {
    return transactions;
}

void BankAccount::deposit(double amount, std::string description) {
    if (amount <= 0) return;
    balance += amount;
    addTransaction(Transaction(amount, Transaction::INCOME, description));
}

bool BankAccount::withdraw(double amount, std::string description) {
    if (amount > balance || amount <= 0) {
        return false;
    }
    balance -= amount;
    addTransaction(Transaction(amount, Transaction::EXPENSE, description));
    return true;
}

bool BankAccount::transfer(double amount, const std::string& recipientIban, const std::string& recipientName) {
    if (amount <= 0 || amount > balance) {
        return false;
    }
    balance -= amount;
    std::string description = "Bonifico a " + recipientName + " (IBAN: " + recipientIban + ")";
    addTransaction(Transaction(amount, Transaction::EXPENSE, description));
    return true;
}

void BankAccount::addTransaction(const Transaction& transaction) {
    transactions.push_back(transaction); // transactions è il vettore delle transaz
}

std::string BankAccount::getIban() const {
    return iban;
}

void BankAccount::setIban(const std::string &newIban) {
    iban = newIban;
}
