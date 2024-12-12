#include<vector>
#include<set>
#include<iostream>
#include<fstream>
#include<string>
using namespace std;

void fillBoard(vector<vector<set<int>>> &Board) {
	string line;
	ifstream boardFile("A.PUZZLE"); // Get the file to be used

	char temp; // Char variable used to store current character
	int setX = 0, setY = 0; // Position variables to place values into the vector
	set<int> tempSet, fullSet; // Set used to contain numbers, and a full set to put into empty spaces
	for (int i = 0; i < 9; i++) fullSet.insert(i + 1); // Create a full set to be used for empty spaces
	if (boardFile) { // If the file was successfully opened

		for (int i = 0; i < Board.size() + 2; i++, setY = 0) { // For every line in the file (the size of the board, plus 2 for the - characters that differentiate each line
			getline(boardFile, line); // Get the line from the file
			for (int j = 0; j <  line.length(); j++) { // For every character in the line

				temp = line[j]; // Get each character
				if (temp == ' ') { // If the character is an empty space
					Board[setX][setY] = fullSet; // Set the set in the board to full
					setY++; // Increment the y position to set
				}

				else if (temp == '-') { // If the character is a dash (indicates breaks in the sudoku puzzle)
					setX--; // Decrement x position by one (since for loop increments it by one, this makes sure no lines are skipped and no vector out of range error)
					break; // Exit the current loop for y position, as there will never be a character to add
				} 

				else if (temp == '|') continue; // If the character is a vertical line, skip it (used to indicate columns in sudoku puzzle)

				else { // If the character is a number 
					tempSet.insert(temp - 48); // Insert the integer value of the character
					Board[setX][setY] = tempSet; // Set the board set's value to that number
					tempSet.clear(); // Clear the temp set
					setY++; // Increment the y value by 1
				}
			}
			setX++; // Increment the x value by 1
		}
	}
	boardFile.close(); // Close the file for safety measures
}

void checkBoard(vector<vector<set<int>>> &Board) {
	bool filled = false; // Used to check if any changes are made
	int temp; // Track current cell's digit
	while (filled == false) { // While changes are being made
		int x = 0, y = 0; // Track the current cell
		filled = true; // No changes have been made in this iterator
		for (x = 0; x < 9; x++, y == 0) { // For every row
			for (y = 0; y < 9; y++) { // For every column in each row
				if (Board[x][y].size() == 1) { // Check if the current cell is of size one (there is only one possible digit)

					// Check the cell's row
					for (int checkY = 0; checkY < 9; checkY++) { // For every item in the row
						temp = *Board[x][y].begin(); // Get the current cell's digit
						if (y != checkY && Board[x][checkY].size() > 1) { // If the cell to check is not the current cell and has more than 1 possible digit
							Board[x][checkY].erase(temp); // Remove the current cell's digit from checked cell's possible digits
							filled = false; // Changes have been made in this iteration
						}
					}

					// Check the cell's column
					for (int checkX = 0; checkX < 9; checkX++) { // For every item in the column
						temp = *Board[x][y].begin(); // Get the current cell's digit
						if (x != checkX && Board[checkX][y].size() > 1) { // If the cell to check is not the current cell and has more than 1 possible digit
							Board[checkX][y].erase(temp); // Remove the current cell's digit from the checked cell's possible digits
							filled = false; // Changes have been made in this iterator
						}
					}

					// Check the cell's 3x3 mini grid
					for (int checkX = 3*(x / 3); checkX < (3*(x / 3) + 3); checkX++) { // For every item in the cell's 3x3 mini grid (each row)
						for (int checkY = 3*(y / 3); checkY < (3*(y / 3) + 3); checkY++) { // For every item in the cell's 3x3 mini grid (each column)
							temp = *Board[x][y].begin(); // Get the current cell's digit
							if (!(x == checkX && y == checkY) && Board[checkX][checkY].size() > 1) { // If the cell to check is not the current cell and has more than one digit
								Board[checkX][checkY].erase(temp); // Remove the current cell's digit from the checked cell's possible digits
								filled = false; // Changes have been made this iteration
							}
						}
					}
				}
			}
		}
	}
}

void outputBoard(vector<vector<set<int>>> &Board) {
	for (int i = 0; i < Board.size(); i++) { // For every column
		if (i % 3 == 0 && i != 0) cout << "---+---+---" << endl; // Separate every 3 rows with a line
		for (int j = 0; j < Board[0].size(); j++) { // For every row in each row
			if (j % 3 == 0 && j != 0) cout << "|"; // Separate every 3 columns with a line
			if (Board[i][j].size() > 1)	cout << " ";  // If the cell has more than 1 possible digit, output a space
			else cout << *(Board[i][j].begin()); // Else, output the item in the cell
		}
		cout << endl;
	}
}

int main() {

	vector<vector<set<int>>> Board; // Create a vector of vectors // Create a new board
	Board.resize(9, vector<set<int>>(9)); // Set the size to 9x9
	fillBoard(Board); // Fill the board
	cout << "Original Board: " << endl << endl;
	outputBoard(Board); // Output the board before it is solved
	checkBoard(Board); // Solve the board
	cout << endl << "~~~~~~~~~~~~~~~~~~~~~" << endl << "Cleared Board: " << endl << endl;
	outputBoard(Board); // Output thhe board after it is solved
	cout << endl;

	system("pause");
	return 0;
}