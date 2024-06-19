#include <iostream>  // Input/output stream operations
#include <string>    // String operations
#include <sstream>   // String stream operations
#include <fstream>   // File input/output operations
#include <vector>    // Vector container
#include <algorithm> // Algorithms
#include <iomanip>   // Input/output manipulators

#include "utilities.cpp"

using namespace std;

string STUDENTS_FILE = "students.csv";

struct Student
{
    string student_id;
    string student_name;
    vector<string> courses;
    string registration_date;
};

// Function to compare students based on student_name for sorting
bool compareStudents(const Student &student1, const Student &student2)
{
    return student1.student_name < student2.student_name;
}

// Function to list all students
void listStudents()
{
    ifstream file = openInputFile(STUDENTS_FILE);
    string student;
    vector<Student> students;

    // Read students from file and store them in a vector
    while (getline(file, student))
    {
        vector<string> row = splitString(student, ',');

        Student newStudent;
        newStudent.student_id = row[0];
        newStudent.student_name = row[1];
        newStudent.registration_date = row[3];

        vector<string> courses = splitString(row[2], ';');
        newStudent.courses = courses;

        students.push_back(newStudent);
    }

    // Check if there are any students
    if (students.empty())
    {
        cout << "No students found" << endl;
    }
    else
    {
        // Sort the students based on student_name
        sort(students.begin(), students.end(), compareStudents);

        // Display each student's details
        for (const auto &student : students)
        {
            cout << "Student ID:" << setw(2) << student.student_id << "            "
                 << "Student Name:" << setw(10) << student.student_name << "                "
                 << "Courses: ";
            for (const auto &course : student.courses)
            {
                cout << course << " ";
            }
            cout << "     Reg Date :" << student.registration_date << endl << endl;
        }
    }

    file.close();
}

// Function to add a new student
void addStudent(const string &student_id, const string &student_name, const vector<string> &courses, const string &registration_date)
{
    // Validate the input data format
    if (!isNumeric(student_id) || !isValidDateFormat(registration_date))
    {
        cout << "Invalid data format. Please enter the following format: <student_id as a number> <student_name> <course1;course2;...> <registration_date as date in YYYY-mm-dd>" << endl;
        return;
    }

    // Open the students file for reading to check for duplicates
    ifstream inputFile = openInputFile(STUDENTS_FILE);

    // Check if the student with the given student_id already exists
    if (checkDuplicate(inputFile, student_id, 0))
    {
        cout << "Student with id: " << student_id << " already exists." << endl;
        inputFile.close();
        return;
    }

    inputFile.close();

    // Check if the courses are valid
    if (!areValidCourses(courses))
    {
        cout << "One or more courses are invalid. Please make sure all courses are available." << endl;
        return;
    }

    // Open the students file in append mode to add the new student
    ofstream file(STUDENTS_FILE, ios::app);
    file << student_id << "," << student_name << ",";
    for (size_t i = 0; i < courses.size(); i++)
    {
        file << courses[i];
        if (i < courses.size() - 1)
        {
            file << ";";
        }
    }
    file << "," << registration_date << endl;
    cout << "Student is added successfully." << endl;
    file.close();
}


// Function to delete a student based on student_id
void deleteStudent(const string &student_id)
{
    ifstream inputFile = openInputFile(STUDENTS_FILE);
    ofstream tempFile("temp.csv");  // Temporary file to write non-deleted records

    string student;
    bool found = false;

    while (getline(inputFile, student))
    {
        vector<string> row = splitString(student, ',');

        if (row.size() > 0 && row[0] == student_id)
        {
            found = true;
            continue;  // Skip writing this student's record to temp file
        }

        tempFile << student << endl;  // Write the record to temp file
    }

    inputFile.close();
    tempFile.close();

    if (found)
    {
        // Remove the original file and rename the temporary file
        remove(STUDENTS_FILE.c_str());
        rename("temp.csv", STUDENTS_FILE.c_str());
        cout << "Student with ID " << student_id << " deleted successfully." << endl;
    }
    else
    {
        cout << "Student with ID " << student_id << " not found." << endl;
        remove("temp.csv");  // Delete the temporary file
    }
}



// Function to display the help menu
void help()
{
    cout << " ===========================================================================" << endl;
    cout << " *                            Command syntaxes                              *" << endl;
    cout << " ===========================================================================" << endl;
    cout << " studentadd <student_id> <student_name> <course1;course2;...> <registration_date>" << endl;
    cout << " studentslist                                              :List all students" << endl;
    cout << " courseslist                                               :List all available courses" << endl;
    cout << " help                                                      :Prints user manual" << endl;
    cout << " exit                                                      :Exit the program" << endl;
    cout << " =============================================================================" << endl;
}



// Function to display an invalid command message
void invalidCommand(const string &correctCommand)
{
    cout << "Invalid command. Usage: " << correctCommand << endl;
}



// Function to update student ID based on current student_id
void updateStudentID(const string &current_student_id, const string &new_student_id)
{
    ifstream inputFile = openInputFile(STUDENTS_FILE);
    ofstream tempFile("temp.csv");  // Temporary file to write updated records

    string student;
    bool found = false;

    while (getline(inputFile, student))
    {
        vector<string> row = splitString(student, ',');

        if (row.size() > 0 && row[0] == current_student_id)
        {
            // Update student_id with new_student_id
            row[0] = new_student_id;
            student = row[0] + "," + row[1] + "," + row[2] + "," + row[3];
            found = true;
        }

        tempFile << student << endl;  // Write the record (updated or not) to temp file
    }

    inputFile.close();
    tempFile.close();

    if (found)
    {
        // Remove the original file and rename the temporary file
        remove(STUDENTS_FILE.c_str());
        rename("temp.csv", STUDENTS_FILE.c_str());
        cout << "Student ID updated successfully." << endl;
    }
    else
    {
        cout << "Student with ID " << current_student_id << " not found." << endl;
        remove("temp.csv");  // Delete the temporary file
    }
}



// Function to list all available courses =======
void listCourses() 
{
    ifstream file = openInputFile(COURSES_FILE);
    string course;

    cout << "Available Courses:" << endl;
    while (getline(file, course))
    {
        cout << course << endl;
    }

    file.close();
}

// Function to process user commands
void processCommands()
{
    cout << "Need help? Type 'help' then press Enter key." << endl;

    string input;
    string command;

    do
    {
        cout << "Console >";
        getline(cin, input);

        istringstream iss(input);
        iss >> command;

        // Convert the command to lowercase for case-insensitive comparison
        for (size_t i = 0; i < command.length(); i++)
        {
            command[i] = tolower(command[i]);
        }
        // Process the command
        if (command == "studentslist")
        {
            cout << "------------------------------------------------------------------------------------------------------------------" << endl;

            listStudents();
            cout << "------------------------------------------------------------------------------------------------------------------" << endl;
        }
        else if (command == "studentadd")
        {
            string student_id, student_name, courses_input, registration_date;
            iss >> student_id >> student_name >> courses_input >> registration_date;

            if (student_id.empty() || student_name.empty() || courses_input.empty() || registration_date.empty())
            {
                invalidCommand("studentadd <student_id> <student_name> <course1;course2;...> <registration_date>");
            }
            else
            {
                vector<string> courses = splitString(courses_input, ';');
                addStudent(student_id, student_name, courses, registration_date);
            }
        }
        

        else if (command == "help")
        {
            help();
        }
        
        
        // Modify processCommands() function to handle 'courseslist' command
		else if (command == "courseslist")
		{
		    cout << "------------------------------------------------------------------------------------------------------------------" << endl;
		
		    listCourses();
		
		    cout << "------------------------------------------------------------------------------------------------------------------" << endl;
		}
        else
        {
            // Invalid command entered
            if (command != "exit")
            {
                cout << "Invalid command. Below is the help option to provide you a list of commands." << endl;
                help();
            }
        }
    } while (command != "exit");
}


int main()
{
    cout << " ==================================================== " << endl;
    cout << "*     Welcome to the Student Management System!     *" << endl;
    cout << "*  **************************************** *" << endl;
    cout << "*                           *" << endl;
    cout << "* It is developed by 'NKUBITO Pacis' as practical *" << endl;
    cout << "* evaluation for the end of Year 3.               *" << endl;
    cout << " ====================================================" << endl;

    // Start processing user commands
    processCommands();
    return 0;
}
