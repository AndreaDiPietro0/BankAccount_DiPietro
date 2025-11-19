//
// Created by Andrea Di Pietro on 19/11/25.
//

#include "gtest/gtest.h"
#include "../BankAccount.h" // Torniamo indietro di una cartella per trovare l'header

// --- FIXTURE CLASS ---
// classe x  creare un ambiente comune per i test.
class BankAccountTest : public ::testing::Test {
protected:
    //  eseguito prima di ogni singolo TEST_F
    void SetUp() override {
        account = new BankAccount("Test User", 100.0); // Saldo iniziale 100
    }

    //  eseguito opo ogni singolo TEST_F
    void TearDown() override {
        delete account;
    }

    BankAccount* account; // Puntatore all'oggetto da testare
};


// test costruttore
TEST_F(BankAccountTest, InitialBalanceCheck) {
ASSERT_EQ(account->getBalance(), 100.0);
ASSERT_EQ(account->getOwnerName(), "Test User");
}

// test deposit
TEST_F(BankAccountTest, DepositIncreasesBalance) {
account->deposit(50.0);
ASSERT_EQ(account->getBalance(), 150.0); // 100 + 50 = 150
}

// test importi negativi
TEST_F(BankAccountTest, DepositNegativeAmount) {
account->deposit(-50.0);
ASSERT_EQ(account->getBalance(), 100.0); // il saldo non deve cambiare
}

// test prelievo
TEST_F(BankAccountTest, WithdrawSuccess) {
bool result = account->withdraw(40.0);
ASSERT_TRUE(result); // deve ritornare true
ASSERT_EQ(account->getBalance(), 60.0); // 100 - 40 = 60
}

// test prelievo superiore
TEST_F(BankAccountTest, WithdrawFailureInsufficientFunds) {
bool result = account->withdraw(200.0);
ASSERT_FALSE(result);
ASSERT_EQ(account->getBalance(), 100.0); //  saldo non deve cambiare
}

//test bonifico
TEST_F(BankAccountTest, TransferSuccess) {
BankAccount secondAccount("Receiver", 0.0);

bool result = account->transfer(secondAccount, 30.0);

ASSERT_TRUE(result);
ASSERT_EQ(account->getBalance(), 70.0); // 100 - 30
ASSERT_EQ(secondAccount.getBalance(), 30.0); // 0 + 30
}

// test cronologia transaz
TEST_F(BankAccountTest, TransactionHistoryUpdate) {
account->deposit(50.0);
account->withdraw(20.0);

std::vector<Transaction> history = account->getTransactionHistory();

ASSERT_EQ(history.size(), 2); // 2 transaz
ASSERT_EQ(history[0].getType(), Transaction::INCOME);
ASSERT_EQ(history[1].getType(), Transaction::EXPENSE);
}