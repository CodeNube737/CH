#pragma once
//lab2.h
// by Mikhail 01/27/2024

#include <iostream> 
#include <vector> 
#include <string> 
#include <cstring>
#include <fstream> 
#include <regex> 
#include <sstream>
#include <iomanip>

#define lab_weight 0.40
#define quiz_weight 0.10
#define MT_weight 0.20
#define final_weight 0.30

class CStudent { 
private: 
	std::string snum; 
	float lab; 
	float quiz; 
	float MT; 
	float Fexam; 
	float FinalGrade; 

public: 
	CStudent(std::string asnum, float alab, float aquiz, float aMT, float aFexam, float aFinalGrade); 
	~CStudent(); 

	void setsnum(const std::string& asnum); 
	std::string getsnum() const; 

	void setlab(const float& alab); 
	float getlab() const; 

	void setquiz(const float& aquiz); 
	float getquiz() const; 

	void setMT(const float& aMT); 
	float getMT() const; 

	void setFexam(const float& aFexam); 
	float getFexam() const;

	void setFinalGrade(const float& aFinalGrade); 
	float getFinalGrade() const; 
};


class CCourse { 
private: 
	std::vector<CStudent> Course1; 

public: 
	CCourse(); 
	float getValidNumericInput();
	float getValidStudentIndex(); 
	std::string getValidStudentNumber(); 
	void add_student(); 
	void edit_student(); 
	void delete_student(); 
	void print_grades(); 
	void load_class(); 
	void save_class(); 
	////////// in .h file, in the CCourse class, public:
	std::string memoryFileType(); // returns either 'J' for .JSON or 'T' for .txt
};