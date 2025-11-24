//
// Created by Andrea Di Pietro on 19/11/25.
//

#include "BankAccount.h"
#include <utility>

BankAccount::BankAccount(std::string name, double initialBalance) : ownerName(std::move(name)), balance(initialBalance) {
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
    addTransaction(amount, Transaction::INCOME, "Deposito");
}

bool BankAccount::withdraw(double amount) {
    if (amount <= 0 || balance < amount) return false;
    balance -= amount;
    addTransaction(amount, Transaction::EXPENSE, "Prelievo");
    return true;
}

bool BankAccount::transfer(BankAccount &receiver, double amount) {
    if (amount <= 0 || balance < amount) return false;

    balance -= amount;     // levo soldi da questo conto
    addTransaction(amount, Transaction::EXPENSE, "Bonifico a " + receiver.getOwnerName());

    receiver.deposit(amount);     //per aggiungere soldi all'altro conto uso il metodo deposit()

    return true;
}

void BankAccount::addTransaction(double amount, Transaction::Type type, std::string desc) {
    Transaction t(amount, type, std::move(desc));
    transactions.push_back(t);
}