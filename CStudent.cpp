//CStudent.cpp
// by Mikhail 01/27/2024 (but getting close the 28th)
// here you're taking the course information for one instance of CStudent in the dynamic vector Course1, i think

#include "stdafx.h" // must be added above all code in external .ccp files when using the template
#include "lab2.h" 

CStudent::CStudent(std::string asnum, float alab, float aquiz, float aMT, float aFexam, float aFinalGrade) { 
	setsnum(asnum);
	setlab(alab);
	setquiz(aquiz);
	setMT(aMT);
	setFexam(aFexam);
	setFinalGrade(aFinalGrade);
} /// since there are no getters, only setters here, it's clear this function will be used only to set an individual student

CStudent::~CStudent() {
	// Destructor implementation... not needed... self-evidenced
}

///////////////////////////////////////////////////////////////////////
// next, the getters and setters defined.
// note that cuz CStudent is the class-type used in he MyCourse instance of Course1, the student index is implied
///////////////////////////////////////////////////////////////////////
void CStudent::setsnum(const std::string& asnum) {
	snum = asnum; 
}

std::string CStudent::getsnum() const {
	return snum; // clearly a getter
}

void CStudent::setlab(const float& alab) {
	lab = alab;
}

float CStudent::getlab() const {
	return lab;
}

void CStudent::setquiz(const float& aquiz) {
	quiz = aquiz;
}

float CStudent::getquiz() const {
	return quiz;
}

void CStudent::setMT(const float& aMT) {
	MT = aMT;
}

float CStudent::getMT() const {
	return MT;
}

void CStudent::setFexam(const float& aFexam) {
	Fexam = aFexam;
}

float CStudent::getFexam() const {
	return Fexam;
}

void CStudent::setFinalGrade(const float& aFinalGrade) {
	FinalGrade = aFinalGrade;
}

float CStudent::getFinalGrade() const {
	return FinalGrade;
}