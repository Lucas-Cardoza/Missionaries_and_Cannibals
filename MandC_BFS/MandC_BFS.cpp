//********************************************************************
//
// Student: Lucas Cardoza
// Course: CSC 540 - Introduction to Artificial Intelligence
// Assignment 1: Missionaries and Cannibals
// Due Date: Sunday, January 31, 2021, 11:59 PM
// Instructor: Dr. Siming Liu
//
//********************************************************************


// This program is built around the idea that only one side (starting side) 
// of the river needs to be calculated to solve this problem. The legend
// for the tuple representing the various states is as follows:
// Missionaries = M. Cannibals = C
// (Number of M, number of C, boat)
//				****EXAMPLE****
// Starting state: 
// Left side 3, 3, 1	Right side 0, 0, 0  
// If two C were to cross the river, 
// the resulting state would be:
// Left side 3, 1, 0	Right side 0, 2, 1  


#include <vector>
#include <queue>
#include <list>
#include <iostream>


// Node structure
struct Node
{
	int depth;			// Depth of node in tree
	int state[3];		// The state of the node
	Node* children[5];	// The children of node
};


// Function prototypes
bool acceptState(int state[3]);
void bfs();
Node* makeNode();


// Main function
int main()
{
	std::string go;		// Variable for user input to start or quit program
	bool quit = false;	// While loop controller variable

	//loop for program menu
	while (!quit)
	{
		std::cout << "\npress 'p' to run program or 'q to quit. ";
		std::cin >> go;

		if (go.length() > 1)
		{
			go = '0'; // set user input to an invalid input to trigger an invalid input response
		}

		switch (go[0])
		{
		case 'p':
		{
			bfs();	// Run BFS on missionaries and cannibals problem
			break;
		}
		case 'q':
		{
			quit = true;
			break;
		}
		default:
		{
			std::cout << "\n" << "input is not a valid option." << std::endl;
		}
		}
	}
}


// Function to run BFS on Missionaries and Cannibals problem
void bfs()
{
	// All possible actions that can be taken by M or C
	int actions[5][3] = { { 1, 0, 1 }, { 2, 0, 1 }, { 0, 1, 1 }, { 0, 2, 1 }, { 1, 1, 1 } };

	// Starting state of problem
	int startState[3] = { 3, 3, 1 };

	// The goal state of problem
	int goalState[3] = { 0, 0, 0 };

	int count{ 0 };	// Counter for nodes looked at
	Node* rootNode;	// Create new node pointer (root in this case)
	rootNode = new Node;

	// Initialize node values
	for (int i = 0; i < 3; i++)
	{
		rootNode->state[i] = startState[i];
	}
	for (int i = 0; i < 5; i++)
	{
		rootNode->children[i] = nullptr;
	}
	rootNode->depth = 0;

	std::queue<Node> fronteir;	// Queue to hold acceptable states for expansion
	fronteir.push(*rootNode);	// Place root node pointer into queue
	std::list<Node> explored;	// List to hold explored states
	bool flag = true;

	// BFS loop
	while (!fronteir.empty() && flag)
	{
		Node currentNode = fronteir.front();	// Retrieve first node from queue
		std::cout << "\n	   Expanding " << currentNode.state[0] << ", " << currentNode.state[1] << ", " << currentNode.state[2] << "\n" << std::endl;
		fronteir.pop();						// Pop node from queue
		explored.push_back(currentNode);	// Place node into explored list
		int x = explored.size();			// Resize container to hold new states
		explored.resize(x);

		// Depth determines whether or not to subtract the actions or add them
		// to the current state values to move the people to or away from
		// the starting side of the river.
		int mult = 0;
		if (currentNode.depth % 2 == 0)
		{
			mult = -1;
		}
		else
		{
			mult = 1;
		}
		
		for (int i = 0; i < 5; i++)
		{
			if (!flag)
			{
				break;
			}
			Node* newNode = makeNode();	// Make a new node pointer for new node to hold change state

			for (int j = 0; j < 3; j++)
			{
				newNode->state[j] = currentNode.state[j] + mult * actions[i][j];	// Preform actions on current state
			}

			// Get values for other side of river
			int M{ 3 }, C{ 3 }, B{ 1 };
			M = M - newNode->state[0];
			C = C - newNode->state[1];
			B = B - newNode->state[2];

			count++;	// Add to the nodes searched counter

			std::cout << "Looking at: Left side " << newNode->state[0] << ", " << newNode->state[1] << ", " << newNode->state[2] <<
				"	Right side: " << M << ", " << C << ", " << B << std::endl;

			if (acceptState(newNode->state))	// Check to see if state is a valid state
			{
				bool foundInF = false, foundInE = false;	// Variables for if state is found in explored list or frontier queue

				// Cycle through whole queue to check if new state is already in there
				for (int i = 0; i < fronteir.size(); i++)
				{
					Node temp;					// Create temp node to hold nodes from queue
					temp = fronteir.front();	// Get node from front of queue
					fronteir.pop();				// Remove node from queue

					// Check if the states are the same
					if (temp.state[0] == newNode->state[0]
						&& temp.state[1] == newNode->state[1]
						&& temp.state[2] == newNode->state[2])
					{
						foundInF = true;	// If new state was found in queue
					}

					fronteir.push(temp);	// Replace comparison node back into queue
				}

				// Cycle through whole list to check if new state is already in there
				if (!explored.empty())			
				{
					for (int i = 0; i < explored.size(); i++)
					{
						Node temp;				 // Create temp node to hold nodes from list
						temp = explored.front(); // Get node from front of list
						explored.pop_front();	 // Remove node from list

						// Check if the states are the same
						if (temp.state[0] == newNode->state[0]
							&& temp.state[1] == newNode->state[1]
							&& temp.state[2] == newNode->state[2])
						{
							foundInE = true;	// If new state was found in list
						}

						explored.push_back(temp);	// Replace comparison node back into list
					}
				}
				
				// If state was not found
				if (!foundInF && !foundInE)
				{
					int k = 0;
					newNode->depth = currentNode.depth + 1;	// Chang depth of node
					for (k; k < 5; k++)
					{
						if (currentNode.children[k] == nullptr)
						{
							currentNode.children[k] = newNode;	// Add node to tree
							break;
						}
					}

					// Check if node state is goal state
					if (goalState[0] == newNode->state[0]
						&& goalState[1] == newNode->state[1]
						&& goalState[2] == newNode->state[2])
					{
						std::cout << "\nLeft side: " << newNode->state[0] << ", " << newNode->state[1] << ", " << newNode->state[2] <<
							"	Right side: " << M << ", " << C << ", " << B << std::endl;
						std::cout << "	****** GOAL STATE!! ******\n" << std::endl;
						//fronteir.push(*currentNode.children[k]);	// Place node into queue to be explored later
						flag = false;
					}
					else
					{
						std::cout << "\nLeft side: " << newNode->state[0] << ", " << newNode->state[1] << ", " << newNode->state[2] <<
							"	Right side: " << M << ", " << C << ", " << B << std::endl;
						std::cout << "	**** IS A SAFE STATE **** \n" << std::endl;
						fronteir.push(*currentNode.children[k]);	// Place node into queue to be explored later
					}
				}

				// Delete node pointer
				newNode = nullptr;
				delete newNode;
			}
		}
	}

	// Resize list container
	int x = explored.size();
	explored.resize(x + 1);

	std::cout << "Total of " << count << " node states searched." << std::endl;
	std::cout << "Total of " << explored.size() << " node states explored. " << std::endl;
	std::cout << "\nNode states were explored in this order: " << std::endl;

	// Print list of all explored states
	for (int i = 0; i <= x; i++)
	{
		Node temp = explored.front();
		std::cout << temp.state[0] << ", " << temp.state[1] << ", " << temp.state[2] << std::endl;
		explored.pop_front();
	}

	// Delete root node pointer
	rootNode = nullptr;
	delete rootNode;

	std::cout << "\nProgram complete. " << std::endl;
}


// Function to find if the state is an acceptable state
bool acceptState(int state[3])
{
	// Check if a state is invalid
	if (((state[0] < state[1]) && (state[0] != 0))
		|| ((state[0] > state[1]) && (state[0] != 3))
		|| ((state[0] > 3))
		|| ((state[1] > 3))
		|| ((state[0] < 0))
		|| ((state[1] < 0)))
	{
		return false;
	}

	return true;
}


// Function to create a new node
Node* makeNode()
{
	// New node pointer
	Node* newNode;
	newNode = new Node;

	// Initialize new node values
	for (int a = 0; a < 3; a++)
	{
		newNode->state[a] = 0;
	}
	for (int a = 0; a < 5; a++)
	{
		newNode->children[a] = nullptr;
	}

	newNode->depth = 0;

	return newNode;
}





