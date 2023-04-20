//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Trangdai Huynh
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <time.h>
#include <string> // atoi
#include <algorithm>

#include <climits>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

//forward the declarations
double strToDouble(string str, char ch);

//define the structure to hold all the bid information
struct Bid {
    string bidId; //a unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};


struct Node {
	Bid bid;
	Node* right;
	Node* left;

	Node() {
		right = left = nullptr;
	}

	Node(Bid bid) : Node() {
		this->bid = bid;
	}

};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define the class containing all the data members and the methods to
 * implement the binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Bid bid);
    void inOrder(Node* node);
    Node* removeNode(Node* node, string bidId);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Bid bid);
    void Remove(string bidId);
    Bid Search(string bidId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    //initialize the housekeeping variables
	root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    //loop from root deleting every node
}

/**
 * Track the tree in order
 */
void BinarySearchTree::InOrder() {
	this->inOrder(root);
}



Node* BinarySearchTree::removeNode(Node *node, string bidId) {
	if (node == nullptr) {
		return node;
	}

	if(bidId.compare(node->bid.bidId) < 0) {
		node->left = removeNode(node->left, bidId);
	} else if (bidId.compare(node->bid.bidId) > 0) {
		node->right = removeNode(node->right, bidId);
	} else {
		//if no children simply delete  node
		if (node->left == nullptr && node->right == nullptr) {
			delete node;
			node = nullptr;
		}
		//If there is a left children remove node
		else if (node->left != nullptr && node->right == nullptr) {
			Node* tempNode = node;
			node = node->left;
			delete tempNode;
			// If there is a right children remove node
		} else if (node->left == nullptr && node->right != nullptr) {
			Node* tempNode = node;
			node = node->right;
			delete tempNode;
		} else {

			Node* temp = node->right;
			while(temp->left == nullptr) {
				temp = temp->left;
			}
			node->bid = temp->bid;
			node->right = removeNode(node->right, temp->bid.bidId);
		}
	}
	return node;
}


/**
 * Insert the bids
 */
void BinarySearchTree::Insert(Bid bid) {

	//If not any existing node is in the tree
	if(root == nullptr) {
		root = new Node(bid);
	} else {
		//If there is an existing add it to the root
		this->addNode(root, bid);
	}
}

/**
 * Remove the bids
 */
void BinarySearchTree::Remove(string bidId) {

	this->removeNode(root, bidId);
}

/**
 * Search for the bids
 */
Bid BinarySearchTree::Search(string bidId) {

	//begin from the root

	Node* current = root;

	while(current != nullptr) {
		if(current->bid.bidId.compare(bidId) == 0) {
			return current->bid;
		}
		//traverse left if the bid is smaller than the current
		if(bidId.compare(current->bid.bidId) < 0) {
			current = current->left;
		} else {
			current = current->right;
		}
	}

	Bid bid;
    return bid;
}

/**
 * Add the bids to the some node
 *
 * @param node Current node in tree
 * @param bid Bids to be added
 */
void BinarySearchTree::addNode(Node* node, Bid bid) {

	//add to right if the bids bigger than the node
	if (node->bid.bidId.compare(bid.bidId) > 0) {
		if(node->left == nullptr) {
			node->left = new Node(bid);
		} else {
			this->addNode(node->left, bid);
		}
	} else {
		//add to left if the bids smaller than the node
		if(node->right == nullptr) {
			node->right = new Node(bid);
		} else {
			this->addNode(node->right, bid);
		}
	}
}
void BinarySearchTree::inOrder(Node* node) {
	// Recursively track the tree to the right and left and print it as it traverses
	if(node != nullptr) {
		inOrder(node->left);
	    cout << node->bid.bidId << ": " << node->bid.title << " | " << node->bid.amount << " | "
	            << node->bid.fund << endl;
		inOrder(node->right);
	}
}
//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    //initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    //read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // oop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            //Creates a data structure and adds to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            //push the bids to end
            bst->Insert(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * @param ch The character to the strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    //process the command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98109";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
        bidKey = "98109";
    }

    //Defines the timers variables
    clock_t ticks;

    //Defines the binary search tree to hold all of the bids
    BinarySearchTree* bst;

    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load the Bids" << endl;
        cout << "  2. Display All the Bids" << endl;
        cout << "  3. Find the Bid" << endl;
        cout << "  4. Remove the Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            bst = new BinarySearchTree();

            //Initializes the timers variable before loading all of the bids
            ticks = clock();

            //Complete method then call to load all of the bids
            loadBids(csvPath, bst);

            //cout <<bst->Size() <<"bids read"<< endl;

            //Calculates the elapsed time then displays the results
            ticks = clock() - ticks; //the current clock ticks minus the starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << "seconds" << endl;
            break;

        case 2:
            bst->InOrder();
            break;

        case 3:
            ticks = clock();

            bid = bst->Search(bidKey);

            ticks = clock() - ticks; //the current clock ticks minus the starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
            	cout << "Bid Id " << bidKey << "bid not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << "seconds" << endl;

            break;

        case 4:
            bst->Remove(bidKey);
            break;
        }
    }

    cout << "Good bye!!" << endl;

	return 0;
}
