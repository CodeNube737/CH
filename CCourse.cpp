//CCourse.cpp
// by Mikhail 01/28/2024
/// most involved file of them all...

#include "stdafx.h" // must be added above all code in external .ccp files when using the template
#include "lab2.h" 

using namespace std; // meh

CCourse::CCourse() {} // don't fully get the logic, but if this isn't defined as doing nothing here, all hell breaks loose

float CCourse::getValidNumericInput() {
    float value;
    string input;
    while (true) {
        cin >> input;
        //getline(cin, input);
        stringstream ss(input);
        if (!(ss >> value) || value < 0 || value > 100 || !ss.eof()) {
            cout << "Invalid input. Please enter a numeric value between 0 and 100." << endl;
        }
        else {
            break;
        }
    }
    return value;
}




float CCourse::getValidStudentIndex() { 
    float value;
    while (true) {
        cin >> value;
        if (cin.fail() || value <= 0 || value >= (Course1.size() + 1)) {
            cout << "Invalid input. Please enter a valid index." << endl;
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        }
        else {
            break; /// this is a great way to use while loops
        }
    }
    return value;
}
std::string CCourse::getValidStudentNumber() { 
    std::regex pattern("(A00|A01)\\d{6}");  // A00 or A01 followed by exactly 6 digits // beautifully done
    std::string snum;
    while (true) {
        cin >> snum;
        cout << "\n";
        if (regex_match(snum, pattern)) {
            return snum;
            break;
        }
        else {
            cout << "Invalid number" << endl;
        }
    }
}

// i wonder if u can edit the next 2 functions to resemble your lab1 program more closely?
void CCourse::add_student() {
    std::string snum;
    float lab, quiz, MT, Fexam, FinalGrade;

    cout << "Student Number: ";
    snum = getValidStudentNumber();
    cout << "\n";

    cout << "Lab Grade: ";
    lab = getValidNumericInput();
    cout << "\n";

    cout << "quiz Grade: ";
    quiz = getValidNumericInput();
    cout << "\n";

    cout << "MT Grade: ";
    MT = getValidNumericInput(); 
    cout << "\n";

    cout << "Final Grade: ";
    Fexam = getValidNumericInput();
    cout << "\n";


    FinalGrade = (lab_weight * lab) + (quiz_weight * quiz) + (MT_weight * MT) + (final_weight * Fexam); // so here, i made the weights constants

    CStudent Student = CStudent(snum, lab, quiz, MT, Fexam, FinalGrade); /// all the setting in 2 steps
    Course1.push_back(Student); // adding student to the back of the Course vector: VERY IMPORTANT
}

void CCourse::edit_student() {
    int i;
    std::string snum;
    float lab, quiz, MT, Fexam, FinalGrade;

    if (!Course1.size()) // if no students have been entered to the vector Course1 (it's size is zero), u can't edit any students
        cout << "\nThere are no sutdents to edit\n\n";
    else
    {
        cout << "\nStudent to edit: ";
        i = getValidStudentIndex(); // getter functions TYP
        i--;

        cout << "\nStudent Number: ";
        snum = getValidStudentNumber();
        //cout << "\n";
        Course1[i].setsnum(snum); // love the use of setter functions TYP

        cout << "\nlab Grade: ";
        lab = getValidNumericInput();
        Course1[i].setlab(lab);
        //cout << "\n";

        cout << "\nQuiz Grade: ";
        quiz = getValidNumericInput();
        Course1[i].setquiz(quiz);
        //cout << "\n";

        cout << "\nMidterm Grade: ";
        MT = getValidNumericInput();
        Course1[i].setMT(MT);
        //cout << "\n";

        cout << "\nFinal Exam Grade: ";
        Fexam = getValidNumericInput();
        Course1[i].setFexam(Fexam);
        //cout << "\n";
        Course1[i].setFinalGrade((lab_weight * lab) + (quiz_weight * quiz) + (MT_weight * MT) + (final_weight * Fexam));
    }
}

void CCourse::delete_student() { 
    int i;
    if (!Course1.size()) // if no students have been entered to the vector Course1 (it's size is zero), u can't edit any students
        cout << "\nThere are no students to delete\n\n";
    else
    {
        cout << "\nStudent to Delete: ";

        i = getValidStudentIndex() - 1;

        if (i >= 0 && i < Course1.size()) {
            Course1.erase(Course1.begin() + i);
            cout << "\nStudent deleted successfully." << endl;
        }
    }
}

void CCourse::print_grades() {
    cout << "#    Student    Lab    Quiz    Midterm    Final Exam    Final Grade \n";
    std::cout.precision(1);
    int track = Course1.size();
    for (int i = 0; i < track; i++) { // for loop to iterate each print of a student
        cout << i + 1 << "   ";
        cout << Course1[i].getsnum() << "   ";
        cout << std::fixed << Course1[i].getlab() << "   ";
        cout << std::fixed << Course1[i].getquiz() << "    ";
        cout << std::fixed << Course1[i].getMT() << "       ";
        cout << std::fixed << Course1[i].getFexam() << "          ";
        cout << std::fixed << Course1[i].getFinalGrade() << endl; /// endline and go to index i++ student
    }
}


// and understand that it deletes the .txt file after loading it. (This can be clearly fixed in the load_class() function)
void CCourse::load_class() {
    Course1.clear();              //this clears the CCourse1 vector so that when you load a file, none of the previous info is
    std::string input;
    cout << "\nName of Load File (ignore .type): ";
    cin >> input;
    input += memoryFileType();

    std::ifstream inputFile(input);

    if (inputFile.is_open()) {
        std::string snum;
        float lab, quiz, MT, Fexam, FinalGrade;

        while (inputFile >> snum >> lab >> quiz >> MT >> Fexam >> FinalGrade) { /// notice, this is in the exact same order as outfile()
            Course1.push_back(CStudent(snum, lab, quiz, MT, Fexam, FinalGrade));
        }
        inputFile.close();
        /* i removed these lines and just cout'd the success line... deleting was not worth it
        // Remove the file after loading its content
        if (remove(input.c_str()) != 0) {
            std::cerr << "Error loading the file!" << std::endl;
        }
        else {
            std::cout << "File successfully loaded." << std::endl;
        }
        */
        std::cout << "File successfully loaded." << std::endl;
    }
    else {
        std::cerr << "Error loading the file!" << std::endl;
    }
}

void CCourse::save_class() {          //this will write to an external file
    int track = Course1.size();
    std::string output;
    cout << "\nName of Save File (ignore .type): ";
    cin >> output;
    output += memoryFileType();
    ofstream outfile(output); //,ios::app add this in the brackets to only add data
    for (int i = 0; i < track; i++) {
        if (outfile.fail())
        {
            cout << "file error" << endl;
        }
        else
        {
            outfile << Course1[i].getsnum() << " "
                << Course1[i].getlab() << " "
                << Course1[i].getquiz() << " "
                << Course1[i].getMT() << " "
                << Course1[i].getFexam() << " "
                << Course1[i].getFinalGrade() << "\n"; /// i would prefer if this printed-out in a new line
        }
    }
    cout << "file was written" << endl;
}

////////// in .cpp file
std::string CCourse::memoryFileType() {
    char input;
    std::string type;
    while (true) {
        cout << "\n\nChoose file-type for save/load:\n";
        cout << "(J) .json file\n";
        cout << "(T) .txt  file\n";
        cin >> input;
        input = toupper(input);
        if (input == 'J')
        {
            type = ".json";
            break;
        }
        else if (input == 'T')
        {
            type = ".txt";
            break;
        }
        else
            cout << "\ninvalid input";
    }
    return type;
}