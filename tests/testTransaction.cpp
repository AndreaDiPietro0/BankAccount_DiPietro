//
// Created by Andrea Di Pietro on 19/11/25.
//

#include "gtest/gtest.h"
#include "../Transaction.h"

// test valori costruttore
TEST(TransactionTest, ConstructorValues) {
    Transaction t(100.0, Transaction::INCOME, "Stipendio");

    ASSERT_EQ(t.getAmount(), 100.0);
    ASSERT_EQ(t.getType(), Transaction::INCOME);
    ASSERT_EQ(t.getDescription(), "Stipendio");
}

// generazione data
TEST(TransactionTest, DateIsGenerated) {
    Transaction t(50.0, Transaction::EXPENSE, "Spesa");

    std::string date = t.getDate();

    ASSERT_FALSE(date.empty());
    ASSERT_GT(date.length(), 10);
}

// descrizione
TEST(TransactionTest, LongDescription) {
    std::string desc = "Rimborso spese per la cena";
    Transaction t(25.50, Transaction::EXPENSE, desc);

    ASSERT_EQ(t.getDescription(), desc);
}