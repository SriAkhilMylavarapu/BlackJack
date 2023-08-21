#include <iostream>
#include "cards.h"
#define SPADE   "\xE2\x99\xA0"
#define CLUB    "\xE2\x99\xA3"
#define HEART   "\xE2\x99\xA5"
#define DIAMOND "\xE2\x99\xA6"

Card::Card(int symbol , int number)
    : symbol{symbol} , number{number} {}

Card::Card()
    : Card{1 , 1} {}

int Card::get_num() const
{
    return number;
}

int Card::get_sym() const
{
    return symbol;
}

std::ostream &operator<<(std::ostream &os, const Card &rhs) {
    char space;
    std::string sym[4] = {SPADE , CLUB , HEART , DIAMOND};
    std::string faces[13] = {"A" , "2" , "3" , "4" , "5" , "6" , "7" , "8" , "9" , "10" , "J" , "Q" , "K"};

    rhs.number == 10 ? space = '\0' : space = ' ';
    
    os << "┌─────────┐\n"
       << "|" << faces[rhs.number-1] << space << "       |\n"
       << "│         │\n"
       << "│         │\n"
       << "│    " << sym[rhs.symbol] << "    │\n"
       << "│         │\n"
       << "│         │\n"
       << "|       " << space << faces[rhs.number-1] << "|\n"
       << "└─────────┘\n\n";

    return os;
}


Player::Player(std::vector<Card> &deck)
    : hand {*deck.rbegin() , *(deck.rbegin() + 1)}  {
        deck.pop_back();
        deck.pop_back();
    }

void Player::clear_hand()
{
    hand.clear();
}

int Player::get_sum() const
{
    int count {0};
    int sum {0};
    for (auto i : hand)
    {
        if (i.get_num() == 1)
        {
            count++;
            sum += 11;
        }
        else if (i.get_num() >= 10)
        {
            sum += 10;
        }
        else
        {
            sum += i.get_num();
        }
        
        while (sum > 21 && count > 0)
        {
            sum -= 10;
            count--;
        }
    }
    return sum;
}

void Player::write_name(std::string naem)
{
    name = naem;
}

std::string Player::get_name() const
{
    return name;
}

bool Player::check_bust() const
{
    if(this->get_sum() > 21)
        return true;
    else
        return false;
}

void Player::hit(std::vector<Card> &deck)
{
    hand.push_back(*deck.rbegin());
    deck.pop_back();
}

void Player::display() const
{
    char space;
    std::string sym[4] = {SPADE , CLUB , HEART , DIAMOND};
    std::string faces[13] = {"A" , "2" , "3" , "4" , "5" , "6" , "7" , "8" , "9" , "10" , "J" , "Q" , "K"};
    
    for (size_t i = 0 ; i < hand.size() ; i++)
    {
        std::cout << "┌─────────┐              ";
    }
    std::cout << "\n";
    for (auto i : hand)
    {
        i.get_num() == 10 ? space = '\0' : space = ' ';
        std::cout << "|" << faces[i.get_num() - 1] << space << "       |              ";
    }
    std::cout << "\n";
    for (size_t i = 0 ; i < hand.size() ; i++)
    {
        std::cout << "│         │              ";
    }
    std::cout << "\n";
    for (size_t i = 0 ; i < hand.size() ; i++)
    {
        std::cout << "│         │              ";
    }
    std::cout << "\n";
    for (auto i : hand)
    {
        std::cout << "│    " << sym[i.get_sym()] << "    │              ";
    }
    std::cout << "\n";
    for (size_t i = 0 ; i < hand.size() ; i++)
    {
        std::cout << "│         │              ";
    }
    std::cout << "\n";
    for (size_t i = 0 ; i < hand.size() ; i++)
    {
        std::cout << "│         │              ";
    }
    std::cout << "\n";
    for (auto i : hand)
    {
        i.get_num() == 10 ? space = '\0' : space = ' ';
        std::cout << "|       " << space << faces[i.get_num() - 1] << "|              ";
    }
    std::cout << "\n";
    for (size_t i = 0 ; i < hand.size() ; i++)
    {
        std::cout << "└─────────┘              ";
    }
    std::cout << "\n\n";
}

void Player::create_account(Player_Account *accoun)
{
    account = accoun;
}

Player_Account* Player::get_account() const
{
    return this->account;
}

bool Player::operator<(Player &rhs) const
{
    return this->get_sum() < rhs.get_sum();
}

Dealer::Dealer(std::vector<Card> &deck)
    : Player(deck) {
        this->write_name("Dealer");
    }

void Dealer::hidden_display() const
{
    char space;
    std::string sym[4] = {SPADE , CLUB , HEART , DIAMOND};
    std::string faces[13] = {"A" , "2" , "3" , "4" , "5" , "6" , "7" , "8" , "9" , "10" , "J" , "Q" , "K"};
    hand[0].get_num() == 10 ? space = '\0' : space = ' ';

    std::cout << "┌─────────┐              ┌─────────┐\n";
    std::cout << "|" << faces[hand[0].get_num() - 1] << space << "       |              │░░░░░░░░░│\n";
    std::cout << "│         │              │░░░░░░░░░│\n";
    std::cout << "│         │              │░░░░░░░░░│\n";
    std::cout << "│    " << sym[hand[0].get_sym()] << "    │              │░░░░░░░░░│\n";
    std::cout << "│         │              │░░░░░░░░░│\n";
    std::cout << "│         │              │░░░░░░░░░│\n";
    std::cout << "|       " << space << faces[hand[0].get_num() - 1] << "|              │░░░░░░░░░│\n";
    std::cout << "└─────────┘              └─────────┘\n";

}


Player_Account::Player_Account(std::string name , int balance)
    : name {name} , balance {balance} , bet{0} {}

Player_Account::Player_Account(std::string name)
    : Player_Account(name , 1000) {}

bool Player_Account::withdraw(int amount)
{
    if (balance >= amount)
    {
        balance -= amount;
        std::cout << "\n" << amount << " has been deducted from balance\n";
        std::cout << "Remaining balance : " << balance << "\n\n";
        return true;
    }
    else
    {
        std::cout << "\nInsufficeint Funds\n";
        std::cout << "\nRemaining balance : " << balance << "\n\n";
        return false;
    }
}

void Player_Account::deposit(int amount)
{
    balance += amount;
    std::cout << "\n" << amount << " has been added to balance\n";
    std::cout << "Current balance : " << balance << "\n";
}

void Player_Account::display_balance() const
{
    std::cout << "\nCurrent balance of " << name << " is " << balance << "\n\n";
}

int Player_Account::get_balance() const
{
    return balance;
}

void Player_Account::input_bet(int amount)
{
    bet = amount;
}

int Player_Account::get_bet() const
{
    return bet;
}