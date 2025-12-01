//
// Created by Andrea Di Pietro on 19/11/25.
//

#ifndef BANKACCOUNT_TRANSACTION_H
#define BANKACCOUNT_TRANSACTION_H

#include <string>
#include <ctime>

class Transaction {
public:
    // 2 tipi di transazione possibili
    enum Type {
        INCOME, EXPENSE
    };

    explicit Transaction(double amount, Type type, std::string description);

    double getAmount() const;

    Type getType() const;

    std::string getDescription() const;

    std::string getDate() const; //restituisce la data come stringa

private:
    double amount;
    Type type;
    std::string description;
    std::string date;
};


#endif //BANKACCOUNT_TRANSACTION_H
