#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "User.h"
#include "Movie.h"

#include <algorithm>
using namespace std;

Recommender::Recommender(const UserDatabase& user_database, const MovieDatabase& movie_database)
:userData(&user_database), movieData(&movie_database)
{}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    
    if (movie_count <= 0)
        return vector<MovieAndRank>();
    
    unordered_map<string, int> prototype;
    vector<MovieAndRank> ans;
    multimap<int, string> rank;
    
    User* u = userData->get_user_from_email(user_email);
    if (u != nullptr)
    {
        vector<string> history = u->get_watch_history();
        unordered_set<string> historySearch;
        for (int i = 0; i < history.size(); i++)                            //Fill an unordered set for faster searches
            historySearch.insert(history[i]);
        
        //for each movie
        for (int i = 0; i < history.size(); i++)
        {
            string ID = history[i];
            Movie* m = movieData->get_movie_from_id(ID);
            
            //For each director in the movie, each movie in movie database that has the director gets +20 points
            vector<string> directors = m->get_directors();
            for (int i = 0; i < directors.size(); i++)
            {
                vector<Movie*> mD = movieData->get_movies_with_director(directors[i]);
                for (int i = 0; i < mD.size(); i++)
                {
                    checkExists(prototype, mD[i]->get_id(), 20, historySearch, rank, movie_count);
                    
                }
            }
            
            //For each actor in the movie, each movie in the movie database that has the actor gets +30 points
            vector<string> actors = m->get_actors();
            for (int i = 0; i < actors.size(); i++)
            {
                vector<Movie*> mA = movieData->get_movies_with_actor(actors[i]);
                for (int i = 0; i < mA.size(); i++)
                {
                    checkExists(prototype, mA[i]->get_id(), 30, historySearch, rank, movie_count);
                }
                
            }
            
            //For each genre in the movie, each movie in the movie ddatabase with that genre gets +1 points
            vector<string> genres = m->get_genres();
            for (int i = 0; i < genres.size(); i++)
            {
                vector<Movie*> mG = movieData->get_movies_with_genre(genres[i]);
                for (int i = 0; i < mG.size(); i++)
                {
                    checkExists(prototype, mG[i]->get_id(), 1, historySearch, rank, movie_count);
                }
            }
        }
        
        //Rank order movies from greatest to least points
        //If same score, rank from higher to lower rating
        //If same rating, rank alphabetically from A(1) -> Z
        
        multimap<int, string>::iterator p = rank.end();
        int index = 0;
        while (p != rank.begin() && index < movie_count)
        {
            p--;
            MovieAndRank newM = MovieAndRank(p->second, p->first);
            ans.push_back(newM);
            index+=1;
        }
    }
    
    //Return vector with specified number of recommendations
    return ans;
}
