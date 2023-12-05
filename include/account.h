#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "atm.h"

class Account {
public:
    friend class TestCtrl;

    enum Status { UNAUTHED, AUTHED, DISABLED };

    void adjust_balance(int balance);
    int get_balance();
    int get_overdraw_limit();

    bool check_pin(int pin);
    Status get_state();
    void deauth();

    int get_attempts_count();

private:
    int _balance = 0;
    int _overdraw_limit = 0;

    int _pin = 0;
    int _login_attempts_remaining = 0;

    Status _state;
};

#endif //ACCOUNT_H
