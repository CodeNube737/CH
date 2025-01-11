#pragma once
/////////////////////////////////////////////////////////////////////////////
// header file for lab 1 - User input (aux for lab2)
// mikhail rego 01/21/2024
/////////////////////////////////////////////////////////////////////////////


#ifndef LAB1_H_
#define LAB1_H_

#define MAX_STUDENTS 100
#define lab_weight 0.40
#define quiz_weight 0.10
#define MT_weight 0.20
#define final_weight 0.30

#include <iostream>
#include <cstring>

struct student
{
	int index;
	char StudentNumber1[10];
	float lab1;
	float quiz1;
	float MT1;
	float final1;
	float total1;
};

// global variables and constants
student students[MAX_STUDENTS]; // Array to store student information
int number_of_students = 0; // counts # of students in "struct Students"
char input; // user input

extern void lab1_print_menu();
extern void lab1_edit_student(int);
extern void lab1_add_student();
extern void lab1_print_grades();
extern char quit();
//extern void lab1();
//extern void findStudentIndex(const char*, const student, int);


#endif LAB1_H_