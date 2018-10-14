/**
 * A program to implement rolosort.
 *
 * Rolosort is a variant of insertion sort that uses a Rolodex file.
 * The Rolodex is rotated forward or backward to find the position
 * in which to insert new items.
 *
 * @author roge0270
 */

#include "Rolodex.h"

#include <iostream>
#include <unistd.h>
#include <fstream>

using namespace std;

//sets a string to all uppercase
string toUpper(string s)
{
    string upper = s;

    for (int i = 0; i < upper.length(); i++)
        upper[i] = toupper(upper[i]);

    return upper;
}

//checks equality between two strings
bool check(string value, string word, char op, bool alphabetic)
{
    if (alphabetic)
    {
        switch (op)
        {
            case '=': return toUpper(value) == toUpper(word); 
            case '<': return toUpper(value) < toUpper(word); 
            case '#': return toUpper(value) <= toUpper(word); 
            case '$': return toUpper(value) >= toUpper(word); 
            case '>': return toUpper(value) > toUpper(word); 
        }
    }
    else
    {
        switch (op)
        {
            case '=': return value == word;
            case '<': return value < word; 
            case '#': return value <= word;
            case '$': return value >= word;
            case '>': return value > word;
        }
    }
}

//checks if there is a file input
istream& in(ifstream& file)
{
    if (file.is_open())
        return file;
    else
        return cin;
}

// report actions, verbose mode, duplicate mode, report mode, alphabetic mode, sequence mode
bool verbose = false;                                       
bool duplicate = false;
bool report = false;
bool alphabetic = false;
bool sequence = false;

int main (int argc, char** argv) 
{
    int c;
    while ((c = getopt(argc, argv, "vdras")) != -1) 
    {
        switch (c) 
        {
            case 'v': 
                verbose = true; 
                break;
            case 'd':
                duplicate = true;
                break;
            case 'r':
                report = true;
                break;
            case 'a':
                alphabetic = true;
                break;
            case 's':
                sequence = true;
                break;
        }
    }
    argc -= optind;
    argv += optind;

    for (int i = 0; i <= argc; i++)
    {
        //input file stream
        ifstream infile;

        //if there is a file in the command line
        if (argc > i)
        {
            //try opening the file
            try
            {
                infile.open(argv[i]);

                //if the file isn't open, throw an exception of the file name
                if (!infile.is_open())
                {
                    string e = argv[i];
                    throw e;
                }
            }
            //catch the thrown exception
            catch (string e)
            {
                cerr << "Can't read " << e << endl;
                return 0;
            }
        }

        //word that reads input
        string word;
        //create an empty rolodex using strings
        Rolodex<string> rolodex;

        //integers to track statistics of total items inserted, total items deleted, amount of times rotated forwards and amount of times rotated backwards
        int itemsInserted = 0;                                  
        int itemsDeleted = 0;
        int forwards = 0;
        int backwards = 0;

        //while receiving input
        while (in(infile) >> word && word != ")")                      
        {
            //initialise adding a duplicate and delete a word to false
            bool addDup = false;
            bool deleteWord = false;

            if (word == "-")
            {
                continue;
            }

            //if the first character is a '+', it will add a duplicate word
            if (word[0] == '+')
            {
                addDup = true;                                  
                word.erase(0, 1);                               //erase the '+' from the string
            }
                
            //if the first character is a '-', it will try to delete the word
            if (word[0] == '-')
            {
                deleteWord = true;
                word.erase(0, 1);                               //erase the '-' from the string
            }

            if (word.length() == 0)
                continue;

            //rolodex is before the sentinal node or word is less then current value
            if (rolodex.isBeforeFirst() || check(rolodex.currentValue(), word, '#', alphabetic))
            {
                //while rolodex is not at the sentinal node and word is less than current value, rotate forward
                while (!rolodex.isAfterLast() && check(rolodex.currentValue(), word, '<', alphabetic)) 
                {
                    rolodex.rotateForward();
                    forwards++;
                       
                    if (verbose)
                        cerr << "forward" << endl;
                }

                //if current value and word are equal
                if (check(rolodex.currentValue(), word, '=', alphabetic))
                {
                    //if the prefix is '-', delete the current value
                    if (deleteWord)
                    {
                        rolodex.deleteCurrent();
                        itemsDeleted++;
                    }
                    //if duplicate mode is on and the prefix is '+', insert the word
                    else if (duplicate)
                    {
                        if (addDup)
                        {
                            rolodex.insertBeforeCurrent(word);
                            itemsInserted++;
                        }
                    }
                    //if duplicate mode is off, insert the word
                    else
                    {
                        rolodex.insertBeforeCurrent(word);
                        itemsInserted++;
                    }
                }
                //if the current value and word are not equal
                else
                {
                    //if the prefix is NOT '-', insert the word
                    if (!deleteWord)
                    {
                        rolodex.insertBeforeCurrent(word);
                        itemsInserted++;
                    }
                }
                
                if (verbose)
                    cerr << "insertBefore" << endl;
            } 
            //rolodex is after sentinal node or word is greater then current value
            else if (rolodex.isAfterLast() || check(rolodex.currentValue(), word, '$', alphabetic)) 
            {
                //while rolodex is not at the sentinal node and word is greater then current value, rotate backwards
                while (!rolodex.isBeforeFirst() && check(rolodex.currentValue(), word, '>', alphabetic)) 
                {
                    rolodex.rotateBackward();
                    backwards++;
                        
                    if (verbose) 
                        cerr << "backward" << endl;
                }

                //if current value and word are equal
                if (check(rolodex.currentValue(), word, '=', alphabetic))
                {
                    //if the prefix is '-', delete the current value
                    if (deleteWord)
                    {
                        rolodex.deleteCurrent();
                        itemsDeleted++;
                    }
                    //if duplicate mode is on and the prefix is '+', insert the word
                    else if (duplicate)
                    {
                        if (addDup)
                        {
                            rolodex.insertAfterCurrent(word);
                            itemsInserted++;
                        }
                    }
                    //if duplicate mode is off, insert the word
                    else
                    {
                        rolodex.insertAfterCurrent(word);
                        itemsInserted++;
                    }
                }
                //if the current value and word are not equal
                else
                {
                    //if the prefix is NOT '-', insert the word
                    if (!deleteWord)
                    {
                        rolodex.insertAfterCurrent(word);
                        itemsInserted++;
                    }
                }
                    
                if (verbose) 
                    cerr << "insertAfter" << endl;
            } 
        }

        //while the rolodex is before the start, rotate backwards
        while (!rolodex.isBeforeFirst())                        
            rolodex.rotateBackward();                   

        //rotate forward once so it's not pointing to sentinal node
        rolodex.rotateForward();

        //rotates forwards through the rolodex from start to finish
        while (!rolodex.isAfterLast())                          
        {
            //if report mode is on, output the relevant statistics
            if (report)                                         
            {
                cout << itemsInserted << " " << itemsDeleted << " " << forwards << " " << backwards << endl;    
                break;                                                                                          
            }
            //if report mode is not on, output the current value and rotates forwards
            else                                                
            {
                //if sequence mode is on, output the input sequence before the current value
                if (sequence)
                    cout << rolodex.currentOrder() << " ";

                cout << rolodex.currentValue() << endl;    

                rolodex.rotateForward();   
            }
        }

        //if an input file was used, close the file stream
        if (infile.is_open())
            infile.close();
    }
}