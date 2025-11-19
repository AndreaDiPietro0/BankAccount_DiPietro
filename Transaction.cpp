//
// Created by Andrea Di Pietro on 19/11/25.
//

#include "Transaction.h"

#include "Transaction.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <utility>

Transaction::Transaction(double amt, Type t, std::string desc)
        : amount(amt), type(t), description(std::move(desc)) {

    auto now = std::chrono::system_clock::now();  // prende ora esatta
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);   //converte time point in time t
    std::stringstream ss;  //stampa su una var di memoria
    ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S"); //prende l'ora e separa tempo in a,m,g..., put time ordina i valori in Y-M... e mette in ss
    date = ss.str();  // stringa date contiene data ok
}

double Transaction::getAmount() const {
    return amount;
}

Transaction::Type Transaction::getType() const {
    return type;
}

std::string Transaction::getDescription() const {
    return description;
}

std::string Transaction::getDate() const {
    return date;
}