# ATM Controller

Code for a simple ATM controller.

This project and its tests can be built with the following commands:
```
git clone https://github.com/whytro/atm-controller.git
cd atm-controller
mkdir build && cd build
cmake ..
make
./demo
```

The main access point is `atm.h`, which is the ATM class.
The ATM class allows the insertion of a card (`insert_card(unsigned long long card_num)`), followed by the authentication of the card with the PIN (`initial_auth_pin(int pin)`).

Seeing balance(`see_balance()`), depositing(`deposit(int amount)`), or withdrawing(`withdraw(int amount)`) operations are blocked until authentication has been completed.

Tests include:
 - Running standard deposit withdraw flow on a valid card/account holder
 - Attempting to overdraw over the limit, on either the ATM's balance, or the account holder's balance
 - Attempting unusual operations (ie. withdrawing negative amounts, depositing negative amounts, or depositing an amount that would cause an overflow)
 - Attempting to do operations on a non-authed account, or a non account holder
 - Attempting to do operations without a valid session (no card inserted)
