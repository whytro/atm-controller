#include <cassert>
#include <iostream>
#include <unordered_map>

#include "atm.h"

int main() {
    ATM atm;

    TestCtrl tester(atm);

    std::unordered_map<unsigned long long, Account> test_db;
    Account test_acc;
    test_acc.adjust_balance(500);
    tester.set_pin(test_acc, 1234);
    test_db[0] = test_acc;
    tester.set_testdb(test_db);
    tester.set_atm_balance(5000);

    tester.run_deposit_withdraw_flow_test();
    tester.attempt_overdraw_test();
    tester.attempt_unusual_operations();
    tester.run_acc_state_tests();
    tester.check_non_accholder();
    tester.check_non_authed();
    
    

    

    


    
    // atm.initial_auth_pin(500);
    // atm.deposit(500);
    // atm.withdraw(500);
    // atm.expel_card();
    

    return 0;
}
