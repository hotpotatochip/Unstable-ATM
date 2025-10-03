#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif

#include "atm.hpp"
#include "catch.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////
//                             Helper Definitions //
/////////////////////////////////////////////////////////////////////////////////////////////

bool CompareFiles(const std::string& p1, const std::string& p2) {
  std::ifstream f1(p1);
  std::ifstream f2(p2);

  if (f1.fail() || f2.fail()) {
    return false;  // file problem
  }

  std::string f1_read;
  std::string f2_read;
  while (f1.good() || f2.good()) {
    f1 >> f1_read;
    f2 >> f2_read;
    if (f1_read != f2_read || (f1.good() && !f2.good()) ||
        (!f1.good() && f2.good()))
      return false;
  }
  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Test Cases
/////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Example: Create a new account", "[ex-1]") {
  Atm atm;
  atm.RegisterAccount(12345678, 1234, "Sam Sepiol", 300.30);
  auto accounts = atm.GetAccounts();
  REQUIRE(accounts.contains({12345678, 1234}));
  REQUIRE(accounts.size() == 1);

  Account sam_account = accounts[{12345678, 1234}];
  REQUIRE(sam_account.owner_name == "Sam Sepiol");
  REQUIRE(sam_account.balance == 300.30);

  auto transactions = atm.GetTransactions();
  REQUIRE(accounts.contains({12345678, 1234}));
  REQUIRE(accounts.size() == 1);
  std::vector<std::string> empty;
  REQUIRE(transactions[{12345678, 1234}] == empty);
}

TEST_CASE("Example: Simple widthdraw", "[ex-2]") {
  Atm atm;
  atm.RegisterAccount(12345678, 1234, "Sam Sepiol", 300.30);
  atm.WithdrawCash(12345678, 1234, 20);
  auto accounts = atm.GetAccounts();
  Account sam_account = accounts[{12345678, 1234}];

  REQUIRE(sam_account.balance == 280.30);
}

TEST_CASE("Example: Print Prompt Ledger", "[ex-3]") {
  Atm atm;
  atm.RegisterAccount(12345678, 1234, "Sam Sepiol", 300.30);
  auto& transactions = atm.GetTransactions();
  transactions[{12345678, 1234}].push_back(
      "Withdrawal - Amount: $200.40, Updated Balance: $99.90");
  transactions[{12345678, 1234}].push_back(
      "Deposit - Amount: $40000.00, Updated Balance: $40099.90");
  transactions[{12345678, 1234}].push_back(
      "Deposit - Amount: $32000.00, Updated Balance: $72099.90");
  atm.PrintLedger("./prompt.txt", 12345678, 1234);
  REQUIRE(CompareFiles("./ex-1.txt", "./prompt.txt"));
}

TEST_CASE("TEST1", "[Test1]") {   
  Atm atm;   atm.RegisterAccount(11112222, 1234, "Alice Johnson", 500.00);   
  atm.RegisterAccount(33334444, 5678, "Bob Smith", 1000.00);    
  REQUIRE(atm.CheckBalance(11112222, 1234) == 500.00);   
  REQUIRE(atm.CheckBalance(33334444, 5678) == 1000.00);    
  atm.WithdrawCash(11112222, 1234, 100.00);   
  REQUIRE(atm.CheckBalance(11112222, 1234) == 400.00);    
  atm.DepositCash(33334444, 5678, 200.00);   
  REQUIRE(atm.CheckBalance(33334444, 5678) == 1200.00);    
  atm.PrintLedger("./test1.txt", 11112222, 1234);   
  atm.PrintLedger("./test2.txt", 33334444, 5678);    
  REQUIRE(CompareFiles("./test1_expected.txt", "./test1.txt"));   
  REQUIRE(CompareFiles("./test2_expected.txt", "./test2.txt")); 
}  
  
TEST_CASE("TEST2", "[Test2]") {   
  Atm atm;   
  atm.RegisterAccount(55556666, 4321, "Charlie Brown", 750.00);    
  REQUIRE(atm.CheckBalance(55556666, 4321) == 750.00);   
  REQUIRE_THROWS_AS(atm.WithdrawCash(55556666, 2323, 50.00),                     
  std::invalid_argument);   
  REQUIRE_THROWS_AS(atm.WithdrawCash(33335555, 4321, 50.00),                     
  std::invalid_argument);   
  REQUIRE_THROWS_AS(atm.WithdrawCash(55556666, 4321, -50.00),                     
  std::invalid_argument);   
  REQUIRE_THROWS_AS(atm.WithdrawCash(55556666, 4321, 800.00),                     
  std::runtime_error); }  
  
TEST_CASE("TEST3", "[Test3]") {   
  Atm atm;   
  atm.RegisterAccount(55556666, 4321, "Charlie Brown", 750.00);   
  REQUIRE(atm.CheckBalance(55556666, 4321) == 750.00);   
  REQUIRE_THROWS_AS(atm.DepositCash(55556666, 2323, 50.00),                     
  std::invalid_argument);   
  REQUIRE_THROWS_AS(atm.DepositCash(33335555, 4321, 50.00),                     
  std::invalid_argument);   
  REQUIRE_THROWS_AS(atm.DepositCash(55556666, 4321, -50.00),                     
  std::invalid_argument); }  
  
TEST_CASE("TEST4", "[Test4]") {   
  Atm atm;   
  atm.RegisterAccount(55556666, 4321, "Charlie Brown", 750.00);   
  REQUIRE(atm.CheckBalance(55556666, 4321) == 750.00);  
  REQUIRE_THROWS_AS(atm.PrintLedger("./test4.txt", 55556666, 2323),                     
  std::invalid_argument);   
  REQUIRE_THROWS_AS(atm.PrintLedger("./test4.txt", 33335555, 4321),                     
  std::invalid_argument); 
}