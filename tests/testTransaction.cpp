//
// Created by Andrea Di Pietro on 19/11/25.
//

#include "gtest/gtest.h"
#include "../Transaction.h"

TEST(TransactionTest, ConstructorValues) {
Transaction t(100.0, Transaction::INCOME, "Stipendio");

ASSERT_EQ(t.getAmount(), 100.0);
ASSERT_EQ(t.getType(), Transaction::INCOME);
ASSERT_EQ(t.getDescription(), "Stipendio");
}

TEST(TransactionTest, DateIsGenerated) {
Transaction t(50.0, Transaction::EXPENSE, "Spesa");

std::string date = t.getDate();

// controlla che la stringa non sia vuota e che sia lunga il giusto
ASSERT_FALSE(date.empty());
ASSERT_GT(date.length(), 10); // GT = Greater Than (Maggiore di)
}

TEST(TransactionTest, LongDescription) {
std::string desc = "Pagamento per la cena di ieri sera con gli amici";
Transaction t(25.50, Transaction::EXPENSE, desc);

ASSERT_EQ(t.getDescription(), "Pagamento per la cena di ieri sera con gli amici");
}