#pragma once
//#include "../../project_1/project_1/class_1.h"
#include "../../project_2/project_2/Map_class.h"
#include <iostream>

using namespace std;

class Node
{
	Node* left;
	Node* right;
	Node* parent;
	string symb;
	int freq;
	string code;
public: 
	Node() {};
	Node(string symb_n, int freq_n, Node* left_n = nullptr, Node* right_n = nullptr, Node* parent_n = nullptr) :symb(symb_n), freq(freq_n), left(left_n), right(right_n), parent(parent_n) {};
	string GetSymb();
	friend bool operator < (const Node& node_1, const Node& node_2);
	friend ostream& operator << (ostream& output_stream, Node& OBJECT);
	void set_parent(Node*);
	int GetFreq();
	void set_symb(string symb_1);
	void set_freq(int freq_1);
	template<class T, typename T1> friend void Travers(Node* node, Map<T, T1>& OBJ, string, int);
};

void Node::set_symb(string symb_1) {
	symb = symb_1;
}

void Node::set_freq(int freq_1) {
	freq = freq_1;
}

void Node::set_parent(Node* par_1){
	parent = par_1;
}

int Node::GetFreq() {
	return(freq);
}

string Node::GetSymb() {
	return(symb);
}

ostream& operator << (ostream& output_stream, Node& OBJECT)
{
	output_stream << "Symb: " << OBJECT.symb << endl << "Freq: " << OBJECT.freq << endl;
	return output_stream;
}

bool operator < (const Node& node_1, const Node& node_2)
{
	return(node_1.freq < node_2.freq ? true : false);
}