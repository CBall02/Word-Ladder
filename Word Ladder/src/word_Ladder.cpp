#include <iostream>
#include <chrono>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <queue>
#include <set>

using namespace std;

/* Constants */
const int ALPHA_LENGTH = 26;

/* Prototypes */
void getWords(string &word1, string &word2);
void printWordLadder(string word1, string word2, vector<string> &dictionary);
bool checkRealWord(string word, vector<string> &dictionary);

void getDict(vector<string> &wordsList) {
	auto start = chrono::steady_clock::now();
	ifstream myFile;
	string dictFile = "words_alpha.txt";
	while (true) {
		myFile.open(dictFile);
		if (myFile.is_open()) {
			std::cout << "Getting Dictionary.....\n";
			for (string words; getline(myFile, words);) {
				wordsList.push_back(words);
			}
			auto end = chrono::steady_clock::now();
			string wordsGotten = "Dictionary created.";
			std::cout << wordsGotten << " Took " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";
			cout << "----------------------------\n";
			break;
		}
		else {
			std::cout << "Cannot open dictionary file. Standard dictionary file is 'words_alpha.txt'" << endl;
			char answer;
			while (true) {
				std::cout << "Would you like to specify a dictionary file? y/n" << endl;
				cin >> answer;
				switch (answer) {
				case 'y': case 'Y':
					cout << "What is the name of the dictionary file?" << endl;
					cin >> dictFile;
					break;
				case 'n': case 'N':
					exit(1);
				default:
					cout << "Invalid Selection. Please try Again." << endl << endl;
					break;
				}
				break;
			}

		}
	}
}


/* Main function */
vector<string> dictionary;
int main() {
	
	getDict(dictionary);
	string word1, word2;
	getWords(word1, word2);
	printWordLadder(word1, word2, dictionary);
	char answer;
	while (true){
		std::cout << endl << endl << "Would you like to generate another word ladder? y/n" << endl;
		cin >> answer;
		switch (answer) {
		case 'y': case 'Y':
			getWords(word1, word2);
			printWordLadder(word1, word2, dictionary);

			break;
		case 'n': case 'N':
			exit(0);
		default:
			cout << "Invalid Selection. Please try Again." << endl;
			break;
		}
	}
	
	//system("pause");
	return 0;
}



//Function: checkRealWord
//Usage: checkRealWord(word)
//---------------------------
//This function takes one string as a parameter (passed directly)
//checks if the word passed is a member of the dictionary used.
//returns bool


bool checkRealWord(string word, vector<string> &dictionary) {
	bool is_a_word = false;
	unsigned long int i = 0;
	while ( i < dictionary.size()) {
		if (word.compare(dictionary.at(i)) == 0) {
			is_a_word = true;
			break;
		}
		i++;
	}
	return is_a_word;
}


//Function: getWords
//Usage: getWords(word1, word2)
//-----------------------------
//This function takes two strings as parameter (passed by reference),
//prompts the user for her input and stores her answer in those two
//parameters.

void getWords(string &word1, string &word2) {
	while (true) {
		std::cout << "Please enter a word: ";
		cin >> word1;

		std::cout << "Please enter another word of the same length: ";
		cin >> word2;
		transform(word1.begin(), word1.end(), word1.begin(), ::tolower);
		transform(word2.begin(), word2.end(), word2.begin(), ::tolower);
		if (word1.length() == word2.length()) {
			auto start = chrono::steady_clock::now();
			bool first_real = checkRealWord(word1, dictionary);
			bool second_real = checkRealWord(word2, dictionary);
			auto end = chrono::steady_clock::now();
			cout << "Dictionary check took " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";
			if (first_real == true && second_real == true) {
				break;
			}
			else {
				if(!first_real){
					cout << word1 << " is not a real word. ";
				}
				if (!second_real) {
					cout << word2 << " is not a real word. ";
				}
				std::cout << "Please enter two words that are real.\n";
			}
		}
		else {
			std::cout << "Please enter two words with the same length." << endl;
		}
	}
}


/* Function: printWordLadder
 * Usage: printWordLadder(word1, word2)
 * ------------------------------
 * This function takes two strings as parameters and prints the
 * minimal word ladder between two words.
 */

void printWordLadder(string word1, string word2, vector<string> &dictionary) {
	auto start = chrono::steady_clock::now();

	// creates an empty queue of stacks
	queue<stack<string> > myQueue;

	//Create a stack which will contain a final word ladder
	stack<string> wordladder;

	// creates and adds a stack containing word1 to the queue
	stack<string> myStack;
	myStack.push(word1);
	myQueue.push(myStack);

	// creates two sets: one for the dictionary and one for the tested words
	string token;

	set<string> myDictionary(dictionary.begin(), dictionary.end());
	set<string> testedWords;
	bool wordFound = false;


		// while the queue is not empty:
		while (!(myQueue.empty())) {

			// dequeue the partial-ladder stack from the front of the queue.
			stack<string> ladder = myQueue.front();
			myQueue.pop();
			string word = ladder.top();

			// if the word on top of the stack is the destination word:
			if (word == word2) {
				int number_In_Ladder = 0;
				wordFound = true;
				// Yeey! output the elements of the stack as the solution.
				std::cout << "\nThe ladder from " << word1 << " to " << word2 << " takes ";

				//Copy the ladder stack to worldladder to take it in the order.
				while (!ladder.empty()) {
					wordladder.push(ladder.top());
					ladder.pop();
				}
				if (wordladder.size() == 2) {
					cout << wordladder.size() - 1 << " step.\n";
				}
				else {
					cout << wordladder.size() - 1 << " steps.\n";
				}
				while (!wordladder.empty()) {
					number_In_Ladder++;
					std::cout << wordladder.top();
					wordladder.pop();
					
					if (wordladder.empty()) {
						break;
					}
					else std::cout << " > ";
				}
				while (!myQueue.empty()){
					myQueue.pop();
				}
				auto end = chrono::steady_clock::now();
				cout << "\nWord ladder took " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms " << "to find. " << endl;
				cout << "---------------------------------";
			}
			else {
				// for each valid English word that is a "neighbor" (differs by 1 letter) of the word on top of the stack:
				string test;
				for (unsigned int i = 0; i < word.size(); i++) {
					for (char j = 'a'; j <= 'z'; j++) {
						test = word.substr(0, i) + j + word.substr(i + 1);

						// if that word is an english word
						if (myDictionary.count(test)) {

							// if that neighbor word has not already been used in a ladder before:
							if (!testedWords.count(test)) {

								// create a copy of the current ladder stack.
								stack<string> copy = ladder;

								// put the neighbor word on top of the copy stack.
								copy.push(test);

								// add the copy stack to the end of the queue.
								myQueue.push(copy);
							}
						}

						// Add test to tested words because it is already used.
						testedWords.insert(test);
					}
				}
			}
		}
		if (myQueue.empty()) {
			if (wordFound == false) {
				std::cout << "No word ladder found";
				auto end = chrono::steady_clock::now();
				cout << endl << "Operation took: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms ";

			}
		}
}