/*                                                                                                                  
Isaac Kiiza

CS 142, Prof. Chuck Torpe, Fall 2023                                                                                 
File Viewer
    *** Open a file and display it on the window, <text>[n]
    *** Commands to scroll up or down and to open a new file
    *** Designed to have a "Buffer" class that handles data storage and display
    *** Designed to have a main routine that handles the use interface
    *** Key data storage decision: store the file as a vector of strings, where Each string is a full line of text
    */
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;

//Aesthetics
const string DASHES (85, '-');
const string SHORT_DASH (25, '-');

// Buffer class definition
class Buffer {
public:
  // Accessors methods 
void read(string &fname); // read a file into the
void display(); // manage the display
void set_height(int h) {height = h;} // how many rows high to display 
void next(); // move to next screen (scroll down)  
void previous(); // move to previous screen (scroll up) 
void jump(int);
void set_error(string s) {error_message = s;}
private:
  vector <string> data;
  int height;
  int topline = 0; // what line is currently displayed at the top of the window 
  string filename = "";
  string error_message = "";
  int anchorNumber = 0; // anchor initiation
  };
void Buffer::jump(int newline){
  newline --;
  if (topline < 0) topline = 0;
  else if(newline <= data.size()) topline = newline;
  else topline = data.size();
}

void Buffer::previous() {
  topline -= height;
  if (topline < 0) topline = 0;
}

//let the topline go up to the end of the file 

void Buffer::next() {
  topline += height;
  if (topline > data.size()) topline = data.size();
}

void Buffer::display() {
  int bottom;
  cout << static_cast<char> (12); // clear the screen    
  cout << error_message << endl;
  error_message = "";
  cout << filename << endl;
  cout << DASHES << endl;

  if (height + topline <= data.size()) bottom = height + topline;
  else bottom = data.size();

  for (int i = topline; i < bottom; i++) {
    string line = data[i];
    size_t anchorPos = line.find("<");
    while (anchorPos != string::npos) {
      size_t endAnchorPos = line.find(">", anchorPos);
      if (endAnchorPos != string::npos) {
// get the anchor text   

        string anchorText = line.substr(anchorPos, endAnchorPos - anchorPos + 1);
        size_t anchorNumber = stoi(anchorText.substr(1, anchorText.size() - 2));  // change the anchor                                                                                                                                               
        line.replace(anchorPos, endAnchorPos - anchorPos + 1, "<" + to_string(anchorNumber) + ">");
      }
      anchorPos = line.find("<", anchorPos + 1);
    }
    cout << setw(4) << i + 1 << " " << line << endl;
  }
  cout << DASHES << endl;
}

void Buffer::read(string &fname) {
    ifstream in(fname.c_str());
    if (in.fail()) {
      error_message = "Can't open " + fname;
      return;
    }
    string mystring;
    data.clear();
    topline = 0;
    anchorNumber = 0; //initiate anchor number to the new file 
    //convenient way to locate <a and >  
    while (getline(in, mystring)) {
      size_t anchorPos = mystring.find("<a");
      while (anchorPos != string::npos) {
        anchorNumber++;
        size_t endAnchorPos = mystring.find(">", anchorPos);
        if (endAnchorPos != string::npos) {
          // Replace the anchor  
           mystring.replace(anchorPos, endAnchorPos - anchorPos + 1, "<" + to_string(anchorNumber) + ">");
        }
        anchorPos = mystring.find("<a", anchorPos + 1);
      }
      data.push_back(mystring);
    }
    cout << "Read in " << data.size() << " lines" << endl;
    filename = fname;
  }
}
// The user interface part of the file viewer
// Opens a file to get things started, then loops until quit   
// each time through the loop executes one command by calls to   
// Buffer member functions, then displays current page 
int main (){
  Buffer mybuff;
  int h;
  char ans;
  bool again = true;
  cout << "How high ";
  cin >> h;
  mybuff.set_height(h);

  mybuff.display();

  while (again) {
    cout << "Jump Next Previous Open Quit\n";
    cout << SHORT_DASH << endl;
    cin >> ans;
    switch (ans) {
    case 'o':
    case 'O':
      {
      string fname;
      cout << "What file ";
      cin >> fname;
      mybuff.read(fname);
      }
      break;
    case 'j':
    case 'J':
      {
        int newline;
        cout<< "What line ";
        cin >> newline;
        mybuff.jump(newline);
      }
      break;
    case 'n':
    case 'N':
      mybuff.next();
      break;
    case 'p':
    case 'P':
      mybuff.previous();
      break;
    case 'q':
    case 'Q':
      again = false;
      break;
    case 'g':
    case 'G':
      {
        int linkNumber;
        cout << "Link number: ";
        cin >> linkNumber;
        cin.ignore(); // Consume the newline character 
        if (linkNumber >= 1 && linkNumber <= mybuff.getAnchorCount()) {
          string fname = "file_for_anchor_" + to_string(linkNumber) + ".txt"; // Replace with your logic to map anchor numbers to file names  
          if (!fname.empty()) {
            mybuff.read(fname);
          }
          else {
            mybuff.set_error("No file associated with anchor " + to_string(linkNumber));
          }
        }
        else {
          mybuff.set_error("Invalid anchor number");
        }
      }
      break;
    default:
      {
        string temp;
        temp = ans;
      mybuff.set_error(temp + " is not a valid command");
      }
    }
  mybuff.display();
  }
}


