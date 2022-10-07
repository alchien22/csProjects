//
//  Final.cpp
//  Final
//
//  Created by Alex Chien on 7/27/22.
//
//  I decided to make the debugging display a part of the menu so that I could check if changes were updating accordingly if any were made to the profile
//  Video: https://youtu.be/Kx7dMosFsR0

#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

//Data structure and variables
struct Member{
    string name;
    int age;
    int diff;
    int gender;
    int hobby[4];
    string contact;
};

//Function prototypes
void intro();
void userInfo(string hobbySelection[], string &name, int &age, int &genderS, int hobbyS[], int HOBBY);
void getInfo(Member x[], int USERS, int HOBBY);
int checkProfile(string hobbySelection[], string &name, int &age, int &genderS, int hobbyS[], int HOBBY);
string yesNoHobby(int x);
string gender(int x);
int memberDisplay(Member x[], int USERS, int HOBBY, string hobbySelection[], int hobbyS[], string &name, int &age, int &genderS);
int criterionCheck(Member x[], int USERS, int HOBBY, int &age, int &genderS, int hobbyS[], int criterionCount[], string hobbySelection[]);
bool isValid(int x, int min, int max);

int main(int argc, const char * argv[]) {
    //Declare main variables
    const int USERS=5;
    const int HOBBY=4;
    string hobbySelection[4]={"Play Videogames", "Read a Book", "Play a Sport", "Go Hiking"};
    string name;
    int age=0;
    int genderS=0;
    int hobbyS[HOBBY];
    int menuS, profile=0;
    int criterionCount[5];
    int min=1, max=4;
    int back=0;
    int next=0;
    
    Member info[USERS];
    
    //Gets information from the members from .txt document
    getInfo(info, USERS, HOBBY);
  
    //Introduction message
    intro();
    
    //Gets user profile information
    userInfo(hobbySelection, name, age, genderS, hobbyS, HOBBY);
    
    //Resets back to the main menu unless program is exited, a match is made, or the user doesn't select from the top 2 matches
    do{
        //Loop allows for re-display of menu
        do{
            cout << "\n1. Display My Profile\n";
            cout << "2. Find My Match\n";
            cout << "3. Debug\n";
            cout << "4. Quit\n";
            cin >> menuS;
        } while(!isValid(menuS, min, max));
        
        switch (menuS){
            //Allows user to go back to profile and make any changes
            case 1:
                profile = checkProfile(hobbySelection, name, age, genderS, hobbyS, HOBBY);
                switch(profile){
                    case 1:
                        cout << "\nReturning to Main Menu...\n";
                        break;
                    case 2:
                        userInfo(hobbySelection, name, age, genderS, hobbyS, HOBBY);
                        break;
                }
                break;
            //Determines top 2 matches for user and allows user to choose which of the top two to choose and receives their respective contact information (email)
            case 2:
                next = criterionCheck(info, USERS, HOBBY, age, genderS, hobbyS, criterionCount, hobbySelection);
                if (next==1){
                    cout << "Thank you for matching with us today!\n";
                } else if (next==2){
                    cout << "Unfortunately, we have no more matches for you. Try again next time!\n";
                }
                break;
            //Displays user and member information for debugging purposes
            case 3:
                back=memberDisplay(info, USERS, HOBBY, hobbySelection, hobbyS, name, age, genderS);
                switch(back){
                    case 1:
                        cout << "\nHeading back to the main menu...\n";
                        break;
                    case 2:
                        cout << "\nEnding program...\n";
                        break;
                }
                break;
            //Ends switch statement and program
            case 4:
                cout << "\nEnding program...\n";
                break;
        }
    } while(menuS!=4 && menuS!=2 && back!=2);
    
    //Program end message display
    cout << "\nThank you so much " << name << " for coming to LuvBird!\nWe hope to see you again!\n";
    
    return 0;
}

//Welcome message display
void intro(){
    cout << "Welcome to LuvBird, where matters of the heart matter most!" << endl;
    cout << "This matchmaking program will help you find and connect with singles from ages 20 to 50 looking for love as well!" << endl;
    cout << "\nTell us about yourself!\n";
}

//Gets user information and allows changes later on
void userInfo(string hobbySelection[], string &name, int &age, int &genderS, int hobbyS[], int HOBBY){
    int repeat, min=1, max=2;
    
    do{
        cout << "\nPlease enter your name: ";
        getline(cin, name);
        
        cout << "Age: ";
        cin >> age;
        
        do{
            cout << "Who would you be most interested in dating?\n";
            cout << "1. Male" << endl;
            cout << "2. Female" << endl;
            cin >> genderS;
        } while(!isValid(genderS, min, max));
        
        
        for (int i=0; i<HOBBY; i++){
            do{
                cout << "Would you enjoy participating in the following activity: " << hobbySelection[i] << "?" << endl;
                cout << "1. Yes\n";
                cout << "2. No\n";
                cin >> hobbyS[i];
            } while(!isValid(hobbyS[i], min, max));
        }
        //Allows for changes to be made to the user profile
        repeat = checkProfile(hobbySelection, name, age, genderS, hobbyS, HOBBY);
    } while (repeat!=1);
    
    cout << "Generating your perfect match...\n";
    cout << "Press any key to continue...";
    system("read");
}

//Gets member information from txt file and stores in data structure arrays
void getInfo(Member x[], int USERS, int HOBBY){
    fstream infile;
    
    infile.open("matches.txt");
    
    for (int i=0; i<USERS; i++){
        getline(infile, x[i].name);
        infile >> x[i].age;
        infile >> x[i].gender;
        for (int j=0; j<HOBBY; j++){
            infile >> x[i].hobby[j];
        }
        infile.get();
        getline(infile, x[i].contact);
    }
    infile.close();
}

//Allows user to make and save profile changes
int checkProfile(string hobbySelection[], string &name, int &age, int &genderS, int hobbyS[], int HOBBY){
    int confirm, min=1, max=2;
    
    cout << "\nIs the following information correct?\n\n";
    cout << "Name: " << name << endl;
    cout << "Age: " << age << endl;
    cout << "Gender Preference: " << gender(genderS) << endl;
    for (int i=0; i<HOBBY; i++){
        cout << "Likes to " << hobbySelection[i] << ": " << yesNoHobby(hobbyS[i]) << endl;
    }
    
    do{
        cout << "\n1. The information above is correct.\n";
        cout << "2. Edit my profile.\n";
        cin >> confirm;
    } while(!isValid(confirm, min, max));
    
    cin.ignore();
    return confirm;
}

//Determines yes/no given an integer input
string yesNoHobby(int x){
    if (x==1){
        return "Yes";
    } else {
        return "No";
    }
}

//Determines gender given an integer input
string gender(int x){
    if (x==1){
        return "Male";
    } else {
        return "Female";
    }
}

//Display user and memeber profile information to be used for debugging
int memberDisplay(Member x[], int USERS, int HOBBY, string hobbySelection[], int hobbyS[], string &name, int &age, int &genderS){
    int back, min=1, max=2;
    
    cout << "\nName: " << name << endl;
    cout << "Age: " << age << endl;
    cout << "Gender Preference: " << gender(genderS) << endl;
    for (int i=0; i<HOBBY; i++){
        cout << "Likes to " << hobbySelection[i] << ": " << yesNoHobby(hobbyS[i]) << endl;
    }
    
    //Table headers
    cout << setw(15) << left;
    cout << setw(10) << "\nName";
    cout << setw(14) << right << "Age";
    cout << setw(17) << "Gender";
    for (int i=0; i<HOBBY; i++){
        cout << setw(20) << hobbySelection[i];
    }
    cout << setw(20) << "Contact";
    cout << endl;
    
    //Display info for each member in txt file in a table
    for (int i=0; i<USERS; i++){
        cout << setw(15) << left;
        cout << setw(20) << x[i].name;
        cout << setw(15) << x[i].age;
        cout << setw(15) << gender(x[i].gender);
        for (int j=0; j<HOBBY; j++){
            cout << setw(20) << yesNoHobby(x[i].hobby[j]);
        }
        cout << setw(15) << x[i].contact;
        cout << endl;
    }
    
    do{
        cout << "\n1. Return to Main Menu";
        cout << "\n2. Quit\n";
        cin >> back;
    } while(!isValid(back, min, max));
    
    
    return back;
}

//Determines 1st and 2nd best match based on # of similar characteristics
int criterionCheck(Member x[], int USER, int HOBBY, int &age, int &genderS, int hobbyS[], int criterionCount[], string hobbySelection[]){
    int count, high=0, choice1=0, choice2=0, indexC=0, next, min=1, max=2, diff1=0, diff2=0, ageDiff=100, indexD=0;
    
    //Find age difference
    for (int i=0; i<USER; i++){
        x[i].diff = abs(age-x[i].age);
    }
    
    //Lowest age difference
    for (int i=0; i<USER; i++){
        if (x[i].diff<ageDiff){
            ageDiff=x[i].diff;
            diff1=i;
            indexD=i;
        }
    }
    x[indexD].diff=100;
    ageDiff=100;
    
    //Find 2nd lowest age difference
    for (int i=0; i<USER; i++){
        if (x[i].diff<ageDiff){
            ageDiff=x[i].diff;
            diff2=i;
        }
    }
    
    //Add points for each similar item and lower age gap
    for (int i=0; i<USER; i++){
        count=0;
        if (x[i].diff==x[diff1].diff){
            count++;
        }
        if (genderS==x[i].gender){
            count++;
        }
        for (int j=0; j<HOBBY; j++){
            if (hobbyS[i]==x[i].hobby[j]){
                count++;
            }
        }
        criterionCount[i] = count;
    }
    
    //Find most compatible
    for (int i=0; i<USER; i++){
        if (criterionCount[i]>high){
            high=criterionCount[i];
            choice1=i;
            indexC=i;
        }
    }
    criterionCount[indexC]=0;
    high=0;
    criterionCount[diff1]++;
    
    //Find 2nd most compatible
    for (int i=0; i<USER; i++){
        if (criterionCount[i]>high){
            high=criterionCount[i];
            choice2=i;
        }
    }
    
    cout << "\n\nHere is your top match: "; //Display top match
    cout << "\nName: " << x[choice1].name;
    cout << "\nAge: " << x[choice1].age;
    cout << "\nGender: " << gender(x[choice1].gender);
    for (int i=0; i<HOBBY; i++){
        cout << "\nEnjoys to" << hobbySelection[i] << ": " << yesNoHobby(x[choice1].hobby[i]);
    }
    
    do{
        cout << "\n\n1. I Accept the Match";
        cout << "\n2. Next Best Match\n\n";
        cin >> next;
    } while(!isValid(next, min, max));
    
    if (next==1){
        cout << x[choice1].contact << endl << endl;
    } else{
        cout << "\nHere is your next top match: "; //Display next top match
        cout << "\n\nName: " << x[choice2].name;
        cout << "\nAge: " << x[choice2].age;
        cout << "\nGender: " << gender(x[choice2].gender);
        for (int i=0; i<HOBBY; i++){
            cout << "\nEnjoys to " << hobbySelection[i] << ": " << yesNoHobby(x[choice2].hobby[i]);
        }
        
        do{
            cout << "\n\nDo you accept the match?";
            cout << "\n1. Yes\n";
            cout << "2. No\n\n";
            cin >> next;
        } while(!isValid(next, min, max));
        
        if (next==1){
            cout << x[choice2].contact << endl << endl;
        }
    }
    return next;
}

//Checks if entered integer input is valid
bool isValid(int x, int min, int max){
    if (x<min || x>max){
        cout << "\nInvalid Input. Please try again.\n";
        return false;
    } else{
        return true;
    }
}

