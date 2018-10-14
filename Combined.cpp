#include <iostream>
#include <string>
#include <fstream>

#ifdef _WIN32
#include "getopt.h"
#else
#include <unistd.h>
#endif

using namespace std;

#ifndef ROLODEX_H_
#define ROLODEX_H_

template<class T>
class Rolodex { // Rolodex Class.
public:

	struct RolodexItem { // Defines a RolodexItem, contains a word and 2 pointers for next and prev items in the list.
		T value_;
		T LowerValue;
		RolodexItem* next_;
		RolodexItem* prev_;
		int sequence;
	};

	Rolodex() { // Default constructor.
		inserted = 0;
		deleted = 0;
		forward = 0;
		backward = 0;
		sentinel = new RolodexItem;
		sentinel->next_ = sentinel;
		sentinel->prev_ = sentinel;
		current = sentinel;
	}

	bool isBeforeFirst() { // Returns true if the Rolodex is positioned before the first card.
		if (current == sentinel) { return(true); }
		else { return(false); }
	}

	bool isAfterLast() { // Returns true if the Rolodex is positioned after the last card.
		if (current == sentinel) { return (true); }
		else { return(false); }
	}

	void rotateForward() { // Rotates the Rolodex one step forwards.
		current = current->next_;
		forward++;
		return;
	}

	void rotateBackward() { // Rotates the Rolodex one step backwards.
		current = current->prev_;
		backward++;
		return;
	}

	T& currentValue(bool alpha = false) { // Returns the value of the current card or the lowercase card depending on alpha's state. 
		if (alpha) {
			return (current->LowerValue);
		}
		return(current->value_);
	}

	void insertAfterCurrent(const T& value) { // Inserts a new card after the current position and positions the Rolodex at the newly inserted card.
		current->next_->prev_ = new RolodexItem;
		current->next_->prev_->prev_ = current;
		current->next_->prev_->next_ = current->next_;
		current->next_ = current->next_->prev_;
		current = current->next_;
		current->value_ = value;
		current->LowerValue = value;

		string temp = current->LowerValue;
		for (int i = 0; i < temp.size(); i++) {
			temp[i] = tolower(temp[i]);
		}
		current->LowerValue = temp;
		inserted++;
		current->sequence = inserted + deleted;
		return;
	}

	void insertBeforeCurrent(const T& value) { // Inserts a new card before the current position and positions the Rolodex at the newly inserted card.
		current->prev_->next_ = new RolodexItem;
		current->prev_->next_->next_ = current;
		current->prev_->next_->prev_ = current->prev_;
		current->prev_ = current->prev_->next_;
		current = current->prev_;
		current->value_ = value;
		current->LowerValue = value;

		string temp = current->LowerValue;
		for (int i = 0; i < temp.size(); i++) {
			temp[i] = tolower(temp[i]);
		}
		current->LowerValue = temp;
		inserted++;
		current->sequence = inserted + deleted;
		return;
	}

	void Delete() { // Deletes current card. Sets current to next unless next is sentinel. If next is sentinel current is set to previous.
		RolodexItem* temp;
		current->prev_->next_ = current->next_;
		current->next_->prev_ = current->prev_;
		if (current->next_ != sentinel) { temp = current->next_; }
		else { temp = current->prev_; }
		delete current;
		current = temp;
		deleted++;
		return;
	}

	void report() { // reports nember of times each operation was executed.
		cout << inserted << " " << deleted << " " << forward << " " << backward << endl;
	}

	int Sequence() {
		return (current->sequence);
	}

	void printState() { // Testing purposes.
		RolodexItem *item = sentinel->next_;
		while (item != sentinel) {
			item = item->next_;
			cout << item->value_ << endl;
		}
	}

private:
	int deleted;
	int inserted;
	int forward;
	int backward;
	RolodexItem* sentinel;
	RolodexItem* current;
};

#endif /* ROLODEX_H_ */

istream& input(bool FILEFLAG);
ifstream in;

int main(int argc, char** argv) {
	int c;
	bool verbose = false;      // Report actions flag.
	bool NoDuplicates = false; // NoDuplicates flag.
	bool report = false;       // Reports operations flag.
	bool alphabetic = false;   // Alphabetical order flag.
	bool sequence = false;     // Output sequence flag.
	bool plus = false;         // Plus flag decleration.
	bool minus = false;        // Minus flag decleration.
	bool FILEFLAG; // Flag to determine wether to use standard input or external file input.

	while ((c = getopt(argc, argv, "vdras")) != -1) {
		switch (c) {
		case 'v': verbose = true; break;      // Report actions if true.
		case 'd': NoDuplicates = true; break; // Will not insert duplicates if true.
		case 'r': report = true; break;       // Reports operations if true.
		case 'a': alphabetic = true; break;   // Outputs in alphabetical order if true.
		case 's': sequence = true; break;   // Outputs words input order if true.
		}
	}
	argc -= optind;
	argv += optind;

	if (argc == 0) { FILEFLAG = false; }
	for (int i = 0; i < argc; i++) {
		in.open(argv[i]);
		if (in.fail()) { cerr << "Can't read " << argv[i] << endl; exit(0); }
		FILEFLAG = true;
	}

	string word;
	Rolodex<string> rolodex;

	while (input(FILEFLAG) >> word) {
		if (word[0] == '+') { plus = true; word.erase(word.begin()); }  // Sets plus flag if a '+' is read.
		if (word[0] == '-') { minus = true; word.erase(word.begin()); } // Sets minus flag if a '-' is read.

		string compareword = word;
		if (alphabetic) {
			for (int i = 0; i <= word.length(); i++) { // Converts the word to lowercase for comparison with current->LowerValue;
				compareword[i] = tolower(word[i]);
			}
		}

		if (rolodex.isBeforeFirst() || rolodex.currentValue(alphabetic) <= compareword) { // If rolodex is before first or the current value(word) is <= word.
			while (!rolodex.isAfterLast() && rolodex.currentValue(alphabetic) < compareword) { // While rolodex is not after last and the current value(word) is < word.
				rolodex.rotateForward();
				if (verbose) { cerr << "forward" << endl; }
			}
			if (!minus) { // If '-' wasnt used...
				if (!NoDuplicates) { // If -d is not used, always insert the word.
					rolodex.insertBeforeCurrent(word);
					if (verbose) { cerr << "insertBefore" << endl; }
				}
				else if (rolodex.currentValue(alphabetic) != compareword || plus == true) { // If -d is used, insert word if not already present or it has a '+' prefix.
					rolodex.insertBeforeCurrent(word);
					plus = false; // Reset flag.
				}
			}
			else if (rolodex.currentValue(alphabetic) == compareword) { // If '-' was used and the current value is not == word.
				rolodex.Delete(); // Delete the current word.
				minus = false; // Reset flag.
			}
		}
		else if (rolodex.isAfterLast() || rolodex.currentValue(alphabetic) >= compareword) {
			while (!rolodex.isBeforeFirst() && rolodex.currentValue(alphabetic) > compareword) {
				rolodex.rotateBackward();
				if (verbose) { cerr << "backward" << endl; }
			}
			if (!minus) {
				if (!NoDuplicates) {
					rolodex.insertAfterCurrent(word);
					if (verbose) { cerr << "insertAfter" << endl; }
				}
				else if (rolodex.currentValue(alphabetic) != compareword || plus == true) {
					rolodex.insertAfterCurrent(word);
					plus = false;
				}
			}
			else if (rolodex.currentValue(alphabetic) == compareword) {
				rolodex.Delete();
				minus = false;
			}
		}
	}
	if (sequence) { // Outputs the sequence number in which the word was entered, also outputs in alphabetical order.
		while (!rolodex.isBeforeFirst()) { rolodex.rotateBackward(); }
		rolodex.rotateForward(); // Go to first card
		while (!rolodex.isAfterLast()) {
			cout << rolodex.Sequence() << " " << rolodex.currentValue() << endl; // Outputs sequence number " " current card.
			rolodex.rotateForward();
		}
	}
	else if (!report) { // Outputs in either normal alphabetical or strict alphabetical depending on if -a was used.
		while (!rolodex.isBeforeFirst()) { rolodex.rotateBackward(); }
		rolodex.rotateForward(); // Go to first card
		while (!rolodex.isAfterLast()) {
			cout << rolodex.currentValue() << endl;
			rolodex.rotateForward();
		}
	}
	else{ rolodex.report(); } // Reports operations.


#ifdef _WIN32
	system("pause");
#endif

	return(0);
}

istream& input(bool FILEFLAG) {
	if (FILEFLAG == true) { return in; }
	else { return cin; }
}