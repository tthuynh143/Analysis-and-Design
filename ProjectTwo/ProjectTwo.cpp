#include <iostream>

#include <fstream>

#include <vector>

using namespace std;

//Definition of the structure course

struct Course{

string courseNumber;

string name;

vector<string> prerequisites;

};

//Function to split the string to the list of strings on the basis of the given delimiter

vector<string> tokenize(string s, string del = " ")

{

vector<string> stringArray;

int start = 0;

int end = s.find(del);

while (end != -1) {

stringArray.push_back(s.substr(start, end - start));

start = end + del.size();

end = s.find(del, start);

}

stringArray.push_back(s.substr(start, end - start));

return stringArray;

}

//Function to load the file and store the details into the list of courses

vector<Course> LoadDataStructure()

{

//Creating the object of iftsream class to open the file

ifstream fin("abcu.txt",ios::in);

vector<Course> courses;

string line;

//Infinite loop

while(1)

{

getline(fin,line);

//If the end of the file is reached then break the loop

if(line=="-1")

break;

Course course;

//Getting the tokenized information which is separated by the commas

vector<string> tokenizedInformation=tokenize(line,",");

//Storing the information in the structure course

course.courseNumber=tokenizedInformation[0];

course.name =tokenizedInformation[1];

//If there is prerequisites then make sure to store them too

for(int i=2;i<tokenizedInformation.size();i++)

{

course.prerequisites.push_back(tokenizedInformation[i]);

}

//Appending the course into the list of courses

courses.push_back(course);

}

//Closing file

fin.close();

//Return list of courses

return courses;

}

//Printing the course info of the given course in the proper format

void printCourse(Course course)

{

string courseNumber= course.courseNumber;

string name=course.name;

vector<string> prerequisites=course.prerequisites;

cout<<"Course Number: "<<courseNumber<<endl;

cout<<"Course Name: "<<name<<endl;

cout<<"Prerequisites: ";

for(int i=0;i<prerequisites.size();i++)

{

cout<<prerequisites[i]<<" ";

}

cout<<"\n\n";

}

//Printing the course information of all the courses in the proper format

void printCourseList(vector<Course> courses)

{

int n=courses.size();

//Using the bubble sort to sort all of the list

for(int i=0;i<n-1;i++)

{

for(int j=0;j<n-i-1;j++)

{

if(courses[j].courseNumber > courses[j+1].courseNumber)

{

swap(courses[j+1],courses[j]);

}

}

}

//Traversing the list of courses to print all of the courses info

for(int i=0;i<n;i++)

{

printCourse(courses[i]);

}

}

//Search course for the entered course number

void searchCourse(vector<Course> courses)

{

int n=courses.size();

string courseNumber;

int f=0;

cout<<"Enter the Course Number: ";

cin>>courseNumber;

for(int i=0;i<n;i++)

{

//If course is found then print

if(courses[i].courseNumber==courseNumber)

{

f=1;

printCourse(courses[i]);

break;

}

}

//If the course with the given course name are not found then print the error

if(f==0)

{

cout<<"The course with the given course number is not found!\n";

}

}

int main(int argc, char **argv)

{

vector<Course> courses;

//Print menu

cout<<"1.Load the Data Structure\n";

cout<<"2.Print the Course List\n";

cout<<"3.Print the Course\n";

cout<<"4.Exit!\n";

int ch;

//Loop will break once the user enters 4

do{

//Prompt the user to enter their choice

cout<<"\nEnter your choice: ";

cin>>ch;

switch(ch)

{

case 1: courses=LoadDataStructure(); break;

case 2: printCourseList(courses); break;

case 3: searchCourse(courses); break;

case 4: cout<<"Exit!\n"; break;

default: cout<<"Sorry, Invalid Choice! Try Again!\n";

}

}while(ch!=4);

return 0;

}}
