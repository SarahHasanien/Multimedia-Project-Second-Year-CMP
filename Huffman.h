#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;
struct Symbol {
	char value;
	int frequency;//i should keep the frequency in this struct as i need it to calculate the probability of each symbol
	float probability;
	string code;
};
struct node {
	char s;
	float prob;
	string code;
	node* right;
	node* left;
};
string ReadMessage(string file);
map<char, string> makeHuffmanTree(vector<Symbol>& v,node*& tree);
//void makeHuffmanTree(vector<Symbol>& v, int g);//this integer to know if i was decoding or encoding
void CalculateProbabilities(vector<Symbol>& symbols, string message);
void makeEncodedFile(map<char, string>m, string readFile, vector<Symbol> v, string fileName);
string HuffmanEncodemain(string file)//returns the name of the encoded file
{
	//first read the file that has been encoded using RLE
	string s1;

	//1-read the text file we want to encode
	s1 = ReadMessage(file);


	//2-calculate frequency and probability for each symbol in the text file we read
	//make the dictonary that will have frequency of each char/symbol in the text file we read
	//or just make array of symbols that carry both prob and freq for each symbol or char in the text file we read,i can also try
	//to store codeword in the symbol but i'm trying
	vector<Symbol> symbols;
	//after that calculate freq and prob of each char in the text file we read
	CalculateProbabilities(symbols, s1);
	node* HuffmanTree1;
	map<char, string>HuffmanMam = makeHuffmanTree(symbols, HuffmanTree1);
	//string newfile = "rle2.txt";
	//makeEncodedFile(HuffmanMam, s1, symbols, newfile);
	makeEncodedFile(HuffmanMam, s1, symbols, file); //use this line instead of the previous two lines when encoding on the original text file we take from the RLE

	cout << "step 2: huffman encoding passed well " << endl;

	return "rle2.txt";////delete this after testing

}

void CalculateProbabilities(vector<Symbol>& symbols, string message) {

	//now we get each char and put it in the vector of symbols
	float totalNoOfChar = message.size();
	int i = 0;//to traverse the message
	Symbol s;
	while (i < totalNoOfChar)
	{
		float f = 0;//to count the no of redundancies of a certain char in message
		s.value = message[i];
		int sizeOfS = symbols.size();
		bool flag = true;
		if (sizeOfS > 0)
		{
			for (int k = 0; k < sizeOfS; k++)
			{
				if (symbols[k].value == s.value)
				{
					//loop the bigger loop again
					k = sizeOfS;
					flag = false;
				}
			}
		}
		if (flag)
		{
			int j = 0;//new index to search message to find no of redundancies of the char i have in s
			while (j < totalNoOfChar)
			{
				if (s.value == message[j])f++;
				j++;
			}
			s.frequency = f;
			s.probability = f / totalNoOfChar;
			symbols.push_back(s);
		}
		i++;
	}

}

string ReadMessage(string file)
{
	string s;
	ifstream Ftxt;
	Ftxt.open(file);
	if (!Ftxt)
	{
		cout << "error opening the file" << endl;
	}

	while (!Ftxt.eof())
	{
		Ftxt >> s;
	}
	return s;
}

void makeCodeWord(node* t, string c, map<char, string>&m)
{
	if (t->right != NULL)makeCodeWord(t->right, c + '1', m);
	if (t->left != NULL)makeCodeWord(t->left, c + '0', m);
	t->code = c;
	m.insert(pair<char, string>(t->s, t->code));
	return;
}
bool my_comp(node* a, node* b)
{
	return a->prob > b->prob;
}

map<char, string> makeHuffmanTree(vector<Symbol>& v,node*& tree)
{
	//first make the tree
	int size1 = v.size();
	vector<node*> v2;
	for (int i = 0; i < size1; i++)
	{
		node* n = new node;
		n->left = NULL;
		n->right = NULL;
		n->s = v[i].value;
		n->prob = v[i].probability;
		v2.push_back(n);
	}
	//sort the tree
	sort(v2.begin(), v2.end(), my_comp);
	//now working on the algorithm that generates the big tree
	while (v2.size() != 2)
	{
		node* n = new node;
		n->left = v2[size1 - 1];
		n->right = v2[size1 - 2];
		n->prob = v2[size1 - 1]->prob + v2[size1 - 2]->prob;
		v2.pop_back();
		v2.pop_back();
		v2.push_back(n);
		size1--;
		sort(v2.begin(), v2.end(), my_comp);
	}
	//node* tree = new node;
	tree = new node;
	tree->right = v2[0];
	tree->left = v2[1];
	//second make codeword for leaves in the tree
	map<char, string>m;
	makeCodeWord(tree, "", m);
	//third assign codewords made to the vector of symbols we have so that each symbol will contain its codeword
	//assignCodeWord(v, m);//or return the map instead and in the main we can assign codeword to vector of symbols or just use this map to 
	//write the encoding for each char in the file we read
	//assigning each codeword to its symbol in vector v
	for (int i = 0; i < v.size(); i++)
		v[i].code = m[v[i].value];

	//the tree should be destructed before leaving this function
	//using function destruct tree
	return m;
}

//a function to make the decoded file using the my dictionary
void makeEncodedFile(map<char, string>m, string readFile, vector<Symbol> v, string fileName)
{
	//make a file:
	ofstream Stxt;
	Stxt.open(fileName);

	//i should first store my dictionary in the beginning of the encoded message using the vector of symbols
	//i will put each symbol with its prob beside it and i will 2fsel between each two symbols using space 
	//after the last symbol i will send endl to the encoded file so that i begin to enter the encoded message
	for (int i = 0; i < v.size(); i++)
	{
		Stxt << v[i].value << v[i].probability << ' ';
	}
	//send endl to the encoded file after sending the whole dictionary
	Stxt << '\n';

	//send the encoded message
	for (int i = 0; i < readFile.size(); i++)
	{
		Stxt << m[readFile[i]];
	}
	Stxt.close();
}


//functions for decoding:
void makeSymbolsVector(vector<Symbol>& v, vector<string> s);
//function to read the encoded file:
//when reading the encoded file it will differ from reading the original file as the encoded file has endl at the end of the dictionary
//and when reading endl it means that now we will read our encoded text so now we are reading only zeros and ones not symbols with their probabilities
string ReadEncodedMes(string filename, vector<Symbol>& v)
{
	//this function will be the main of the decoding but i am trying here in an external function so that don't make the main messy
	string s1, encoded;
	vector<string> s;
	//open file
	ifstream Ftxt;
	Ftxt.open(filename);
	if (!Ftxt)
	{
		cout << "error opening the file" << endl;
	}

	while (!Ftxt.eof())
	{
		Ftxt >> s1;
		s.push_back(s1);
	}
	encoded = s[s.size() - 1];
	s.pop_back();

	//make the vector of symbols to make the tree of symbols after that
	makeSymbolsVector(v, s);
	return encoded;
}
void makeSymbolsVector(vector<Symbol>& v, vector<string> s)
{
	for (int i = 0; i < s.size(); i++)
	{
		Symbol symbol;
		string str;
		str = s[i];
		symbol.value = str[0];
		str = str.substr(1);
		symbol.probability = stof(str);
		v.push_back(symbol);
	}
}

string HuffmanDecodingmain(string filename)//it returns the name of the decoded file
{
	vector<Symbol> symbols;
	//string file = "rle2.txt";
	//string encodedMessage = ReadEncodedMes(file, symbols);
	string encodedMessage = ReadEncodedMes(filename, symbols);
	node* HuffmanTree;
	makeHuffmanTree(symbols, HuffmanTree);


	//make the text file to write in :
	ofstream Stxt;
	//Stxt.open("rle3.txt");
	Stxt.open(filename); //use this line instead of the previous line when decoding on the same text file we read
	//put the decoded text in the text file rle3.txt
	node* ptr = new node;
	ptr = HuffmanTree;
	int j = 0;
	while (j<encodedMessage.size())
	{
		bool flag = true;
		for (int i = j; flag; i++)
		{
			if (encodedMessage[i] == '0' && ptr->left != NULL)
				ptr = ptr->left;
			else if (encodedMessage[i] == '1' && ptr->right != NULL)
				ptr = ptr->right;
			else
			{
				flag = false;
				Stxt << ptr->s;
				ptr = HuffmanTree;
				j--;
			}
			j++;
		}
	}
	Stxt.close();
	
	cout << "step 2 decoding: huffman decoding passed well" << endl;

	return "rle3.txt";//////delete this after testing
}