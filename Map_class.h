#pragma once
#include <iostream>
#include <stdexcept>

using namespace std;

enum COLOR { black, red }; //enumeration black = 0, red = 1

template<class T, typename T1>
class Map
{
	struct Node
	{
		Node* parent; //pointer to the father of the node
		Node* left, *right; //pointers to the right and left sons of the node
		COLOR color; //node color
		T data; //node data field
		T1 key; //noe key field

		Node(T data_1, T1 key_1, Node* par_1, COLOR color_1 = red) : parent(par_1), left(nullptr), right(nullptr), color(color_1), data(data_1), key(key_1) {}
	};
	Node* root; //pointer to the root of map
	void RotateLeft(Node* node); //left rotation
	void RotateRight(Node* node); //right rotation
	void VertexBalancing(Node* new_elem, Node* father, Node* gr_father); //balancing after inserting a new item 
	bool RightOrLeft(Node* node); //determines whether the node is a left or right child
	void GoToMaxLeft(Node* node); //looking for the maximum node from the left subtree (for remove)
	void FixUp(Node* son, Node* nodePar, Node* uncle); //recursive method for fix up changes after node removing
	void DeleteRed(Node* node); //delete red node
	void DeleteBlack(Node* node); //delete black node
public:
	Map() : root(nullptr) {} //map constructor
	~Map() { clear(root); } //map destructor
	void insert(T data_n, T1 key_n); //inserting node
	void clear(Node* ptr); //recursive delete map
	void get_keys(Node* ptr); //returns a list of keys
	void get_values(Node* ptr); //returns a list of data
	void remove(T1 key_n); //remove node by the key
	void replaceData( Map<T, T1>& NEW_OBJ, Node* ptr);
	
	friend ostream& operator << (ostream& output_stream, Node* pointer) //output statement overload for node pointer
	{
		if (pointer != nullptr) { //if pointer exists
			output_stream << "Data feild: " << pointer->data << endl; //output to stream date field
			output_stream << "Key: " << pointer->key << endl; //output to stream key field
			output_stream << "Node color: " << (pointer->color ? "red" : "black") << endl; //output to stream color field
		}
		else //if pointer is nullptr
			output_stream << "Couldn't find node with such index!" << endl;
		return output_stream; //return output stream
	}

	Node* find(T1 key_n) //finds a node by key
	{
		Node* temp = root; //make new pointer on root
		if (temp != nullptr) { //if tree exists
			//cycle for search temp with such key
			while ((temp->right != nullptr && key_n > temp->key) || (temp->left != nullptr && key_n < temp->key)) {
				key_n > temp->key ? temp = temp->right : temp = temp->left;
			}
			if (temp->key == key_n) //if the keys match
				return temp; //return pointer to node
		}
		return nullptr; //if the tree doesn't exist or the node with such a key isn't found, returns nullptr
	}

	Node* GetRoot(){ //method for getting the root of a tree
		return(root); //returns the pointer to the root
	}
};

template <class T, typename T1>
string GetCodedString(Map<T, T1>& OBJ, string str)
{
	string coded;
	for (int i = 0; i < str.length(); i++) {
		string tmp;
		tmp.push_back(str[i]);
		if (OBJ.find(tmp) != nullptr)
			coded += OBJ.find(tmp)->data;
	}
	return coded;
}

template <class T, typename T1>
string GetDecodedString(Map<T, T1> & OBJ, string str)
{
	string tmp_code = "";
	string result;
	for (int i = 0; i < str.length(); ++i)
	{
		tmp_code += str[i];
		if (OBJ.find(tmp_code))
		{
			result += OBJ.find(tmp_code)->data;
			tmp_code = "";
		}
	}
	return result;
}

template <class T, typename T1>
void Map<T, T1> :: replaceData(Map<T, T1>& NEW_OBJ, Node* ptr)
{
	if (ptr != nullptr) { //if the node exists
		Node* leftptr = ptr->left; //transition to the left branch of a tree
		Node* rightptr = ptr->right; //transition to the right branch of a tree
		NEW_OBJ.insert(ptr->key, ptr->data);
		replaceData(NEW_OBJ, ptr->left); //recursive call for the left branch
		replaceData(NEW_OBJ, ptr->right); //recursive call for the right branch
	}
}

template<class T, typename T1>
bool Map<T, T1>::RightOrLeft(Node* node) { //determines whether the node is a left or right child
	if(node->parent != nullptr)
		return(node->parent->right == node); //if the node is right, then output true
}

template<class T, typename T1>
void Map<T, T1>::RotateLeft(Node* node) //left rotation
{
	Node* node_right = node->right; //writes the right child of node
	bool ifRoot = (node == root ? true : false); //is the node root?
	if (!ifRoot) { //if the node isn't root
		if (RightOrLeft(node)) //if the node is right, then
			node->parent->right = node_right; //the right son of the node becomes the son of the father of the node
		else
			node->parent->left = node_right; //the right son of the node becomes the son of the father of the node
	}
	node_right->parent = node->parent; //do the parent node of his son his father
	node->right = node_right->left; //do the right son of the node left son of his son
	if(node_right->left != nullptr) //if the node grand son isn't a nullptr
		node_right->left->parent = node; //create a connection between node and node_right left son
	node_right->left = node; //create a connection between node and node_right
	node->parent = node_right; //node_right is the node parent now
	if (ifRoot) //if node is root
		root = node_right; //make new root node_right
}

template<class T, typename T1>
void Map<T, T1>::RotateRight(Node* node) //right rotation 
{
	Node* node_left = node->left; //writes the left child of node
	bool ifRoot = (node == root ? true : false); //is the node root?
	if (!ifRoot) { //if the node isn't root
		if (RightOrLeft(node)) //if the node is right, then
			node->parent->right = node_left;  //the left son of the node becomes the son of the father of the node
		else
			node->parent->left = node_left; //the left son of the node becomes the son of the father of the node
	}
	node_left->parent = node->parent; //do the parent node of his son his father
	node->left = node_left->right; //do the left son of the node right son of his son
	if (node_left->right != nullptr) //if the node grand son isn't a nullptr
		node_left->right->parent = node; //create a connection between node and node_left right son
	node_left->right = node; //create a connection between node and node_left
	node->parent = node_left; //node_left is the node parent now
	if (ifRoot) //if node is root
		root = node_left; //make new root node_left
}

template<class T, typename T1>
void Map<T, T1>::VertexBalancing(Node* new_elem, Node* father, Node* gr_father) //balancing after inserting a new item
{
	if (RightOrLeft(father)) { //if the new nodes father is the right son
		if (gr_father->left != nullptr && gr_father->left->color == red) { //if the uncle of new node exists and his color is red
			father->color = black; //recolor father in black
			gr_father->left->color = black; //recolor uncle in black
			if (gr_father != root) { //grand father isn't a root
				gr_father->color = red; //recolor grand father in red
				if (gr_father->parent->color == red) //if uncle color is red
					VertexBalancing(gr_father, gr_father->parent, gr_father->parent->parent); //call balancing for grandfather, grand-grand father and grand-grand-grand father
			}
		}
		else { //if uncle is nullptr or uncle color is black
			if (RightOrLeft(new_elem)) { //if the new element is the right son
				RotateLeft(gr_father); //call left rotation of grand father
				father->color = black; //recolor father in black
				gr_father->color = red; //recolor grandfather in red
				
			}
			else { //if the new element is the left son
				RotateRight(father); //call right rotation of father
				VertexBalancing(father, new_elem, gr_father);
			}
		}
	}
	else { //if the new node father is the left son
		if (gr_father->right != nullptr && gr_father->right->color == red) { //if the uncle of new node exists and his color is red
			father->color = black; //recolor father in black
			gr_father->right->color = black; //recolor uncle in black
			if (gr_father != root) { //grand father isn't a root
				gr_father->color = red; //recolor grand father in red
				if(gr_father->parent->color == red) //if uncle color is red
					VertexBalancing(gr_father, gr_father->parent, gr_father->parent->parent); //call balancing for grandfather, grand-grand father and grand-grand-grand father
			}
		}
		else { //if uncle is nullptr or uncle color is black
			if (RightOrLeft(new_elem)) { //if the new element is the right son
				RotateLeft(father); //call left rotation of father
				VertexBalancing(father, new_elem, gr_father); //call balancing for father, new element and grandfather
			}
			else { //if the new element is the left son
				RotateRight(gr_father); //call the right rotation of the grandfather
				father->color = black; //recolor father in black
				gr_father->color = red; //recolor grandfather in red
			}
		}
	}
}

template<class T, typename T1>
void Map<T, T1>::insert(T data_n, T1 key_n) //adding new item
{
	Node* temp = root, * new_node = nullptr; //make a new pointer to root and null pointer new_node
	if (temp == nullptr) //if root is nullptr
		root = new Node(data_n, key_n, nullptr, black); //make a new element of the root
	else {
		//until we find a place to insert
		while ((temp->right != nullptr && key_n >= temp->key) || (temp->left != nullptr && key_n < temp->key)) {
			key_n < temp->key ? temp = temp->left : temp = temp->right; //depending on the key go to the left or right branch
		}
		if (key_n > temp->key) { //if key of new node bigger than previous node key
			temp->right = new Node(data_n, key_n, temp); //allocate memory for right son of the previous node
			new_node = temp->right; //new node is the right son of the previous node
		}
		else if (key_n < temp->key) { //if key of new node smaller than previous node key
			temp->left = new Node(data_n, key_n, temp); //allocate memory for left son of the previous node
			new_node = temp->left; //new node is the left son of the previous node
		}
		else //if the key of the new node is equal to the key of the previous node
			temp->data = data_n; //change the data to the data of the added node
		if (temp->color != black) //if the color of new node is red
			VertexBalancing(new_node, temp, temp->parent); //call balancing vertices for new node, his father and his grandfather
	}
}

template<class T, typename T1>
void Map<T, T1>::clear(Node* ptr) //recursive delete map
{
	if (ptr != nullptr) { //if the node exists
		Node* leftptr = ptr->left; //transition to the left branch of a tree
		Node* rightptr = ptr->right; //transition to the right branch of a tree
		delete ptr; //delete pointer
		clear(leftptr); //recursive call for the left branch
		clear(rightptr); //recursive call for the right branch
	}
}

template<class T, typename T1>
void Map<T, T1>::get_keys(Node* ptr) //returns a list of keys
{
	if (ptr != nullptr) { //if the node exists
		Node* leftptr = ptr->left; //transition to the left branch of a tree
		Node* rightptr = ptr->right; //transition to the right branch of a tree
		cout << " " << ptr->key << " "; //key output for node
	    get_keys(ptr->left); //recursive call for the left branch
		get_keys(ptr->right); //recursive call for the right branch
	}
}

template<class T, typename T1>
void Map<T, T1>::get_values(Node* ptr) //returns a list of data
{
	if (ptr != nullptr) { //if the node exists
		Node* leftptr = ptr->left; //transition to the left branch of a tree
		Node* rightptr = ptr->right; //transition to the right branch of a tree
		cout << " " << ptr->data << " "; //data output for node
		get_values(ptr->left); //recursive call for the left branch
		get_values(ptr->right); //recursive call for the right branch
	}
}

template<class T,typename T1>
void Map<T,T1>::GoToMaxLeft(Node* node) //looking for the maximum node from the left subtree (for remove)
{
	Node* temp = node->left; //transition to the left branch of a tree
	while (temp->right != nullptr) { //go to the right branch until you reach the leaf
		temp = temp->right; //transition to the right branch of a tree
	}
	node->data = temp->data; //rewriting the data to the max node from the left subtree
	node->key = temp->key; //rewriting the key to the max node from the left subtree
	if (temp->color == red) //if max left node color is red
		DeleteRed(temp); //go to method to remove red node
	else
		DeleteBlack(temp); //go to method to remove black node
}

template<class T, typename T1>
void Map<T, T1>::DeleteRed(Node* temp) //delete red node
{
	if (RightOrLeft(temp)) //division into cases for left and right nodes
	{
		if (temp->right == nullptr && temp->left == nullptr) { //both sons of node are leaves
			temp->parent->right = nullptr; //sever father and son
			delete temp; //delete node
		}
		else if (temp->right != nullptr && temp->left != nullptr) //the node has two sons
			GoToMaxLeft(temp); //select the max node from the left subtree for replacement
		else
		{
			if (temp->right == nullptr) { //the case when the node has one left son
				temp->parent->right = temp->left; //make grandson son
				temp->left->parent = temp->parent; //make new father for son of the temp
				delete temp; //delete node
			}
			else {                                 //the node has one right son
				temp->parent->right = temp->right; //make grandson son
				temp->right->parent = temp->parent; //make new father for son of the temp
				delete temp; //delete node
			}
		}
	}
	else {                                                     //temp is left son
		if (temp->right == nullptr && temp->left == nullptr) { //both sons of node are leaves
			temp->parent->left = nullptr; //sever father and son
			delete temp; //delete node
		}
		else if (temp->right != nullptr && temp->left != nullptr) //the node has two sons
			GoToMaxLeft(temp); //select the max node from the left subtree for replacement
		else
		{
			if (temp->right == nullptr) { //the case when the node has one left son
				temp->parent->left = temp->left; //make grandson son
				temp->left->parent = temp->parent; //make new father for son of the temp
				delete temp; //delete node
			}
			else {                                //the node has one right son
				temp->parent->left = temp->right; //make grandson son
				temp->right->parent = temp->parent; //make new father for son of the temp
				delete temp; //delete node
			}
		}
	}
}

template<class T, typename T1>
void Map<T, T1>::DeleteBlack(Node* temp) //delete black node
{
	if (temp->right == nullptr && temp->left == nullptr) //both sons of node are leaves
	{
		if (temp == root) { //if temp if root
			delete root; //detele node
			root = nullptr; //make root nullptr
		}
		else {
			FixUp(temp, temp->parent, RightOrLeft(temp) ? temp->parent->left : temp->parent->right); //call fixup for the node, his father and his brother
			RightOrLeft(temp) ? temp->parent->right = nullptr : temp->parent->left = nullptr; //node right/left break his relationship with his father
			delete temp; //delete node
		}
	}
	else if (temp->right == nullptr) // the case when the node has one left son
	{
		if (temp->left->color == red) //if left son color is red then
		{
			if (temp == root) { //is temp a root?
				root = temp->left; //make a root left son of the node
				root->color = black; //make a root color black
				root->parent = nullptr; //make a father of root nullptr
				delete temp; //delete node
			}
			else //if temp is not root then
			{
				if (RightOrLeft(temp)) { //if node is the right son
					temp->left->parent = temp->parent; //make grandson son
					temp->parent->right = temp->left; //break the bond of father and son
					temp->left->color = black; //make his son color black
					delete temp; //delete node
				}
				else {                                 //if node is the left son
					temp->left->parent = temp->parent; //make grandson son
					temp->parent->left = temp->left; //break the bond of father and son
					temp->left->color = black; //make his son color black
					delete temp; //delete node
				}
			}
		}
		else //if the left son color is black
		{
			if (RightOrLeft(temp)) { //if node is the right son
				temp->parent->right = temp->left; //break the bond of father and son
				temp->left->parent = temp->parent; //make grandson son
				FixUp(temp->left, temp->parent, temp->parent->right); //call FixUp for the temp left son, temp father and temp brother
			}
			else {                               //if node is the left son
				temp->parent->left = temp->left; //break the bond of father and son
				temp->left->parent = temp->parent; //make grandson son
				FixUp(temp->left, temp->parent, temp->parent->right); //call FixUp for the temp left son, temp father and temp brother
			}
			delete temp; //delete node
		}
	}
	else if (temp->left == nullptr) //if temp has one right son
	{
		if (temp->right->color == red) //if left son color is red
		{
			if (temp == root) { //is temp a root?
				root = temp->right; //make a root right son of the node
				root->color = black; //make a root color black
				root->parent = nullptr; //make a father of root nullptr
				delete temp; //delete node
			}
			else //if temp son color is black
			{
				if (RightOrLeft(temp)) { //if node is the right son
					temp->right->parent = temp->parent; //make grandson son
					temp->parent->right = temp->right; //break the bond of father and son
					temp->right->color = black; //make his son color black
					delete temp; //delete node
				}
				else {
					temp->right->parent = temp->parent; //make grandson son
					temp->parent->left = temp->right; //break the bond of father and son
					temp->right->color = black; //make his son color black
					delete temp; //delete node
				}
			}
		}
		else //if the node son color black
		{
			if (RightOrLeft(temp)) { //if the node is the right son
				temp->parent->right = temp->right; //break the bond of father and son
				temp->right->parent = temp->parent; //make grandson son
				FixUp(temp->right, temp->parent, temp->parent->left); //call FixUp for temp right son, temp father and temp brother
			}
			else { //if the node is the left son
				temp->parent->left = temp->right; //break the bond of father and son
				temp->right->parent = temp->parent; //make grandson son
				FixUp(temp->right, temp->parent, temp->parent->left); //call FixUp for temp right son, temp father and temp brother
			}
			delete temp; //delete node
			temp = nullptr; //temp pointer nullptr
		}
	}
	else //node has two sons
		GoToMaxLeft(temp); //select the max node from the left subtree for replacement
}

template<class T, typename T1>
void Map<T, T1>::FixUp(Node* node, Node* nodePar, Node* brother) //recursive method for fix up changes after node removing
{
	//division into several cases depending on the color of the node father and node brother sons
	if (nodePar->color == black && (brother->left == nullptr || brother->left->color == black) && (brother->right == nullptr || brother->right->color == black))
	{
		brother->color = red; //recolor brother in red
		if (nodePar != root) //if node parent  isn't a root, call FixUp for node father, node grandfather and node brother
			FixUp(nodePar, nodePar->parent, RightOrLeft(nodePar) ? nodePar->parent->left : nodePar->parent->right);
	}
	else if (nodePar->color == red && (brother->left == nullptr || brother->left->color == black) && (brother->right == nullptr || brother->right->color == black))
	{
		nodePar->color = black; //recolor father in black
		brother->color = red; //recolor brother in red
	}
	else if (!RightOrLeft(node) && brother->right != nullptr && brother->right->color == red)
	{
		brother->right->color = black; //recolor brother right son in black
		brother->color = nodePar->color; //recolor brother in father color
		nodePar->color = black; //recolor father in black
		RotateLeft(nodePar); //call left rotation for father
	}
	else if (RightOrLeft(node) && brother->left != nullptr && brother->left->color == red)
	{
		brother->left->color = black; //recolor brother left son in black
		brother->color = nodePar->color; //recolor brother in father color
		nodePar->color = black; //recolor father in black
		RotateRight(nodePar); //call right rotation for father
	}
	else if (!RightOrLeft(node) && brother->left != nullptr && brother->left->color == red)
	{
		brother->left->color = nodePar->color; //recolor brother left son in father color
		nodePar->color = black; //recolor father in black
		RotateRight(brother); //call right rotation for brother
		RotateLeft(nodePar); //call left rotation for father
	}
	else if (RightOrLeft(node) && brother->right != nullptr && brother->right->color == red)
	{
		brother->right->color = nodePar->color; //recolor brother right son in father color
		nodePar->color = black; //recolor father in black
		RotateLeft(brother); //call left rotation for brother
		RotateRight(nodePar); //call right rotation for father
	}
}

template <class T, typename T1>
void Map<T, T1>::remove(T1 key_n) //remove node by the key
{
	Node* temp = find(key_n); //find the node that want to delete
	if (temp->color == red) //if the temp color is red then
		DeleteRed(temp); //call deleting for red node
	else
		DeleteBlack(temp); //call deleting for black node
}
