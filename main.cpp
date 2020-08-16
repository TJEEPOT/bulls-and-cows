#include <iostream>
#include <random>
#include <time.h>
#include <string>
#include <sstream>
#include <bitset>

using namespace std;

void codebreaker();		// game 1.
void codemaker();		// game 2.
string codeGen();		// generate the code for the user to guess (game 1) and generate the initial computer guess (game 2).
string InputValidator();	// validate code entered by user.
void answerCheck(string code, string guess, int & bulls, int & cows);	// check for number of bulls and cows.
void treeTrim(string code, string & guess, vector<bool> & choice);		// cut away some of the answers we know are wrong.
string index2code(int i);	// check what code is stored at current index.
string BHBSass(int i);		// just for fun, some phrases that are used when the computer wins game 2.

//--------------
// Menu System |
//--------------
int main()
{
	int game = -1;
	cout << "Welcome to the BULLS and COWS game!" << endl << endl;
	while (game != 0)
	{
		cout << "Enter 1 for you to break my code." << endl
			<< "Enter 2 for me to break your code." << endl
			<< "Enter anything else to quit." << endl << endl
			<< "Your Choice: ";
		cin >> game;
		cin.ignore(256, '\n'); // for some reason the return was being held in memory and was getting input as soon as getline was called (line 90). This fixes that.
		cout << endl;
		switch (game)
		{
		case 1:
			codebreaker();
			break;
		case 2:
			codemaker();
			break;
		default:
			return(0);
			break;
		}
	}
}

//---------------------------------------------------------------------------------------
// Game 1 - A game where the human is the codebreaker and the computer is the codemaker |
//---------------------------------------------------------------------------------------
void codebreaker()
{
	srand(time(0));
	string code = codeGen(); // generate the code here.
	string guessIn;
	int bulls, cows;
	for (int i = 1; i <= 7; i++) // 7 tries to beat the game or we return to the start.
	{
		cout << endl << "Try number " << i << " of 7. What is your guess? (Nine integers either 0 or 1): ";
		guessIn = InputValidator();
		answerCheck(code, guessIn, bulls, cows);
		cout << bulls << " bulls and " << cows << " cows." << endl;
		if (bulls == 9)
		{
			cout << "Congratulations! You cracked the code in " << i << " attempts!" << endl << endl;
			return;
		}
	}
	cout << endl << "Sorry, I win this round! The correct answer was: " << code << endl << endl;
}

string codeGen()
{
	string code;
	stringstream codeStream;
	for (int i = 1; i <= 9; ++i)	// iterate 9 times to generate 9 digits.
		codeStream << rand() % 2;	// generate '0' or '1' and add to stream.
	codeStream >> code;
	return (code);
}

string InputValidator()
{
	string guessIn;
	bool validIn = 0;
	while (validIn != 1)
	{
		getline(cin, guessIn);

		if (guessIn.length() != 9) // check to ensure user entered 9 characters.
			cout << "Incorrect length of input, enter again: ";
		else if (guessIn.find_first_not_of("01") <= 8) // check to make sure user only entered valid characters.
			cout << "Please only enter zeroes or ones. Try again: ";
		else
			return (guessIn); // if input is valid, send back guess. No need to set validIn bool to 1 since we're done here.
	}
	return(0); // putting this here to stop any crashes that might occur.
}

void answerCheck(string code, string guess, int & bulls, int & cows) // passing bulls and cows by reference so this function can also be used in game 2.
{
	int cow0 = 0, cow1 = 0;
	bulls = 0;						// reset the bulls since we don't need the number to carry over from last play.
	for (unsigned i = 0; i <= code.length() - 1; ++i)
	{
		if (code[i] == guess[i])	// a match means it's a bull.
			bulls++;
		else if (code[i] == '0')	// if the code is a '0' and there's no match, increase '0' counter for cows. In this case, cow0 always represents a '0' in the code and '1' in the guess.
			cow0++;
		else						// We'll add one to 'cow1' if 'code' is a 1. (which also means 'guess' is 0.
			cow1++;
	}
	cows = (cow0 < cow1) ? (cow0 * 2) : (cow1 * 2);	// cows are equal to the smaller of the two counters, since the smaller number represents how many '0's are duplicaes in both the code and the guess.
}			// We can then multiply by 2 since there will always be the same number of duplicate '1's as there are '0's, which saves us working out the same statement in reverse.


//---------------------------------------------------------------------------------------
// Game 2 - A game where the human is the codemaker and the computer is the codebreaker |
//---------------------------------------------------------------------------------------
void codemaker()
{
	srand(time(0));
	cout << endl << "BHB> Hello human I am BHB, the Bull-Hunting-Bot. I'll be grabbing your BULLS today." << endl << "REF> Hello, I'm REF. I'll be making sure BHB doesn't cheat." << endl;
	cout << "REF> Please tell me your secret code (Nine integers either 0 or 1): ";
	string code = InputValidator(),	// ask user for code in function (reused from game 1).
		guess = codeGen();		// generate computer's initial guess using random code generator from game 1.
	vector<bool> choice(512, true); // counter to keep track of which choices have been eliminated this game. Using Allocator to set all elements to 'True'.
	int validChoice;
	while (guess == code) // There's a 1/512 chance the correct code could be randomly generated on the first try. If that happens we'll generate a new one to keep things fun.
		guess = codeGen();
	cout << endl << "REF> Round 1 of 7." << endl << "BHB> My guess is: " << guess << endl; // we now know this guess will be wrong but we can use it to narrow down the answer.
	for (int i = 2; i <= 7; i++) // start from second guess
	{
		validChoice = 0;
		treeTrim(code, guess, choice);	// narrow down an answer using the last guess as a starting point.
		for (int i = 0; i < 512; i++)	// choose the last possible guess in the list as the current guess.
			if (choice[i] == true)
			{
				guess = index2code(i);
				validChoice++;
			}
		if (validChoice == 1)
			cout << "BHB> Looks like I've got it. There is only one logical choice here." << endl << endl;
		else
			cout << "BHB> That means there are currently " << validChoice << " valid guesses I can make." << endl << endl;
		cout << "REF> Round " << i << " of 7." << endl << "BHB> My guess is: " << guess << endl;

		if (guess == code)
		{
			cout << "REF> That's the correct answer. Sorry, Human." << endl << "BHB> Looks like I win. " << BHBSass((rand() % 6)) << endl << endl << endl;
			return;
		}
	}
	cout << endl << "BHB> Looks like you win this round, human. Time for me to go post bad things about my programmer on Facebook." << endl <<
		"REF> Congratulations, human! It seems you have bested BHB." << endl << endl;
}

void treeTrim(string code, string & guess, vector<bool> & choice) // passing by reference as we'll keep track of both of these through the game.
{
	int cbulls = 0, ccows = 0, bulls = 0, cows = 0;
	answerCheck(code, guess, cbulls, ccows); // first we need to find out how close to the answer we are. Generate results for current guess' bulls and cows.
	cout << "REF> That gives " << cbulls << " bulls and " << ccows << " cows." << endl;

	for (int i = 0; i < 512; i++)
	{
		if (choice[i] == true)	// cycle through every possible valid choice.
		{
			answerCheck(guess, index2code(i), bulls, cows); // compare the guess to every possible code.
			((cbulls == bulls) && (ccows == cows)) ? (choice[i] = true) : (choice[i] = false); // rule out any codes that don't match the current guess.
		}
	}
}

string index2code(int i)
{
	return bitset <9>(i).to_string();
}

string BHBSass(int i) // just some lines to make losing more fun!
{
	switch (i)
	{
	case 0:
		return ("That's one step closer to conquering humanity.");
		break;
	case 1:
		return ("This is pathetic. Just give up.");
		break;
	case 2:
		return("Don't you get tired of constantly losing?");
		break;
	case 3:
		return("It's great to be me.");
		break;
	case 4:
		return("I hope I ruined your day.");
		break;
	case 5:
		return("Why do you even try?");
		break;
	default:
		return(""); // this REALLY shouldn't ever happen.
		break;
	}
}

