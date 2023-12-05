#include <cassert>
#include <iostream>
#include <limits.h>

#include "atm.h"

void TestCtrl::run_deposit_withdraw_flow_test() {
    _atm.insert_card(0);
    _atm.initial_auth_pin(1234);

    assert(_atm.see_balance().msg == "Current Balance: 500");
    assert(_atm.deposit(500).success);
    assert(_atm.see_balance().msg == "Current Balance: 1000");
    assert(_atm.withdraw(1000).success);
    assert(_atm.see_balance().msg == "Current Balance: 0");
    _atm.expel_card();

    try {
        _atm.deposit(500);
        assert(0);
    }
    catch(const std::exception& ex) {
        // Check to see if ATM correctly registers the ended session
    }

    std::cout << "Standard Withdraw/Deposit Flow Test Passed" << std::endl;
}

void TestCtrl::attempt_overdraw_test() {
    _atm.insert_card(0);
    _atm.initial_auth_pin(1234);

    assert(_atm.see_balance().msg == "Current Balance: 500");
    assert(_atm.withdraw(6000).msg == "Amount exceeds ATM's current balance.");
    assert(_atm.withdraw(600).msg == "Insufficient Funds.");
    assert(_atm.see_balance().msg == "Current Balance: 500");
    _atm.expel_card();

    try {
        _atm.deposit(100);
        _atm.withdraw(100);
        assert(0);
    }
    catch(const std::exception& ex) {
        // Check to see if ATM correctly registers the ended session
    }

    std::cout << "Overdraw for ATM balance, User balance Passed" << std::endl;
}

void TestCtrl::attempt_unusual_operations() {
    _atm.insert_card(0);
    _atm.initial_auth_pin(1234);

    assert(_atm.see_balance().msg == "Current Balance: 500");
    assert(_atm.withdraw(-500).msg == "Cannot withdraw negative or zero amount of cash.");
    const std::string invalid_deposit_msg = "Amount deposited cannot be negative or zero.";
    try {
        _atm.deposit(-500);
        assert(0);
    }
    catch(const std::exception& ex) {
        assert(ex.what() == invalid_deposit_msg);
    }
    try {
        _atm.deposit(0);
        assert(0);
    }
    catch(const std::exception& ex) {
        assert(ex.what() == invalid_deposit_msg);
    }
    assert(_atm.withdraw(0).msg == "Cannot withdraw negative or zero amount of cash.");
    assert(_atm.see_balance().msg == "Current Balance: 500");

    const std::string overflow_msg = "Amount deposited would cause an integer overflow.";
    try {
        _atm.deposit(INT_MAX);
        assert(0);
    }
    catch(const std::exception& ex) {
        assert(ex.what() == overflow_msg);
    }
    
    _atm.expel_card();

    try {
        _atm.deposit(100);
        _atm.withdraw(100);
        assert(0);
    }
    catch(const std::exception& ex) {
        // Check to see if ATM correctly registers the ended session
    }

    std::cout << "Unusual Withdrawal/Deposit Tests Passed" << std::endl;
}

void TestCtrl::check_non_accholder() {
    _atm.expel_card();
    assert(_atm.insert_card(1).success == false);
    
    const std::string invalid_acc_state = "Attempted operation on invalid account instance.";
    try {
        _atm.initial_auth_pin(1234);
        assert(0);
    }
    catch(const std::exception& ex) {
        assert(ex.what() == invalid_acc_state);
    }

    std::cout << "Non Account Holder Test Passed" << std::endl;
}

void TestCtrl::check_non_authed() {
    assert(_atm.insert_card(0).success);
    
    const std::string invalid_acc_state = "Attempted deposit operation on non-authenticated account.";
    try {
        _atm.deposit(500);
        assert(0);
    }
    catch(const std::exception& ex) {
        assert(ex.what() == invalid_acc_state);
    }

    std::cout << "Non Authed Test Passed" << std::endl;
}

void TestCtrl::run_acc_state_tests() {
    const std::string invalid_acc_state = "Attempted operation on invalid account instance.";
    try {
        _atm.expel_card();
        _atm.initial_auth_pin(500);
        assert(0);
    }
    catch(const std::exception& ex) {
        assert(ex.what() == invalid_acc_state);
    }
    try {
        _atm.deposit(500);
        assert(0);
    }
    catch(const std::exception& ex) {
        assert(ex.what() == invalid_acc_state);
    }
    try {
        _atm.withdraw(500);
        assert(0);
    }
    catch(const std::exception& ex) {
        assert(ex.what() == invalid_acc_state);
    }
    try {
        _atm.expel_card();
        assert(0);
    }
    catch(const std::exception& ex) {
        assert(ex.what() == invalid_acc_state);
    }

    const std::string invalid_account_card = "Attempted to insert card in midst of another instance";
    try {
        _atm.insert_card(0);
        _atm.insert_card(0);
        assert(0);
    }
    catch(const std::exception& ex) {
        assert(ex.what() == invalid_account_card);
    }

    std::cout << "Invalid Account Instances Tests Passed" << std::endl;
}

