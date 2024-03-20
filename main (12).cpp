
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <map>
using namespace std;
vector<int> readDimensions(ifstream& file);
vector<vector<char>> readMatrix(ifstream& file, int rows, int cols);
vector<string> readWords(ifstream& file);
void printMatrix(const vector<vector<char>>& matrix);
vector<string> processMovieTitles(const vector<string>& titles);
void searchInAllDirections(const vector<vector<char>>& matrix, const vector<string>& search_words, vector<vector<bool>>& foundPositions);
void createPPMFile(const vector<vector<char>>& matrix, const vector<vector<bool>>& foundPositions);
ifstream openFile() {
 ifstream file;
 string filename;
 while(true) {
 cout << "Enter filename (or 'quit' to exit): ";
 cin >> filename;
 
 if(filename == "quit") {
 cout << "Exiting word search" << endl;
 exit(0); // Exit program if user wants to quit
 }
 
 file.open(filename);
 
 if(file) {
 cout << "--- The file name is: " << filename << endl; 
 return file; // Return the opened file
 } else {
 cout << "Oops, can't open the specified input file: No such file or directory" << endl;
 cout << "The file name used was: " << filename << endl;
 }
 }
}
vector<int> readDimensions(ifstream& file) {
 string line;
 // Skip empty lines and comments to find dimensions
 while (getline(file, line)) {
 if (line.empty() || line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;
 break;
 }
 stringstream ss(line);
 int rows, cols;
 ss >> rows >> cols;
 return {rows, cols}; // Returns a vector with two elements: rows and columns
}
vector<vector<char>> readMatrix(ifstream& file, int rows, int cols) {
 vector<vector<char>> matrix(rows, vector<char>(cols));
 string line;
 while (getline(file, line)) {
 if (line.empty() || line[0] == '#' || line[0] == '\r' || line[0] == '\n') continue; // Skip empty lines and comments
 file.seekg(-int(line.length()) - 1, ios_base::cur);
 break;
 }
 for (int i = 0; i < rows; ++i) {
 getline(file, line);
 for (int j = 0; j < cols && j < line.size(); ++j) {
 matrix[i][j] = line[j];
 }
 }
 return matrix;
}
vector<string> readWords(ifstream& file) {
 vector<string> words;
 string line;
 // Skip to content after matrix
 while (getline(file, line)) {
 if (line.empty() || line[0] == '#' || line[0] == '\r' || line[0] == '\n') continue; // Skip empty lines and comments
 file.seekg(-int(line.length()) - 1, ios_base::cur);
 break;
 }
 // Read movie titles or words
 while (getline(file, line)) {
 if (!line.empty() && line[0] != '#') words.push_back(line);
 }
 return words;
}
void printMatrix(const vector<vector<char>>& matrix) {
 cout << "Matrix:" << endl;
 for (const auto& row : matrix) {
 for (char cell : row) cout << cell;
 cout << endl;
 }
 cout << endl;
}
/*void printWords(const vector<string>& words) {
 cout << "Movie Titles:" << endl;
 for (const string& word : words) cout << word << endl;
}*/
// Processes movie titles by removing spaces and converting to uppercase
vector<string> processMovieTitles(const vector<string>& titles) {
 
 vector<string> search_words;
 
 for(const string& title: titles){
 string temp;
 for(char ch: title){
 if(ch != ' '){
 temp += toupper(ch);
 }
 }
 search_words.push_back(temp);
 }
 
 return search_words;
 /*for(const string& search_word: search_words){
 cout << search_word << endl;
 }*/
 
}
// Checks if a word can be found starting from (startRow, startCol) going north
bool North(const vector<vector<char>>& matrix, int startRow, int startCol, const string& word) {
 for (int k = 0; k < word.length(); ++k) {
 if (startRow - k < 0 || matrix[startRow - k][startCol] != word[k]) {
 return false;
 }
 }
 return true;
}
// Checks if a word can be found starting from (startRow, startCol) going S
bool South(const vector<vector<char>>& matrix, int startRow, int startCol, const string& word) {
 for (int k = 0; k < word.length(); ++k) {
 if (startRow + k >= matrix.size() || matrix[startRow + k][startCol] != word[k]) {
 return false;
 }
 }
 return true;
}
// Checks if a word can be found starting from (startRow, startCol) going E
bool East(const vector<vector<char>>& matrix, int startRow, int startCol, const string& word) {
 for (int k = 0; k < word.length(); ++k) {
 if (startCol + k >= matrix[0].size() || matrix[startRow][startCol + k] != word[k]) {
 return false;
 }
 }
 return true;
}
// Checks if a word can be found starting from (startRow, startCol) going W
bool West(const vector<vector<char>>& matrix, int startRow, int startCol, const string& word) {
 for (int k = 0; k < word.length(); ++k) {
 if (startCol - k < 0 || matrix[startRow][startCol - k] != word[k]) {
 return false;
 }
 }
 return true;
}
// Checks if a word can be found starting from (startRow, startCol) going NE
bool NorthEast(const vector<vector<char>>& matrix, int startRow, int startCol, const string& word) {
 for (int k = 0; k < word.length(); ++k) {
 if (startRow - k < 0 || startCol + k >= matrix[0].size() || matrix[startRow - k][startCol + k] != word[k]) {
 return false;
 }
 }
 return true;
}
// Checks if a word can be found starting from (startRow, startCol) going NW
bool NorthWest(const vector<vector<char>>& matrix, int startRow, int startCol, const string& word) {
 for (int k = 0; k < word.length(); ++k) {
 if (startRow - k < 0 || startCol - k < 0 || matrix[startRow - k][startCol - k] != word[k]) {
 return false;
 }
 }
 return true;
}
// Checks if a word can be found starting from (startRow, startCol) going SE
bool SouthEast(const vector<vector<char>>& matrix, int startRow, int startCol, const string& word) {
 for (int k = 0; k < word.length(); ++k) {
 if (startRow + k >= matrix.size() || startCol + k >= matrix[0].size() || matrix[startRow + k][startCol + k] != word[k]) {
 return false;
 }
 }
 return true;
}
// Checks if a word can be found starting from (startRow, startCol) going SW
bool SouthWest(const vector<vector<char>>& matrix, int startRow, int startCol, const string& word) {
 for (int k = 0; k < word.length(); ++k) {
 if (startRow + k >= matrix.size() || startCol - k < 0 || matrix[startRow + k][startCol - k] != word[k]) {
 return false;
 }
 }
 return true;
}
// Creates a PPM file to visualize the search results
void createPPMFile(const vector<vector<char>>& matrix, const map<string, bool>& foundWords, const vector<vector<bool>>& foundPositions) {
 ofstream ppmFile("output.ppm");
 ppmFile << "P3\n"; // PPM file format
 ppmFile << matrix[0].size() << " " << matrix.size() << "\n"; // Width and Height
 ppmFile << "255\n"; // Maximum color value
 for (int i = 0; i < matrix.size(); i++) {
 for (int j = 0; j < matrix[i].size(); j++) {
 if (foundPositions[i][j]) {
 ppmFile << "255 0 0 "; // Red for found letters
 } else {
 ppmFile << "0 0 0 "; // Black for everything else
 }
 }
 ppmFile << "\n";
 }
 ppmFile.close();
 cout << "PPM file created." << endl;
}
// Searches for each word in all directions and updates found positions
void searchInAllDirections(const vector<vector<char>>& matrix, const vector<string>& search_words, vector<vector<bool>>& foundPositions) {
 int rows = matrix.size();
 int cols = matrix[0].size(); // Assuming the matrix is non-empty
 map<string, bool> foundWords;
 for (const string& word : search_words) {
 foundWords[word] = false;
 }
 foundPositions = vector<vector<bool>>(rows, vector<bool>(cols, false));
 for (const string& search_for : search_words) {
 bool isFound = false;
 for (int i = 0; i < rows && !isFound; ++i) {
 for (int j = 0; j < cols && !isFound; ++j) {
 char ch = toupper(matrix[i][j]);
 if (ch == search_for[0]) {
 // Check in each direction
if (North(matrix, i, j, search_for)) {
 isFound = true;
 for (int k = 0; k < search_for.length(); ++k) {
 foundPositions[i - k][j] = true;
 }
 } else if (South(matrix, i, j, search_for)) {
 isFound = true;
 for (int k = 0; k < search_for.length(); ++k) {
 foundPositions[i + k][j] = true;
 }
 } else if (East(matrix, i, j, search_for)) {
 isFound = true;
 for (int k = 0; k < search_for.length(); ++k) {
 foundPositions[i][j + k] = true;
 }
 } else if (West(matrix, i, j, search_for)) {
 isFound = true;
 for (int k = 0; k < search_for.length(); ++k) {
 foundPositions[i][j - k] = true;
 }
 } else if (NorthEast(matrix, i, j, search_for)) {
 isFound = true;
 for (int k = 0; k < search_for.length(); ++k) {
 foundPositions[i - k][j + k] = true;
 }
 } else if (NorthWest(matrix, i, j, search_for)) {
 isFound = true;
 for (int k = 0; k < search_for.length(); ++k) {
 foundPositions[i - k][j - k] = true;
 }
 } else if (SouthEast(matrix, i, j, search_for)) {
 isFound = true;
 for (int k = 0; k < search_for.length(); ++k) {
 foundPositions[i + k][j + k] = true;
 }
 } else if (SouthWest(matrix, i, j, search_for)) {
 isFound = true;
 for (int k = 0; k < search_for.length(); ++k) {
 foundPositions[i + k][j - k] = true;
 }
 }
 if (isFound) {
 cout << search_for << " found starting at (" << (i+1) << ", " << (j+1) << ")" << endl;
 foundWords[search_for] = true;
 }
 }
 }
 }
 if (!isFound) {
 foundWords[search_for] = false;
 }
 }
 // After the search, output words not found and create the PPM file
 cout << "\nWords not found:" << endl;
 for (const auto& word : foundWords) {
 if (!word.second) {
 cout << word.first << endl;
 }
 }
 createPPMFile(matrix, foundWords, foundPositions);
}
string formatter(const string& word) {
 string formattedWord;
 for (char ch : word) {
 formattedWord += toupper(ch); // converting to uppercase
 }
 return formattedWord;
}
int main() {
 ifstream file = openFile();
 vector<int> dimensions = readDimensions(file);
 int rows = dimensions[0];
 int cols = dimensions[1];
 cout << "Rows: " << rows << ", Columns: " << cols << endl; // Print out the rows and columns
 cout << "\n";
 cout << "\n";
 auto matrix = readMatrix(file, rows, cols); // Use the variables for clarity
 auto words = readWords(file);
 file.close();
 printMatrix(matrix);
 //printWords(words); // Commented out as per your setup; uncomment if needed
 cout << "\n";
 vector<string> search_words = processMovieTitles(words);
 vector<vector<bool>> foundPositions; // To track positions of found letters
 searchInAllDirections(matrix, search_words, foundPositions);
 // Search for each word in all directions
 
 return 0;
}