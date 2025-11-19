//
// Created by Andrea Di Pietro on 19/11/25.
//

#include "BankAccount.h"

BankAccount::BankAccount(std::string name, double initialBalance)
        : ownerName(name), balance(initialBalance) {
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

void BankAccount::deposit(double amount) {
    if (amount <= 0) return;
    balance += amount;
    addTransaction(amount, Transaction::INCOME, "Deposit");
}

bool BankAccount::withdraw(double amount) {
    if (amount <= 0 || balance < amount) return false;
    balance -= amount;
    addTransaction(amount, Transaction::EXPENSE, "Withdrawal");
    return true;
}

bool BankAccount::transfer(BankAccount& receiver, double amount) {
    if (amount <= 0 || balance < amount) return false;

    // levo soldi da questo conto
    balance -= amount;
    addTransaction(amount, Transaction::EXPENSE, "Transfer to " + receiver.getOwnerName());

    // aggiungo soldi all'altro conto
    // uso deposit() dell'altro conto per semplicità
    receiver.deposit(amount);

    return true;
}

void BankAccount::addTransaction(double amount, Transaction::Type type, std::string desc) {
    Transaction t(amount, type, desc);
    transactions.push_back(t);
}