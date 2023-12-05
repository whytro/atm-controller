#include "atm.h"

#define ATTEMPTS 5

//Abstractions for what should be database transactions

void Account::adjust_balance(int amount) {
    _balance += amount;
}
int Account::get_balance() {
    return _balance;
}
int Account::get_overdraw_limit() {
    return _overdraw_limit;
}

bool Account::check_pin(int pin) {
    if(_state == Status::DISABLED) {
        return false;
    }
    if(_pin != pin) {
        if(_login_attempts_remaining-- <= 0) {
            _state = Status::DISABLED;
        }
        return false;
    }

    _login_attempts_remaining = ATTEMPTS;
    _state = Status::AUTHED;
    return true;
}
Account::Status Account::get_state() {
    return _state;
}
void Account::deauth() {
    if(_state == Status::AUTHED) {
        _state = Status::UNAUTHED;
    }
}

int Account::get_attempts_count() {
    return _login_attempts_remaining;
}