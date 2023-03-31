#ifndef RECOMMENDER_INCLUDED
#define RECOMMENDER_INCLUDED

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <iostream>

class UserDatabase;
class MovieDatabase;

struct MovieAndRank
{
    MovieAndRank(const std::string& id, int score)
     : movie_id(id), compatibility_score(score)
    {}

    std::string movie_id;
    int compatibility_score;
};

class Recommender
{
  public:
    Recommender(const UserDatabase& user_database,
                const MovieDatabase& movie_database);
    std::vector<MovieAndRank> recommend_movies(const std::string& user_email,
                                               int movie_count) const;
    
  private:
    
    const UserDatabase* userData;
    const MovieDatabase* movieData;
    
    void checkExists(std::unordered_map<std::string,int>& movies, std::string ID, int amt, std::unordered_set<std::string> hist, std::multimap<int,std::string>& rank, int count) const
    {
        if (hist.find(ID) != hist.end())                    //If already watched, don't add and exit function
            return;
        
        movies.insert({ID, 0});

        int old = movies[ID];
        movies[ID] += amt;
        if (rank.size() < count || movies[ID] >= rank.begin()->first)
        {
            rank.insert(std::pair<int, std::string>(movies[ID], ID));
            std::multimap<int,std::string>::iterator x = rank.begin();
            while (x != rank.end())
            {
                if (x->first == old)
                {
                    rank.erase(x);
                    x = rank.end();
                    break;
                }
                x++;
            }
        }
    }
};

#endif // RECOMMENDER_INCLUDED
