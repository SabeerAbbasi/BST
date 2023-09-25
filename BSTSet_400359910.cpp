#include "BSTSet.h"
#include <iostream>
#include <vector>
using namespace std;

BSTSet::BSTSet()
{
	root = NULL;
}

BSTSet::BSTSet(const std::vector<int>& input) //Time complexity: O(n^2), Space complexity: O(n)
{
	// Initialize root to NULL
	root = NULL;

	// If the input vector is empty, return
	if (input.empty()) {
		return;
	}

	// Iterate through the input vector and add each element to the BST
	for (int i = 0; i < input.size(); i++) {
		// Create a new node with the current input value
		TNode* new_node = new TNode(input[i], NULL, NULL);

		// If the root is NULL, set the new node as the root
		if (root == NULL) {
			root = new_node;
		}
		else {
			// Otherwise, find the correct position for the new node in the BST
			TNode* current_node = root;
			while (true) {
				if (input[i] < current_node->element) {
					if (current_node->left == NULL) {
						current_node->left = new_node;
						break;
					}
					else {
						current_node = current_node->left;
					}
				}
				else if (input[i] > current_node->element) {
					if (current_node->right == NULL) {
						current_node->right = new_node;
						break;
					}
					else {
						current_node = current_node->right;
					}
				}
				else {
					// If the input value already exists in the BST, delete the new node and break out of the loop
					delete new_node;
					break;
				}
			}
		}
	}
}


void BSTSet::deleteSubtree(TNode*& node) { //Time complexity: O(n). Space complexity is O(n). This is because the deleteSubtree function is called recursively on each node in the BST exactly once.
	// Check if the node is not null
	if (node) {
		// Recursively delete the left subtree
		deleteSubtree(node->left);
		// Recursively delete the right subtree
		deleteSubtree(node->right);
		// Delete the node itself
		delete node;
	}
}

// Destructor for BSTSet
BSTSet::~BSTSet() {
	// Call the deleteSubtree function on the root node to delete the entire BST
	deleteSubtree(root);
}



bool BSTSet::isIn(int v) { //Time complexity: O(logn), Space Complexity: O(1) as only using a constant amount of memory for variables
	// If the tree is empty, return false
	if (root == NULL) {
		return false;
	}
	// Start at the root of the tree
	TNode* current = root;
	// Traverse the tree until we find the node with value v
	while (true) {
		// If we have reached a leaf node and not found v, it is not in the tree
		if (current == NULL) {
			return false;
		}
		// If the current node has value v, return true
		if (current->element == v) {
			return true;
		}
		// If v is less than the current node's value, move to the left child
		if (v < current->element) {
			current = current->left;
		}
		// If v is greater than the current node's value, move to the right child
		else {
			current = current->right;
		}
	}
}



void BSTSet::add(int v) // Time complexity: O(n), Space complexity is O(1)
{
	TNode* newNode = new TNode(v, NULL, NULL); // Create a new node to be inserted

	if (root == NULL) { // If tree is empty, new node becomes the root
		root = newNode;
		return;
	}

	TNode* current = root; // Start at the root
	while (true) {
		if (v < current->element) { // If the value to insert is less than the current node's value
			if (current->left == NULL) { // If the left child is empty, insert the new node here
				current->left = newNode;
				return;
			}
			else { // Otherwise, move left
				current = current->left;
			}
		}
		else if (v > current->element) { // If the value to insert is greater than the current node's value
			if (current->right == NULL) { // If the right child is empty, insert the new node here
				current->right = newNode;
				return;
			}
			else { // Otherwise, move right
				current = current->right;
			}
		}
		else { // If the value to insert is already in the tree, return
			return;
		}
	}
}


TNode* BSTSet::findMax(TNode*& root) { //Time complexity: O(n), Space complexity: O(n)
	// Check if the root node is null
	if (root == NULL) {
		return NULL;
	}
	// Check if the root node has a right child
	if (root->right != NULL) {
		// Recursively search for the maximum node in the right subtree
		return findMax(root->right);
	}
	// If the root node does not have a right child, it is the maximum node
	return root;
}

bool BSTSet::remove(int v, TNode*& ptr) //Time complexity: O(n), Space complexity: O(n)
{
	// If the current node is NULL, the value v is not in the tree, so return false
	if (!ptr)
	{
		return false;
	}

	// If the value v is less than the current node's value, search in the left subtree
	if (v < ptr->element)
	{
		return remove(v, ptr->left);
	}

	// If the value v is greater than the current node's value, search in the right subtree
	else if (v > ptr->element)
	{
		return remove(v, ptr->right);
	}

	// If the node to be removed has two children, replace its value with the maximum value in the left subtree and remove the node with that value
	else if (ptr->left != NULL && ptr->right != NULL)
	{
		ptr->element = findMax(ptr->left)->element;
		return remove(ptr->element, ptr->left);
	}

	// If the node to be removed has one or zero children, update the parent's reference to the child node and delete the node
	else {
		TNode* temp = ptr;
		ptr = (ptr->left) ? ptr->left : ptr->right;
		delete temp;
		return true;
	}
}

bool BSTSet::remove(int v)
{
	// Call the remove function with the root of the tree
	return remove(v, root);
}


// The Union function takes another BSTSet s and a pointer to a node in the current BSTSet
void BSTSet::Union(const BSTSet& s, TNode* ptr) //Time complexity: O(logn), Space complexity: O(logn)

{
	// Base case: if the current node is NULL, we return
	if (ptr == NULL) 
	{
		return;
	}

	// If the current node's element is not in the current BSTSet, we add it
	if (!isIn(ptr->element)) 
	{
		add(ptr->element);
	}

	// Recursively call the Union function on the left and right subtrees
	Union(s, ptr->left);
	Union(s, ptr->right);
}


void BSTSet::Union(const BSTSet& s)
{
	Union(s, s.root);
}

// Define a member function to find the intersection between two BSTSets
void BSTSet::intersection(const BSTSet& s) //Time complexity: O(n * m), where n is the number of nodes in the root of the current BSTSet and m is the number of nodes in the root of the input BSTSet s.
//space complexity is O(n)
{
	// Create a new BSTSet to store the intersection
	BSTSet* temp = new BSTSet();
	// Call the recursive intersection function with the root of the BSTSet to find the intersection
	intersection(s.root, temp);
	// Delete the current BSTSet's subtree
	deleteSubtree(root);
	// Set the root of the current BSTSet to the root of the intersection BSTSet
	root = temp->root;
}

// Define the recursive intersection function to find the intersection between two BSTSets
void BSTSet::intersection(TNode* ptr, BSTSet* temp)
{
	// If the current node is null, return
	if (!ptr) 
	{ 
		return; 
	}

	// If the current node's element is in the current BSTSet, add it to the intersection BSTSet
	if (isIn(ptr->element))
	{
		temp->add(ptr->element); 
	}

	// Recursively call the intersection function on the left subtree
	intersection(ptr->left, temp);
	// Recursively call the intersection function on the right subtree
	intersection(ptr->right, temp);
}

// Define the recursive difference function to find the difference between two BSTSets
void BSTSet::difference(TNode* ptr) //Time complexity: O(n), Space complexity: O(n)
{
	// If the current node is null, return
	if (ptr == NULL)
	{
		return; 
	}

	// Check if the element at the current node is in the BSTSet
	bool is_in = isIn(ptr->element);

	// Recursively call the difference function on the left subtree
	difference(ptr->left);

	// If the element at the current node is in the BSTSet, remove it
	if (is_in)
	{ 
		remove(ptr->element); 
	}

	// Recursively call the difference function on the right subtree
	difference(ptr->right);
}

// Define a member function to find the difference between two BSTSets
void BSTSet::difference(const BSTSet& s)
{
	// Call the recursive difference function on the root of the other BSTSet to find the difference
	difference(s.root);
}

// Define a member function to calculate the size of the BSTSet
int BSTSet::size()
{
	// Initialize a counter variable to keep track of the size
	int count = 0;
	// Call the recursive size function to calculate the size and update the count variable
	size(count, root);
	// Return the final count value
	return count;
}


int BSTSet::size(int& count, TNode* ptr) //Space and time complexity is O(n): This is because each node is visited exactly once during the recursive calls, and there are n nodes in total in the tree.
{
	// If the current node is null, return the current count
	if (ptr == NULL) { return count; }

	// Recursively call the size function on the left subtree
	size(count, ptr->left);
	// Recursively call the size function on the right subtree
	size(count, ptr->right);

	// Increment the count for the current node
	++count;

	// Return the updated count value
	return count;
}


int BSTSet::height(TNode* ptr)
{
	// Base case: if the node is null, return -1
	if (!ptr) 
	{
		return -1;
	}

	// Recursively calculate the heights of the left and right subtrees
	int r_height = height(ptr->right);
	int l_height = height(ptr->left);

	// Return the maximum height of the left and right subtrees plus one
	return max(r_height, l_height) + 1;
}

int BSTSet::height()
{
	return height(root); 
}


class MyStack
{
private:
	// an SNode contains a pointer to a TNode and a pointer to the next SNode
	class SNode {
	public:
		TNode* node;
		SNode* next;

		// constructor for creating a new SNode with a given TNode and next SNode
		SNode(SNode* n, TNode* v) {
			node = v;
			next = n;
		}
	};

	SNode* head;  // a pointer to the top SNode of the stack

public:
	// constructor for creating an empty stack
	MyStack() {
		head = NULL;
	}

	// returns true if the stack is empty, false otherwise
	bool isEmpty() {
		return(head == NULL);
	}

	// pops the top TNode off the stack, deletes the top SNode, and returns the TNode
	TNode* pop() {
		// if the stack is not empty
		if (head != NULL) {
			// create a temporary pointer to the top SNode, a pointer to the TNode at the top, and update the head to the next SNode
			SNode* temp = head;
			TNode* output = head->node;
			head = head->next;
			// delete the old top SNode and return the top TNode
			delete temp;
			return output;
		}
		else {
			// if the stack is empty, throw an exception
			throw std::underflow_error("stack underflow");
		}
	}

	// pushes a TNode onto the top of the stack
	void push(TNode* v) {
		// create a new SNode with the TNode and the current top SNode as its next, and update the head to point to the new top SNode
		head = new SNode(head, v);
	}

	// destructor for deleting all the SNodes in the stack
	~MyStack() {
		while (head != NULL) {
			SNode* temp = head;
			head = head->next;
			delete temp;
		}
	}
};


void BSTSet::printNonRec()
{
	// create a stack to store TNodes
	MyStack stack = MyStack();

	// start with the root node
	TNode* current = root;

	// while the stack is not empty or there are still nodes to traverse
	while (!(stack.isEmpty()) || current != NULL) {
		// if the current node is not null, push it and all its left children onto the stack
		if (current != NULL) {
			stack.push(current);
			current = current->left;
		}
		// if the current node is null, pop the top node off the stack, print its value, and set the current node to its right child
		else {
			current = stack.pop();
			cout << current->element << ",";
			current = current->right;
		}
	}
}


void BSTSet::printBSTSet()
{
	if (root == NULL)
		std::cout << "";
	else {
		printBSTSet(root);
	}
}

void BSTSet::printBSTSet(TNode* t)
{
	if (t != NULL) {
		printBSTSet(t->left);
		std::cout << t->element << ",";
		printBSTSet(t->right);
	}
}
