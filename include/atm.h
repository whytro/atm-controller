#ifndef ATM_H
#define ATM_H

#include "account.h"

#include <memory>
#include <string>
#include <unordered_map>

class ATM {
public:
    friend class TestCtrl;

    struct Response {
        bool success = false;
        std::string msg;
    };

    ATM::Response insert_card(unsigned long long card_num);
    ATM::Response expel_card();
    ATM::Response initial_auth_pin(int pin);

    ATM::Response see_balance();
    ATM::Response deposit(int amount);
    ATM::Response withdraw(int amount);

private:
    int _atm_balance;
    std::shared_ptr<Account> _current_instance;

    void instance_state_check();

    std::unordered_map<unsigned long long, Account> _pretend_db;
};

class TestCtrl {
public:
    TestCtrl(ATM atm) : _atm(std::move(atm)) {}

    void set_testdb(std::unordered_map<unsigned long long, Account> db) {
        _atm._pretend_db = std::move(db);
    }
    void set_pin(Account& acc, int pin) {
        acc._pin = pin;
    }
    void set_atm_balance(int bal) {
        _atm._atm_balance = bal;
    }

    void run_acc_state_tests();
    void run_deposit_withdraw_flow_test();
    void attempt_overdraw_test();
    void attempt_unusual_operations();
    void check_non_accholder();
    void check_non_authed();

private:
    ATM _atm;
};

#endif //ATM_H
