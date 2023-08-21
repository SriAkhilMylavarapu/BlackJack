#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
#include "cards.h"

#define SPADE   "\xE2\x99\xA0"
#define CLUB    "\xE2\x99\xA3"
#define HEART   "\xE2\x99\xA5"
#define DIAMOND "\xE2\x99\xA6"

using namespace std;

void clear_buffer()
{
    int c;
    while ( (c = getchar()) != '\n' && c != EOF ) { }
}

void display_cards(Player *player , Dealer *dealer)
{
    cout << "\n\nDealer\n\n";
    dealer->display();
    cout << "\nDealer Total : " << dealer->get_sum() << endl;
    cout << "\n\n" << player->get_name() << "\n\n";
    player->display();
    cout << "\nCurrent Total : " << player->get_sum() << endl;
}

void reset_shuffle(vector<Card> &deck)
{
    deck.clear();

    for (size_t i = 0; i < 52; i++)
    {
        deck.push_back(Card(i%4 , (i%13)+1));
    }

    unsigned seed = chrono::system_clock::now()
                    .time_since_epoch()
                    .count();

    shuffle (deck.begin(), deck.end(), std::default_random_engine(seed));
}

void betting(Player_Account *player_acc)
{
    int bet;
    player_acc->display_balance();
    cout << "Place your bet : ";
    cin >> bet;
    while(!player_acc->withdraw(bet))
    {
        cout << "\nEnter a valid amount : ";
        cin >> bet;
        cout << "\n";
    }
    player_acc->input_bet(bet);
}

bool game_start(vector<Card> &deck , Player *player , Dealer *dealer , ifstream &save_data , fstream &temp)
{
    string choice;
    cout << "\n\n                 " << SPADE << " " << CLUB << " " << "WELCOME TO BLACKJACK" << " " << HEART << " " << DIAMOND << "\n\n\n" << endl;
    cout << "Begin Game\n\nEnter Y/N : ";
    cin >> choice;
    clear_buffer();
    choice[0] = toupper(choice[0]);

    if(choice[0] == 'Y')
    {
        string name;
        int balance;
        cout << "\nEnter your name : ";
        cin >> choice;
        clear_buffer();

        while (!save_data.eof())
        {
            save_data >> name >> balance;
            if (name == choice)
            {
                Player_Account *temp_acc = new Player_Account(name , balance);
                player->create_account(temp_acc);
                break;
            }
        }
        if (name != choice)
        {
            Player_Account *temp_acc = new Player_Account(choice);
            player->create_account(temp_acc);
        }
        
        temp << choice << " ";
        betting(player->get_account());
        player->write_name(choice);
        cout << "\n\nDealer\n\n";
        dealer->hidden_display();
        cout << "\n\n" << player->get_name() << "\n\n";
        player->display();
        cout << "\nCurrent Total : " << player->get_sum() << endl; 
        return true;
    }
    else
        return false;
}

bool hit_stand(Player *player , vector<Card> &deck)
{
    char choice;
    cout << "Do you want to hit or stand (Enter H/S) : ";
    cin >> choice;
    clear_buffer(); 
    choice = toupper(choice);
    while (choice != 'H' && choice != 'S')
    {
        cout << "\n\nInvalid Input\n\nEnter a valid Input (Enter H/S) : ";
        cin >> choice;
    }
    
    if (choice == 'H')
    {
        player->hit(deck);
        return true;
    }
    else
    {
        return false;
    }
}

bool natural_check(Player *player , Dealer *dealer)
{
    if (player->get_sum() == 21 && dealer->get_sum() == 21)
    {
        cout << "\n\nPress Enter to continue\n\n";
        cin.ignore();
        cout << "\n\nDealer\n\n";
        dealer->display();
        cout << "\nDealer Total : " << dealer->get_sum() << endl;
        cout << "\n\n" << player->get_name() << "\n\n";
        player->display();
        cout << "\nCurrent Total : " << player->get_sum() << endl; 
        cout << "\n\nSTAND-OFF\n\nIt's a tie\n\n";
        player->get_account()->deposit(player->get_account()->get_bet());
        return true;
    }
    else if(player->get_sum() == 21)
    {
        cout << "\n\nPress Enter to continue\n\n";
        cin.ignore();
        cout << "\n\nDealer\n\n";
        dealer->display();
        cout << "\nDealer Total : " << dealer->get_sum() << endl;
        cout << "\n\n" << player->get_name() << "\n\n";
        player->display();
        cout << "\nCurrent Total : " << player->get_sum() << endl; 
        cout << "\n\nBLACKJACK\n\nYou Win\n\n";
        player->get_account()->deposit(2*(player->get_account()->get_bet()));
        return true;
    }
    else if (dealer->get_sum() == 21)
    {
        cout << "\n\nPress Enter to continue\n\n";
        cin.ignore();
        cout << "\n\nDealer\n\n";
        dealer->display();
        cout << "\nDealer Total : " << dealer->get_sum() << endl;
        cout << "\n\n" << player->get_name() << "\n\n";
        player->display();
        cout << "\nCurrent Total : " << player->get_sum() << endl; 
        cout << "\n\nDealer has a natural\n\nYou Lose\n\n";
        player->get_account()->display_balance();
        return true;
    }
    else
        return false;
}

bool blackjack(Player *player , Dealer *dealer , vector<Card> &deck)
{
    while (hit_stand(player , deck))
    {
        cout << "\n\n";
        cout << "\n\nDealer\n\n";
        dealer->hidden_display();
        cout << "\n\n" << player->get_name() << "\n\n";
        player->display();
        cout << "\nCurrent Total : " << player->get_sum() << endl; 
        if (player->check_bust())
        {
            cout << "\n\nBUST\n\nYou Lose\n\n";
            player->get_account()->display_balance();
            return false;
        }
    }
    return true;
}

void dealer_play(Player *player , Dealer *dealer , vector<Card> &deck)
{
    display_cards(player , dealer);
    cout << "\n\nPress Enter to continue\n\n";
    cin.ignore();
    
    while (dealer->get_sum() < 17)
    { 
        dealer->hit(deck);
        display_cards(player , dealer);
        cout << "\n\nPress Enter to continue\n\n";
        cin.ignore();
    }
    if (dealer->get_sum() > 21)
    {
        cout << "\n\nDealer goes BUST\n\nYou Win\n\n";
        player->get_account()->deposit(2*(player->get_account()->get_bet()));
    }
    else if(*dealer < *player)
    {
        cout << "Your total is higher\n\nYou Win\n\n";
        player->get_account()->deposit(2*(player->get_account()->get_bet()));
    }
    else if (*player < *dealer)
    {
        cout << "Your total is lower\n\nYou Lose\n\n";
        player->get_account()->display_balance();
    }
    else
    {
        cout << "Both totals are equal\n\nIt's a tie\n\n";
        player->get_account()->deposit(player->get_account()->get_bet());
    }
}

bool another_round(vector<Card> &deck , Player *player , Dealer *dealer , ifstream &save_data , fstream &temp)
{
    cout << "\n\nPlay another round (Y/N) : "; 

    char choice;
    cin >> choice;
    choice = toupper(choice);

    if (choice == 'N')
    {
        temp << player->get_account()->get_balance();
        string name;
        int balance;
        save_data.clear();
        save_data.seekg (0, ios::beg);
        while(!save_data.eof())
        {
            save_data >> name >> balance;
            if(name == player->get_name())
                continue;
            else
                temp << "\n" << name << " " << balance;
        }

        cout << "\nSaving " << player->get_name() << "'s balance as " << player->get_account()->get_balance() << endl;
        cout << "\nThank you for playing BlackJack\nMade by Mylavarapu Sri Akhil\n\n" << endl;
        return false;
    }

    else
    {
        betting(player->get_account());
        reset_shuffle(deck);
        player->clear_hand();
        dealer->clear_hand();
        player->hit(deck);
        player->hit(deck);
        dealer->hit(deck);
        dealer->hit(deck);
        cout << "\n\nDealer\n\n";
        dealer->hidden_display();
        cout << "\n\n" << player->get_name() << "\n\n";
        player->display();
        cout << "\nCurrent Total : " << player->get_sum() << endl; 

        if(!natural_check(player , dealer)){
            if(blackjack(player , dealer , deck))
            {
                dealer_play(player , dealer , deck);
            }
        }
        return true;
    } 
}

int main()
{
    vector<Card> deck;
    reset_shuffle(deck);
    Player *player = new Player(deck);
    Dealer *dealer = new Dealer(deck);

    ifstream save_data {"save_file.txt"};
    fstream temp {"temp.txt" , ios::in | ios::out | ios::trunc};
    if(!save_data || !temp)
    {
        cout << "Failed in save file creation";
        return 0;
    }

    if (!game_start(deck , player , dealer , save_data , temp))
    {
        return 0;
    }
    
    if(!natural_check(player , dealer)){
        if(blackjack(player , dealer , deck))
        {
            dealer_play(player , dealer , deck);
        }
    }
    
    while (another_round(deck , player , dealer , save_data , temp)){}

    
    
    delete player;
    delete dealer;
    save_data.close();
    temp.close();

    remove("save_file.txt");
    rename("temp.txt" , "save_file.txt");
}
