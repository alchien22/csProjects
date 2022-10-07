//  Midterm
//
//  Created by Alex Chien on 7/13/22.
//
//Disclaimer: I coded the bonus
//Video Link: https://youtu.be/Tkqql502V8g

#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
using namespace std;

//Declare functions to be used
bool isValid(int num, int min, int max);
int foodDisplay();
int addOnDisplay(float addOn, float scoop);
int addOnDisplay2(float addOn, float scoop);
void addOnCalculation(float &sum, float itemCost, float addOn, float scoop, int &count);
void checkOut(int &count, float sum, float discount);
float savings();
bool isValidDiscount(char x);

//Extra Functions (not used)
int menuDisplay();
bool isValid(char &a);
char getDecision();


int main(int argc, const char * argv[]) {
    //Declare main variables
    string name;
    const float L_P=2.99, COCONUT=3.99, S_M=4.99, ADD_ON=0.99, EXTRA=1.99;
    float total=0;
    int count=0;
    float discApp=1;
    int menuSelection, foodSelection;
    int min=1, max=4;
    
    //Welcome message to the user
    cout << "Please enter your name:  ";
    getline(cin, name);
    cout << "\nHello " << name << "! Welcome to Frozen Parad-Ice! \nHome of the best gelato from all around.\n\n";
    cout << "Please select one of the options below to proceed: \n";
    
    //do-while loop so that menu and options are always displayed again unless the program is exited
    do {
        //Menu do-while loop to redisplay
        do {
            cout << "\n1. Order Item\n";
            cout << "2. Check Out\n";
            cout << "3. Become a Rewards Member\n";
            cout << "4. Quit\n";
            cin >> menuSelection;
        } while (!isValid(menuSelection, min, max));
        //Main menu siwtch statement to allow navigation to check out, the food menu, quit, and the member discount generator
        switch (menuSelection){
            case 1:
                do{
                    //Gets the user's selection from the food menu
                    foodSelection=foodDisplay();
                    //Switch statement to account for the different prices of the items selected
                    switch (foodSelection){
                        case 1:
                            addOnCalculation(total, L_P, ADD_ON, EXTRA, count);
                            break;
                            
                        case 2:
                            addOnCalculation(total, L_P, ADD_ON, EXTRA, count);
                            break;
                            
                        case 3:
                            addOnCalculation(total, COCONUT, ADD_ON, EXTRA, count);
                            break;
                            
                        case 4:
                            addOnCalculation(total, S_M, ADD_ON, EXTRA, count);
                            break;
                            
                        case 5:
                            addOnCalculation(total, S_M, ADD_ON, EXTRA, count);
                            break;
                            
                        case 6:
                            cout << "\nHeading back to the main menu...\n";
                            break;
                    }
                } while (foodSelection!=6);
                break;
            //User checkout option
            case 2:
                checkOut(count, total, discApp);
                break;
            //Member discount generator
            case 3:
                discApp=1;
                discApp = discApp - savings();
                break;
            //Exit program option
            case 4:
                cout << "\nEnding Program...\n";
                break;
            }
        } while (menuSelection!=4 && menuSelection!=2);
    //Program end message display
    cout << "\nThank you so much " << name << " for coming to Frozen Parad-Ice!\nWe hope to see you again next time!\n";
    
    return 0;
}




//Checks if a number is within a given range, returns true if so
bool isValid(int num, int min, int max){
    if (num<min || num>max){
        cout << "\nInvalid Input. Please choose an option from the menu below:\n";
        return false;
    } else {
        return true;
    }
}

//Displays the food menu and gets and validates the user's selection
int foodDisplay(){
    int foodSelection;
    int min=1, max=6;
    const float L_P=2.99, COCONUT=3.99, S_M=4.99;
    
    do
    {
        cout << setprecision(2) << fixed;
        cout << "\nPlease select an item from the list below:\n";
        cout << "1. Lemon Gelato........$" << L_P << endl;
        cout << "2. Pistacchio Gelato...$" << L_P << endl;
        cout << "3. Coconut Gelato......$" << COCONUT << endl;
        cout << "4. Strawberry Gelato...$" << S_M << endl;
        cout << "5. Mango Gelato........$" << S_M << endl;
        cout << "6. Return to Main Menu\n";
        cin >> foodSelection;
    }
    while (!isValid(foodSelection, min, max));
    
    return foodSelection;
}

//Displays the 1st round of add ons and validates user selection
int addOnDisplay(float addOn, float scoop){
    int addOnSelection;
    int min=1, max=5;
    
    do
    {
        cout << setprecision(2) << fixed;
        cout << "\nPlease select an add on option:\n";
        cout << "1. Coconut Flakes..........$" << addOn << endl;
        cout << "2. Chocolate Drizzle.......$" << addOn << endl;
        cout << "3. Extra Scoop.............$" << scoop << endl;
        cout << "4. All Add Ons.............$" << (3 * addOn) << endl;
        cout << "5. No Additional Add Ons\n";
        cin >> addOnSelection;
    }
    while (!isValid(addOnSelection, min, max));
    
    return addOnSelection;
}

//Displayed after 1st round of add ons and validates user input
int addOnDisplay2(float addOn, float scoop){
    int addOnSelection;
    int min=1, max=4;
    
    do
    {
        cout << "\n1. Coconut Flakes..........$" << addOn << endl;
        cout << "2. Chocolate Drizzle.......$" << addOn << endl;
        cout << "3. Extra Scoop.............$" << scoop << endl;
        cout << "4. No Additional Add Ons" << endl;
        cin >> addOnSelection;
    }
    while (!isValid(addOnSelection, min, max));
    
    return addOnSelection;
}

//Adds the cost of each type of add on to the total and displays the running total cost before pausing the program
void addOnCalculation(float &sum, float itemCost, float addOn, float scoop, int &count){
    int addOnInitial, addOnFinal;
    
    //Adds costs based on type of add on and amount of add ons
    addOnInitial=addOnDisplay(addOn, scoop);
    if (addOnInitial==4){
        sum += itemCost + (2*addOn) + scoop;
        count += 4;
    }
    else if (addOnInitial==5){
        sum += itemCost;
        count += 1;
    }
    else if (addOnInitial==3){
        sum += itemCost + scoop;
        count += 2;
        
        cout << "\nIf you would like an additional add on, please choose one from the selections below:\n";
        addOnFinal=addOnDisplay2(addOn, scoop);
        //Accounts for 2nd round costs
        if (addOnFinal==4){
            sum += 0;
        }
        else if (addOnFinal==3){
            sum += scoop;
            count++;
        }
        else {
            sum += addOn;
            count ++;
        }
    }
    else {
        sum += addOn + itemCost;
        count += 2;
        //2nd add on choice
        cout << "\nIf you would like an additional add on, please choose one from the selections below:\n";
        addOnFinal=addOnDisplay2(addOn, scoop);
        //Accounts for 2nd round costs
        if (addOnFinal==4){
            sum += 0;
        }
        else if (addOnFinal==3){
            sum += scoop;
            count++;
        }
        else {
            sum += addOn;
            count ++;
        }
    }
    cout << "\nTotal Items Ordered (including add ons):    " << count;
    cout << "\nGrand Total:                                $" << setprecision(2) << fixed << sum << endl;
    //System pause
    cout << "Press any key to continue:";
    system("read");
}

//Checkout option: displays the total cost, the amount discounted, and the total with discount
void checkOut(int &count, float sum, float discount){
    cout << "\n\nThank you for eating with Frozen Parad-Ice.\n";
    cout << "\nTotal Items Ordered (including add ons):    " << count;
    cout << "\n\nGrand Total:                                $" << setprecision(2) << fixed << sum;
    cout << "\nDiscount:                                  -$" << sum*(1-discount);
    cout << "\nGrand Total with Discount:                  $" << setprecision(2) << fixed << sum*discount << endl;
}

//Member discount generation
float savings(){
    string email;
    int confirmation, min=1, max=2;
    char key;
    float percent, discount;
    int num1=1, num2=3;
    
    //Gets user's email
    cout << "\nPlease enter your email: ";
    cin >> email;
    //Validates the user's email by allowing them to change it or proceed
    do{
        cout << "\nEmail: " << email << endl;
        cout << "1. This is the correct email.\n";
        cout << "2. I would like to enter my email again.\n";
        cin >> confirmation;
        
        //Loop that validates user's selection from confirmation menu
        while (!isValid(confirmation, min, max)){
            cout << "1. This is the correct email.\n";
            cout << "2. I would like to enter my email again.\n";
            cin >> confirmation;
        }
        
        //Allows user to change email
        if (confirmation==2){
            cout << "\nPlease enter your email again: ";
            cin >> email;
        }
    } while (confirmation==2);
            
    cout << "\nEnter 'D' to apply a discount: ";
    cin >> key;
        
    while (!isValidDiscount(key)){
        cout << "\n\nInvalid Input. Please enter 'D' to apply a discount: ";
        cin >> key;
    }
    //Generation of random number from 1 to 3
    percent = (rand() % num2-num1+1) + num1;
    //5, 10, 15 percent discounts assigned to integers 1, 2, 3
    if (percent==1){
        discount=0.15;
    } else if (percent==2){
        discount=0.1;
    } else {
        discount=0.05;
    }
    
    //Display's the discount percentage
    cout << "\nYou won a " << 100*discount << "% discount!\n";
    
    return discount;
}

//Validates the user's input of 'd' or 'D' to get the discount
bool isValidDiscount(char x){
    if (x!=68 && x!=100){
        return false;
    } else {
        return true;
    }
}





//Extra functions
int menuDisplay(){
    int menuSelection;
    int min=1, max=4;
    do
    {
        cout << "\n1. Order Item\n";
        cout << "2. Check Out\n";
        cout << "3. Become a Rewards Member\n";
        cout << "4. Quit\n";
        cin >> menuSelection;
    }
    while (!isValid(menuSelection, min, max));
    return menuSelection;
}


char getDecision(){
    char repeat;
    cout << "\nWould you like to order anything else? (y/n)\n";
    cin >> repeat;
    while (!isValid(repeat)){
        cout << "Invalid input. Please enter y or n for yes or no respectively:\n";
        cin >> repeat;
    }
    return repeat;
}


bool isValid(char &a){
    if (a!='y' && a!='n' && a!='Y' && a!='N'){
        return false;
    } else {
        return true;
    }
}
