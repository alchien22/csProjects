//  Created by Alex Chien on 7/26/22.

#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

//Global variables
const int NUM_STUDENTS = 4;
const int NUM_TESTS = 5;

//Structure type Student
struct Student{
    string name;
    int id;
    int scores[NUM_TESTS];
    float average;
    char grade;
};

//Prototypes
void getInfo(Student []);
void calcAverage(Student []);
void getGrade(Student []);
void display(Student []);
void classAvg(Student []);
void studentHigh(Student []);

int main(int argc, const char * argv[]) {
    
    Student stats[NUM_TESTS];
    
    getInfo(stats);
    
    calcAverage(stats);
    
    getGrade(stats);
    
    //Table labels
    cout << setw(20) << left;
    cout << setw(10) << "\nName";
    cout << setw(10) << right << "Test 1";
    cout << setw(10) << "Test 2";
    cout << setw(10) << "Test 3";
    cout << setw(10) << "Test 4";
    cout << setw(10) << "Test 5";
    cout << setw(10) << "Average";
    cout << setw(10) << "Grade" << endl;
    
    display(stats);
    
    cout << endl;
    
    classAvg(stats);
    
    studentHigh(stats);
   
    return 0;
}

//Gets info from a txt file and stores in arrays
void getInfo(Student a[]){
    ifstream infile;
    
    infile.open("stats.txt");
    
    for (int i=0; i<NUM_STUDENTS; i++){
        getline(infile, a[i].name);
        cout << "Student Name: " << a[i].name << endl;
        infile >> a[i].id;
        cout << "Student ID: " << a[i].id << endl;
        
        for (int j=0; j<NUM_TESTS; j++){
            infile >> a[i].scores[j];
            cout << "Test " << j+1 << ": " << a[i].scores[j] << endl;
        }
        cout << endl;
        infile.get();
    }
    infile.close();
}

//Calculates the total and average for each student after eliminated high and low scores
void calcAverage(Student a[]){
    int total;
    int sHigh[NUM_STUDENTS], sLow[NUM_STUDENTS];
    
    for (int i=0; i<NUM_STUDENTS; i++){
        total=0;
        sHigh[i]=0;
        sLow[i]=0;
        for (int j=0; j<NUM_TESTS; j++){
            total += a[i].scores[j];
            if (a[i].scores[j]>a[i].scores[sHigh[i]]){
                sHigh[i]=j;
            }
            if (a[i].scores[j]<a[i].scores[sLow[i]]){
                sLow[i]=j;
            }
        }
        total = total - (a[i].scores[sHigh[i]] + a[i].scores[sLow[i]]);
        a[i].average = static_cast<float>(total) / (NUM_TESTS-2);
    }
}

//Determines the letter grade based on average per person
void getGrade(Student a[]){
    for (int i=0; i<NUM_STUDENTS; i++){
        if (a[i].average>=90){
            a[i].grade='A';
        } else if (a[i].average>=80){
            a[i].grade='B';
        } else if (a[i].average>=70){
            a[i].grade='C';
        } else if (a[i].average>=60){
            a[i].grade='D';
        } else {
            a[i].grade='F';
        }
    }
}
    
//Determines the class average based on individual average grades
void classAvg(Student a[]){
    float classAvg;
    int classTotal=0;
    
    for (int i=0; i<NUM_STUDENTS; i++){
        classTotal+=a[i].average;
    }
    
    classAvg = classTotal / NUM_STUDENTS;
    
    cout << "Class Average: " << classAvg << endl;
}

//Display results in a table to user
void display(Student a[]){
    for (int i=0; i<NUM_STUDENTS; i++){
        cout << setw(15) << left << a[i].name;
        for (int j=0; j<NUM_TESTS; j++){
            cout << setw(10) << a[i].scores[j];
        }
        cout << setprecision(2) << fixed << setw(10) << a[i].average;
        cout << setw(10) << a[i].grade;
        cout << endl;
    }
}

//Determines and displays the student with the highest average grade 
void studentHigh(Student a[]){
    float avgH=0;
    int highestAvg=0;
    
    for (int i=0; i<NUM_STUDENTS; i++){
        if (a[i].average>avgH){
            avgH=a[i].average;
            highestAvg=i;
        }
    }
    cout << a[highestAvg].name << " (ID: " << a[highestAvg].id << ") had the highest average: " << a[highestAvg].average << endl;
}
