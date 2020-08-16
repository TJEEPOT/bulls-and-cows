## What It Does ##
This was my first full programming project, set in C++. The program simulates the game [Bulls and Cows](https://en.wikipedia.org/wiki/Bulls_and_Cows), also known as Mastermind in the UK. The idea is that one player creates a number (in this case, a nine digit binary number) and the other must guess it. Upon every guess, the player who created the number tells the other player how many of the guessed digits were correct and in the correct position, and how many digits were present but not in the correct position. The game is concluded when the guessing player either guesses the correct number or makes seven incorrect guesses in a row.
To fulfil the design requirements for this project, I needed to allow the user to both guess a randomised number, as well as provide a number for the computer to guess.

## What I Learned ##
* The basics of C++ (Basic I/O, stringStream, random number generation, functions, etc)
* Utilisation of provided code (bitset)
* Debugging in Visual Studio
* Good coding practices including layout and commenting

## Usage Notes ##
Compile as usual with GCC or G++.