#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

using namespace std;
// Object used to hold flight data

class Flight
{
private:
    string uniqueID;
    string year, month, day;
    string departure, arrival, airline;
    bool delayed;
    bool cancelled;

public:
    Flight(string _year, string _month, string _day, string _departure, string _arrival, string _airline, bool _delayed, bool _cancelled, string uniqueID);
	Flight();

    void printFlight();
    string GetUniqueID();
};

Flight::Flight() {
	year = "null";
	month = "null";
	day = "null";
	departure = "null";
	arrival = "null";
	airline = "null";
	delayed = false;
	cancelled = false;
	uniqueID = "null";
}

//Flight Constructor
Flight::Flight(string _year, string _month, string _day, string _departure, string _arrival, string _airline, bool _delayed, bool _cancelled, string _uniqueID) {
    year = _year;
    month = _month;
    day = _day;
    departure = _departure;
    arrival = _arrival;
    airline = _airline;
    delayed = _delayed;
    cancelled = _cancelled;
    uniqueID = _uniqueID;
}

void Flight::printFlight()
{
    cout << year << " " << month << " " << day << " " << departure << " " << arrival;
    cout << " " << airline << " " << delayed << " " << cancelled << " " << uniqueID << endl;
}

string Flight::GetUniqueID() 
{
    return(uniqueID);
}

// Object used to hold airline data
class Airline
{
private:
    string name;
    int numFlights, numFlightsDelayed, numFlightsCancelled;
    double pctDelayed, pctCancelled;

public:
    Airline();
    Airline(string _name);
    // REMINDER: only consider positive flight delays
};

//Airline Constructor
Airline::Airline(string _name) {
    name = _name;
    numFlights = 0;
    numFlightsDelayed = 0;
    numFlightsCancelled = 0;
    pctDelayed = 0.0;
    pctCancelled = 0.0;
}





// DATA STRUCTURES
// Red Black Tree
// Data structure that represents a node in the tree
struct Node {
	Flight flight; // holds the key
	Node* parent; // pointer to the parent
	Node* left; // pointer to left child
	Node* right; // pointer to right child
	int color; // 1 -> Red, 0 -> Black
	
	Node(Flight _flight, Node* _parent, Node* _left, Node* _right, int _color) :flight(_flight), parent(_parent), left(_left), right(_right), color(_color) {};
	Node() {
		flight = Flight();
		parent = nullptr;
		left = nullptr;
		right = nullptr;
		color = 0;
	}
};



typedef Node* NodePtr;

// class RBTree implements the operations in Red Black Tree
class RBTree {
private:
	NodePtr root;
	NodePtr TNULL;

	// initializes the nodes with appropirate values
	// all the pointers are set to point to the null pointer
	void initializeNULLNode(NodePtr node, NodePtr parent) {
		Flight trashFlight = Flight(" ", " ", " ", " ", " ", " ", false, false, " ");
		node->flight = trashFlight;
		node->parent = parent;
		node->left = nullptr;
		node->right = nullptr;
		node->color = 0;
	}

	void preOrderHelper(NodePtr node) {
		if (node != TNULL) {
			cout << node->flight.GetUniqueID() << " ";
			preOrderHelper(node->left);
			preOrderHelper(node->right);
		}
	}

	void inOrderHelper(NodePtr node) {
		if (node != TNULL) {
			inOrderHelper(node->left);
			cout << node->flight.GetUniqueID() << " ";
			inOrderHelper(node->right);
		}
	}

	void postOrderHelper(NodePtr node) {
		if (node != TNULL) {
			postOrderHelper(node->left);
			postOrderHelper(node->right);
			cout << node->flight.GetUniqueID() << " ";
		}
	}

	NodePtr searchTreeHelper(NodePtr node, Flight key) {
		if (node == TNULL || key.GetUniqueID() == node->flight.GetUniqueID()) {
			return node;
		}

		if (key.GetUniqueID() < node->flight.GetUniqueID()) {
			return searchTreeHelper(node->left, key);
		}
		return searchTreeHelper(node->right, key);
	}

	// fix the rb tree modified by the delete operation
	void fixDelete(NodePtr x) {
		NodePtr s;
		while (x != root && x->color == 0) {
			if (x == x->parent->left) {
				s = x->parent->right;
				if (s->color == 1) {
					// case 3.1
					s->color = 0;
					x->parent->color = 1;
					leftRotate(x->parent);
					s = x->parent->right;
				}

				if (s->left->color == 0 && s->right->color == 0) {
					// case 3.2
					s->color = 1;
					x = x->parent;
				}
				else {
					if (s->right->color == 0) {
						// case 3.3
						s->left->color = 0;
						s->color = 1;
						rightRotate(s);
						s = x->parent->right;
					}

					// case 3.4
					s->color = x->parent->color;
					x->parent->color = 0;
					s->right->color = 0;
					leftRotate(x->parent);
					x = root;
				}
			}
			else {
				s = x->parent->left;
				if (s->color == 1) {
					// case 3.1
					s->color = 0;
					x->parent->color = 1;
					rightRotate(x->parent);
					s = x->parent->left;
				}

				if (s->right->color == 0 && s->right->color == 0) {
					// case 3.2
					s->color = 1;
					x = x->parent;
				}
				else {
					if (s->left->color == 0) {
						// case 3.3
						s->right->color = 0;
						s->color = 1;
						leftRotate(s);
						s = x->parent->left;
					}

					// case 3.4
					s->color = x->parent->color;
					x->parent->color = 0;
					s->left->color = 0;
					rightRotate(x->parent);
					x = root;
				}
			}
		}
		x->color = 0;
	}


	void rbTransplant(NodePtr u, NodePtr v) {
		if (u->parent == nullptr) {
			root = v;
		}
		else if (u == u->parent->left) {
			u->parent->left = v;
		}
		else {
			u->parent->right = v;
		}
		v->parent = u->parent;
	}

	void deleteNodeHelper(NodePtr node, Flight key) {
		// find the node containing key
		NodePtr z = TNULL;
		NodePtr x, y;
		while (node != TNULL) {
			if (node->flight.GetUniqueID() == key.GetUniqueID()) {
				z = node;
			}

			if (node->flight.GetUniqueID() <= key.GetUniqueID()) {
				node = node->right;
			}
			else {
				node = node->left;
			}
		}

		if (z == TNULL) {
			cout << "Couldn't find key in the tree" << endl;
			return;
		}

		y = z;
		int y_original_color = y->color;
		if (z->left == TNULL) {
			x = z->right;
			rbTransplant(z, z->right);
		}
		else if (z->right == TNULL) {
			x = z->left;
			rbTransplant(z, z->left);
		}
		else {
			y = minimum(z->right);
			y_original_color = y->color;
			x = y->right;
			if (y->parent == z) {
				x->parent = y;
			}
			else {
				rbTransplant(y, y->right);
				y->right = z->right;
				y->right->parent = y;
			}

			rbTransplant(z, y);
			y->left = z->left;
			y->left->parent = y;
			y->color = z->color;
		}
		delete z;
		if (y_original_color == 0) {
			fixDelete(x);
		}
	}

	// fix the red-black tree
	void fixInsert(NodePtr k) {
		NodePtr u;
		while (k->parent->color == 1) {
			if (k->parent == k->parent->parent->right) {
				u = k->parent->parent->left; // uncle
				if (u->color == 1) {
					// case 3.1
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				}
				else {
					if (k == k->parent->left) {
						// case 3.2.2
						k = k->parent;
						rightRotate(k);
					}
					// case 3.2.1
					k->parent->color = 0;
					k->parent->parent->color = 1;
					leftRotate(k->parent->parent);
				}
			}
			else {
				u = k->parent->parent->right; // uncle

				if (u->color == 1) {
					// mirror case 3.1
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				}
				else {
					if (k == k->parent->right) {
						// mirror case 3.2.2
						k = k->parent;
						leftRotate(k);
					}
					// mirror case 3.2.1
					k->parent->color = 0;
					k->parent->parent->color = 1;
					rightRotate(k->parent->parent);
				}
			}
			if (k == root) {
				break;
			}
		}
		root->color = 0;
	}

	void printHelper(NodePtr root, string indent, bool last) {
		// print the tree structure on the screen
		if (root != TNULL) {
			cout << indent;
			if (last) {
				cout << "R----";
				indent += "     ";
			}
			else {
				cout << "L----";
				indent += "|    ";
			}

			string sColor = root->color ? "RED" : "BLACK";
			cout << root->flight.GetUniqueID() << "(" << sColor << ")" << endl;
			printHelper(root->left, indent, false);
			printHelper(root->right, indent, true);
		}
		// cout<<root->left->data<<endl;
	}

public:
	RBTree() {
		Flight flight();
		TNULL = new Node;
		TNULL->color = 0;
		TNULL->left = nullptr;
		TNULL->right = nullptr;
		root = TNULL;
	}

	// Pre-Order traversal
	// Node->Left Subtree->Right Subtree
	void preorder() {
		preOrderHelper(this->root);
	}

	// In-Order traversal
	// Left Subtree -> Node -> Right Subtree
	void inorder() {
		inOrderHelper(this->root);
	}

	// Post-Order traversal
	// Left Subtree -> Right Subtree -> Node
	void postorder() {
		postOrderHelper(this->root);
	}

	// search the tree for the key k
	// and return the corresponding node
	NodePtr searchTree(Flight k) {
		return searchTreeHelper(this->root, k);
	}

	// find the node with the minimum key
	NodePtr minimum(NodePtr node) {
		while (node->left != TNULL) {
			node = node->left;
		}
		return node;
	}

	// find the node with the maximum key
	NodePtr maximum(NodePtr node) {
		while (node->right != TNULL) {
			node = node->right;
		}
		return node;
	}

	// find the successor of a given node
	NodePtr successor(NodePtr x) {
		// if the right subtree is not null,
		// the successor is the leftmost node in the
		// right subtree
		if (x->right != TNULL) {
			return minimum(x->right);
		}

		// else it is the lowest ancestor of x whose
		// left child is also an ancestor of x.
		NodePtr y = x->parent;
		while (y != TNULL && x == y->right) {
			x = y;
			y = y->parent;
		}
		return y;
	}

	// find the predecessor of a given node
	NodePtr predecessor(NodePtr x) {
		// if the left subtree is not null,
		// the predecessor is the rightmost node in the 
		// left subtree
		if (x->left != TNULL) {
			return maximum(x->left);
		}

		NodePtr y = x->parent;
		while (y != TNULL && x == y->left) {
			x = y;
			y = y->parent;
		}

		return y;
	}

	// rotate left at node x
	void leftRotate(NodePtr x) {
		NodePtr y = x->right;
		x->right = y->left;
		if (y->left != TNULL) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		}
		else if (x == x->parent->left) {
			x->parent->left = y;
		}
		else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

	// rotate right at node x
	void rightRotate(NodePtr x) {
		NodePtr y = x->left;
		x->left = y->right;
		if (y->right != TNULL) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		}
		else if (x == x->parent->right) {
			x->parent->right = y;
		}
		else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}

	// insert the key to the tree in its appropriate position
	// and fix the tree
	void insert(Flight flight) {
		// Ordinary Binary Search Insertion
		NodePtr node = new Node;
		node->parent = nullptr;
		node->flight = flight;
		node->left = TNULL;
		node->right = TNULL;
		node->color = 1; // new node must be red

		NodePtr y = nullptr;
		NodePtr x = this->root;

		while (x != TNULL) {
			y = x;
			if (node->flight.GetUniqueID() < x->flight.GetUniqueID()) {
				x = x->left;
			}
			else {
				x = x->right;
			}
		}

		// y is parent of x
		node->parent = y;
		if (y == nullptr) {
			root = node;
		}
		else if (node->flight.GetUniqueID() < y->flight.GetUniqueID()) {
			y->left = node;
		}
		else {
			y->right = node;
		}

		// if new node is a root node, simply return
		if (node->parent == nullptr) {
			node->color = 0;
			return;
		}

		// if the grandparent is null, simply return
		if (node->parent->parent == nullptr) {
			return;
		}

		// Fix the tree
		fixInsert(node);
	}

	NodePtr getRoot() {
		return this->root;
	}

	// delete the node from the tree
	void deleteNode(Flight flight) {
		deleteNodeHelper(this->root, flight);
	}

	// print the tree structure on the screen
	void prettyPrint() {
		if (root) {
			printHelper(this->root, "", true);
		}
	}

};
















int main() {

    // Read in Excel sheets to gather flight data and place in containers
    // Loop will create Flight objects to be inserted into Red-Black tree/other container
    ifstream data;

    data.open("Project_3_Dataset_Origin.csv");

    if (data.is_open())
    {
        cout << "test";
        // Read in first row which is column headers
        string firstLine;
        data >> firstLine;

        // Read in flight data
        string trash1, trash2;
        string year, month, day, airline, origin, destination;

        // Transfer to booleans
        string departDelay, arriveDelay, cancellation;

        // INITILIZE DATA STRUCTURES
        vector<Flight> flights;
		RBTree rbTree;

        int uniqueCounter = 0;

        while (!data.eof())
        {
            getline(data, year, ',');
            getline(data, month, ',');
            getline(data, day, ',');
            getline(data, airline, ',');
            getline(data, origin, ',');
            getline(data, destination, ',');
            getline(data, departDelay, ',');
            getline(data, arriveDelay, ',');
            getline(data, trash1, ',');
            getline(data, cancellation, ',');
            getline(data, trash2);

            string uniqueID;
            uniqueID = airline + to_string(uniqueCounter);

            // Create flight object based on strings
            // Check if flight is cancelled before adding delays
            if (cancellation == "0")
            {
                bool delayed, cancelled;
                cancelled = false;

                if (stoi(departDelay) > 0 && origin == "ORD")
                    delayed = true;
                else if (stoi(departDelay) <= 0 && origin == "ORD")
                    delayed = false;
                else if (stoi(arriveDelay) > 0 && destination == "ORD")
                    delayed = true;
                else if (stoi(arriveDelay) <= 0 && destination == "ORD")
                    delayed = false;


                Flight currentFlight = Flight(year, month, day, origin, destination, airline, delayed, cancelled, uniqueID);

                // INSERT INTO DATA STRUCTURE
                flights.push_back(currentFlight);
				rbTree.insert(currentFlight);
            }

            else
            {
                Flight currentFlight = Flight(year, month, day, origin, destination, airline, false, true, uniqueID);

                // INSERT INTO DATA STRUCTURE
                flights.push_back(currentFlight);
            }
            uniqueCounter++;
        }
		rbTree.inorder();
    }

    return 0;
}