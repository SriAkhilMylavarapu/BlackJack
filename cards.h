#ifndef _CARD_H_
#define _CARD_H_

#include <iostream>
#include <vector>
#include <string>

class Card
{
friend std::ostream &operator<<(std::ostream &os, const Card &rhs);
private:
    int symbol; //0-spade, 1-club, 2-heart, 3-diamond
    int number; //1-Ace, 11-Jack, 12-Queen, 13-King
public:
    Card();
    Card(int symbol , int number);
    int get_num() const;
    int get_sym() const;
};

class Player_Account
{
private:
    std::string name;
    int balance;
    int bet;
public:
    Player_Account(std::string name);
    Player_Account(std::string name , int balance);
    bool withdraw(int amount);
    void deposit(int amount);
    void display_balance() const;
    int get_balance() const;
    void input_bet(int amount);
    int get_bet() const;
};

class Player
{
protected:
    std::vector<Card> hand;
    std::string name;
    Player_Account *account;
public:
    Player(std::vector<Card> &deck);
    void clear_hand();
    int get_sum() const;
    void write_name(std::string name);
    std::string get_name() const;
    bool check_bust() const;
    void hit(std::vector<Card> &deck);
    void display() const;
    void create_account(Player_Account *account);
    Player_Account* get_account() const;
    bool operator<(Player &rhs) const;
};

class Dealer: public Player
{
public:
    Dealer(std::vector<Card> &deck);
    void hidden_display() const;
};


#endif