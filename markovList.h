//*********************************************************
// Author: Michael Clinton
// File: markovList.h
// Date: 11/24/2023
// Purpose: Header for program 4.
//*********************************************************

#ifndef MARKOVLIST_H
#define MARKOVLIST_H

#include<map>
#include<fstream>
#include<sstream>
#include<iostream>
#include<stdlib.h>

using namespace std;


// Defines the edge struct class
struct edge
{
    // Destination node
    string destination;
    // Weight of traversing the edge
    float weight;
    // Points to the next edge in the list
    edge* next;
};

class markovList
{
    private:
        map<string, edge*> corpus;
    int corpusSize;

    public:
        markovList(const char*);
        ~markovList();

        string generate(int);
};



markovList::markovList(const char* filename)  
{
    ifstream file(filename);
    stringstream parser, splitter;
    string line, index, word;
    float weight;
    edge* newEdge;
    int i = 0;  
    srand(time(0));
    

    if (file.good())
    {
        file >> corpusSize;
        file.ignore(1, '\n');

        while (getline(file, line, '\n')) //map every word to a position in the matrix, keep the line in an array of buffers for later
        {
            parser.clear();
            parser << line;
            getline(parser, index, ','); //pulls the first word of the line, which is the node for which we're making a list of neighbors

            // Initializes an empty list at the index
            edge* head = nullptr;

            while (getline(parser, word, ','))
            {
                splitter.clear();
                splitter.str(word);
                splitter >> word >> weight;

                // Allocates a new node in the edge list
                newEdge = new edge;
                newEdge->destination = word;
                newEdge->weight = weight;
                newEdge->next = head;

                //make sure your new node is attached to the list
                head = newEdge;
            }

            // Adds the list to the corpus map
            corpus[index] = head;
        }
    }
}

markovList::~markovList()
{
    // Destructor to release any dynamically allocated resources
    // Loops through every array in the corpus
    for (auto& entry : corpus) 
    {
        edge* current = entry.second;
        // Traverses the linked list and deallocate memory for each edge
        while (current != nullptr)
        {
            edge* next = current->next;
            delete current;
            // Moves to the next edge in the list
            current = next;
        }
    }
}

string markovList::generate(int length)
{
    map<string, edge*>::iterator it = corpus.begin();
    advance(it, rand() % corpusSize);

    string r = it->first; // Starts with the initial node 

    for (int i = 0; i < length; ++i)
    {
        // Checks the value against to weight every edge
        float randomValue = (float)rand() / RAND_MAX;
         

        // Traverses through the edge list
        edge* current = it->second;
        // Moves to the next edge in the linked list
        while (current != nullptr)
        {
            randomValue -= current->weight;// might delete this 

            // Checks if the random value is less than the accumulated weight
            if (randomValue < 0)
            {
                // Does a traversal to the destination node
                // Appends the current destination node to the result string
                r += " " + current->destination; 
                it = corpus.find(current->destination); // mention C++ website here and add documentation of what its doing
                break;
            }

            current = current->next;
        }
    }

    return r; // Returns result
}

#endif
