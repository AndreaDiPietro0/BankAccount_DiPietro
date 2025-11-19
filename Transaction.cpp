//
// Created by Andrea Di Pietro on 19/11/25.
//

#include "Transaction.h"

#include "Transaction.h"
#include <chrono>
#include <iomanip>
#include <sstream>

Transaction::Transaction(double amt, Type t, std::string desc)
        : amount(amt), type(t), description(desc) {

    // Logica per ottenere la data e ora corrente automaticamente
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");
    date = ss.str();
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