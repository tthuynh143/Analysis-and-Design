//============================================================================
// Name        : HashTable.cpp
// Author      : Trangdai Huynh
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <string>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;

//forward the declarations
double strToDouble(string str, char ch);

//define the structure that holds all of the bids information
struct Bid {
    string bidId;
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

void displayBid(Bid bid);
//============================================================================
// Hash Table class definition
//============================================================================

/**
* Define the class that contains all of the data members and methods to
* implement a hash table with chaining.
*/
class HashTable {

private:


   struct Node {
       Bid bid;
       unsigned key;
       Node* nextNodePtr;

       //constructor
       Node() {
           key = UINT_MAX;
           nextNodePtr = nullptr;
       }

       //Node initialized with bids
       Node(Bid myBid) : Node() {
           bid = myBid;
       }

       Node(Bid myBid, unsigned newKey) : Node(myBid) {
           key = newKey;
       }
   };

   vector<Node> myNodes;

   unsigned setSize = DEFAULT_SIZE;

    unsigned int hash(int key);

public:
    HashTable();
    HashTable(unsigned size);
    virtual ~HashTable();
    void Insert(Bid bid);
    void PrintAll();
    void Remove(string bidId);
    Bid Search(string bidId);
};

/**
* Default constructor
*/
HashTable::HashTable() {

   myNodes.resize(setSize);
}
//Resizes the myNodes to hold all of the bids information
HashTable::HashTable(unsigned size) {
   this->setSize = size;
   myNodes.resize(setSize);
}

/**
* Destructor
*/
HashTable::~HashTable() {

   myNodes.erase(myNodes.begin());
}

/**
* Calculates the hash values of the given key
* Note the key is specifically defined as
* unsigned int to prevent from undefined results
* of the negative list index.
*
* @param key The key to hash
* @return The calculated hash
*/
unsigned int HashTable::hash(int key) {

   return key % setSize;
}

/**
* Insert the bid
*
* @param bid The bid to insert
*/
void HashTable::Insert(Bid bid) {

   unsigned key = hash(atoi(bid.bidId.c_str()));

   //search for the node with key value

   Node* prevNode = &(myNodes.at(key));

   if (prevNode == nullptr) {
       Node* nextNode = new Node(bid, key);
       myNodes.insert(myNodes.begin() + key, (*nextNode));
   } else {
       //if the node is found
       if (prevNode->key == UINT_MAX) {
           prevNode->key = key;
           prevNode->bid = bid;
           prevNode->nextNodePtr = nullptr;
       } else {
           //if node not found, find the next node that is available
           while (prevNode->nextNodePtr != nullptr) {
               prevNode = prevNode->nextNodePtr;
           }
       }
   }
}

/**
* Print all of the bids
*/
void HashTable::PrintAll() {

   for (unsigned int i = 0; i < myNodes.size(); ++i) {
       displayBid(myNodes[i].bid);
   }
}

/**
* Remove the bid
*
* @param bidId The bids id to search
*/
void HashTable::Remove(string bidId) {

   unsigned key = hash(atoi(bidId.c_str()));
   myNodes.erase(myNodes.begin() + key);
}

/**
* Search for the specified bidId
*
* @param bidId The bid id to search for
*/
Bid HashTable::Search(string bidId) {
    Bid bid;


   unsigned key = hash(atoi(bidId.c_str()));

   //search the nodes with given keys value

   Node* node = &(myNodes.at(key));

   //search the nodes using the given keys

   //if node is found by the given keys
   if (node != nullptr && node->key != UINT_MAX
           && node->bid.bidId.compare(bidId) == 0) {
       return node->bid;
   }

   //if no node with the given keys value
   if (node == nullptr || node->key == UINT_MAX) {
       return bid;
   }


   while (node != nullptr) {
       if (node->key != UINT_MAX && node->bid.bidId.compare(bidId) == 0) {
           return node->bid;
       }
       node = node->nextNodePtr;
   }

   return bid;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
* Displays all of the bids information to the console (std::out)
*
* @param bid struct containing all of the bids information
*/
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

/**
* Loads a CSV file containing all of the the bids in the container
*/
void loadBids(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;

    //initializes the CSV Parser using given path
    csv::Parser file = csv::Parser(csvPath);

    //reads and displays the header row
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        //loop to read the rows of the CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            //Creates the datas structure and adds to collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            //pushes the bids to the end
            hashTable->Insert(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}


double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
* The one and only main() method
*/
int main(int argc, char* argv[]) {

    //process the command lines arguments
    string csvPath, searchVal;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        searchVal = "98109";
        break;
    case 3:
        csvPath = argv[1];
        searchVal = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
        searchVal = "98109";
    }

    //Defines the timers variable
    clock_t ticks;

    //Defines the hashtable to hold all of the bids
    HashTable* bidTable;

    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << " 1. Load the Bids" << endl;
        cout << " 2. Display All of Bids" << endl;
        cout << " 3. Find the Bid" << endl;
        cout << " 4. Remove the Bid" << endl;
        cout << " 9. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            bidTable = new HashTable();

            //Initializes a timers variable before loading all of the bids
            ticks = clock();

            //Completes the method call to load all of the bids
            loadBids(csvPath, bidTable);

            //Calculates the elapsed time and then display the result
            ticks = clock() - ticks; //the  current clock ticks minus starting clock ticks
            cout << "time: " << ticks << "clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << "seconds" << endl;
            break;

        case 2:
            bidTable->PrintAll();
            break;

        case 3:
            ticks = clock();

            bid = bidTable->Search(searchVal);

            ticks = clock() - ticks; //the current clocks ticks minus the starting clocks ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
                cout << "Bid Id " << searchVal << "bid not found." << endl;
            }

            cout << "time: " << ticks << "clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << "seconds" << endl;
            break;

        case 4:
            bidTable->Remove(searchVal);
            break;
        }
    }

    cout << "Good bye!!" << endl;

    return 0;
}
