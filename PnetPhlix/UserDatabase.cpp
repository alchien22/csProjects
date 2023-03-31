#include "UserDatabase.h"
#include "User.h"
#include <fstream>

#include <vector>

using namespace std;

UserDatabase::UserDatabase()
{}

bool UserDatabase::load(const string& filename)
{
    ifstream inputFile(filename);
    if (!inputFile)
    {
        cerr << "cannot open file" << endl;
        return false;
    }
        string name;
        while(getline(inputFile, name))             //While not at the end of the file
        {
            while (name.length() == 0)
                getline(inputFile, name);           //Keep skipping lines if empty
            
            string email;                           //Get user email
            getline(inputFile, email);
            
            string count;                           //Get user movies watched
            getline(inputFile, count);
            
            vector<string> userHist;
            string ID;                              //Get user movie IDs
            getline(inputFile, ID);
            while (ID.length() != 0)                //While no empty line is detected, read in IDs
            {
                userHist.push_back(ID);             //Add movie ID to the list
                getline(inputFile, ID);
            }                                       //End of a single user block; move on to the next user if not at end
            
            User* create = new User(name, email, userHist);
            pointers.push_back(create);
            actors.insert(email, create);         //name, count, IDs
        }
        return true;
}

User* UserDatabase::get_user_from_email(const string& email) const
{
    TreeMultimap<string, User*>::Iterator it = actors.find(email);
    if (it.is_valid())
    {
        User* find = it.get_value();
        return find;
    }
    return nullptr;
}

UserDatabase::~UserDatabase()
{
    for (int i = 0; i < pointers.size(); i++)
    {
        delete pointers[i];
    }
}
