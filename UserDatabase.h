#ifndef USERDATABASE_INCLUDED
#define USERDATABASE_INCLUDED

#include <string>
#include "treemm.h"
#include <vector>

class User;

class UserDatabase
{
  public:
    UserDatabase();
    bool load(const std::string& filename);
    User* get_user_from_email(const std::string& email) const;
    ~UserDatabase();
  private:
    TreeMultimap<std::string, User*> actors;
    std::vector<User*> pointers;
};

#endif // USERDATABASE_INCLUDED
