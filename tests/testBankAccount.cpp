//
// Created by Andrea Di Pietro on 19/11/25.
//

#include "gtest/gtest.h"
#include "../BankAccount.h"

class BankAccountTest : public ::testing::Test {
protected:
    void SetUp() override {
        account = new BankAccount("Luca", 100.0, "IT0000000000000000000000000");
    }

    void TearDown() override {
        delete account;
    }

    BankAccount* account;
};

// 1 costruttore e getter
TEST_F(BankAccountTest, InitialBalanceCheck) {
    ASSERT_EQ(account->getBalance(), 100.0);
    ASSERT_EQ(account->getOwnerName(), "Luca");
    ASSERT_EQ(account->getIban(), "IT0000000000000000000000000");

    ASSERT_EQ(account->getTransactionHistory().size(), 1); // verifica che ci sia già la prima transazione di apertura conto
}

//2 deposito > 0
TEST_F(BankAccountTest, DepositIncreasesBalance) {
    account->deposit(50.0);
    ASSERT_EQ(account->getBalance(), 150.0);
}

// 3 test deposito < 0
TEST_F(BankAccountTest, DepositNegativeAmount) {
    account->deposit(-50.0);
    ASSERT_EQ(account->getBalance(), 100.0);
}

// 4 prelievo
TEST_F(BankAccountTest, WithdrawSuccess) {
    bool result = account->withdraw(40.0);
    ASSERT_TRUE(result);
    ASSERT_EQ(account->getBalance(), 60.0);
}

// 5 prelievo > fondi
TEST_F(BankAccountTest, WithdrawFailureInsufficientFunds) {
    bool result = account->withdraw(200.0);
    ASSERT_FALSE(result);
    ASSERT_EQ(account->getBalance(), 100.0);
}

// 6 bonifico
TEST_F(BankAccountTest, TransferReducesSenderBalance) {
    bool result = account->transfer(30.0, "IT1111111111111111111111111", "Andrea");

    ASSERT_TRUE(result);
    ASSERT_EQ(account->getBalance(), 70.0);

    auto history = account->getTransactionHistory(); // controlla che abbia creato la transazione corretta

    ASSERT_EQ(history.size(), 2); // dimensione 2, 1iniziale + 1 bonifico

    std::string lastDesc = history.back().getDescription();     // controlla descrizione
    ASSERT_NE(lastDesc.find("Andrea"), std::string::npos); // cerca il nome
    ASSERT_NE(lastDesc.find("IT1111111111111111111111111"), std::string::npos); // cerca iban
}

// 7 cronologia
TEST_F(BankAccountTest, TransactionHistoryUpdate) {
    //1 transazione saldo iniziale
    account->deposit(50.0);  // transazione 2
    account->withdraw(20.0); // transazione 3

    std::vector<Transaction> history = account->getTransactionHistory();

    ASSERT_EQ(history.size(), 3);     // devono esser 3 transazioni

    // verifica i tipi
    ASSERT_EQ(history[0].getType(), Transaction::INCOME);  // saldo iniziale
    ASSERT_EQ(history[1].getType(), Transaction::INCOME);  // deposito
    ASSERT_EQ(history[2].getType(), Transaction::EXPENSE); // prelievo
}