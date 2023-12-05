#include "atm.h"
#include "account.h"

#include <stdexcept>
#include <limits.h>
#include <string>

#include <unordered_map>

ATM::Response ATM::insert_card(unsigned long long card_num) {
    if(_current_instance.get() != nullptr) {
        throw std::logic_error("Attempted to insert card in midst of another instance");
    }

    Response res;
    // Abstract getting account from database into getting it from a map
    if(_pretend_db.find(card_num) == _pretend_db.end()) {
        res.msg = "Account could not be found.";
        return res;
    }

    // Pretending SELECT query -> ORM/Assignments of Account obj
    _current_instance = std::make_shared<Account>(_pretend_db[card_num]);

    res.msg = "Awaiting PIN Authentication.";
    res.success = true;
    return res;
}

ATM::Response ATM::expel_card() {
    instance_state_check();
    
    _current_instance->deauth();
    _current_instance.reset();

    Response res;
    res.msg = "Take card.";
    res.success = true;
    return res;
}

void ATM::instance_state_check() {
    if(_current_instance.get() == nullptr) {
        throw std::logic_error("Attempted operation on invalid account instance.");
    }
}

ATM::Response ATM::initial_auth_pin(int pin) {
    instance_state_check();
    if(_current_instance->get_state() == Account::Status::AUTHED) {
        throw std::logic_error("Attempted to PIN auth on an already unlocked account.");
    }

    Response res;
    
    if(_current_instance->check_pin(pin)) {
        res.msg = "Incorrect PIN. Attempts Remaining: " + _current_instance->get_attempts_count();
    }
    if(!_current_instance->get_state() == Account::Status::DISABLED) {
        res.msg = "Account has been locked. Contact the bank.";
        return res;
    }
    if(_current_instance->get_state() != Account::Status::AUTHED) {
        throw std::logic_error("Account has not been authed.");
    }
    
    res.msg = "Auth successful.";
    res.success = true;
    return res;
}

ATM::Response ATM::see_balance() {
    instance_state_check();

    if(_current_instance->get_state() != Account::Status::AUTHED) {
        throw std::logic_error("Attempted deposit operation on non-authenticated account.");
    }

    Response res;
    res.msg =  "Current Balance: " + std::to_string(_current_instance->get_balance());
    res.success = true;

    return res;
}

ATM::Response ATM::deposit(int amount) {
    instance_state_check();
    if(_current_instance->get_state() != Account::Status::AUTHED) {
        throw std::logic_error("Attempted deposit operation on non-authenticated account.");
    }
    if((double)_atm_balance+amount > INT_MAX || (double)_current_instance->get_balance()+amount > INT_MAX) {
        throw std::overflow_error("Amount deposited would cause an integer overflow.");
    }
    if(amount <= 0) {
        throw std::logic_error("Amount deposited cannot be negative or zero.");
    }
    
    // Register deposit in account
    _current_instance->adjust_balance(amount);
    // Update ATM's current cash amount
    _atm_balance += amount;

    Response res;
    res.msg = "Deposited " + std::to_string(amount) + " Current Balance: " + std::to_string(_current_instance->get_balance());
    res.success = true;

    return res;
}

ATM::Response ATM::withdraw(int amount) {
    instance_state_check();

    if(_current_instance->get_state() != Account::Status::AUTHED) {
        throw std::logic_error("Attempted deposit operation on non-authenticated account.");
    }

    Response res;

    if(amount <= 0) {
        res.msg = "Cannot withdraw negative or zero amount of cash.";
        return res;
    }
    if(amount > _atm_balance) {
        res.msg = "Amount exceeds ATM's current balance.";
        return res;
    }
    if(_current_instance->get_balance()-amount+_current_instance->get_overdraw_limit() < 0) {
        res.msg = "Insufficient Funds.";
        return res;
    }

    // Register withdrawal
    _current_instance->adjust_balance(-1*amount);
    // Update ATM's current cash amount
    _atm_balance -= amount;

    res.msg = "Withdrew " + std::to_string(amount) + " Current Balance: " + std::to_string(_current_instance->get_balance());
    res.success = true;

    return res;
}
