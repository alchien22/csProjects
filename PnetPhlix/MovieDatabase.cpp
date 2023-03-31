
#include "MovieDatabase.h"
#include "Movie.h"
#include <fstream>
#include <cctype>
using namespace std;

MovieDatabase::MovieDatabase()
{}

bool MovieDatabase::load(const string& filename)
{
    ifstream inputFile(filename);
    
    if (inputFile)
    {
        string ID;
        while(getline(inputFile, ID))                       //While not at the end of the file
        {
            vector<string> info;
            info.push_back(ID);
            
            string name;                                    //Get movie name
            getline(inputFile, name);
            info.push_back(name);
            
            string year;                                    //Get year
            getline(inputFile, year);
            info.push_back(year);
            
            vector<string> directors;
            string allDirectorNames;
            getline(inputFile, allDirectorNames);
            string build;
            
            for (int i = 0; i < allDirectorNames.length(); i++)
            {
                if (allDirectorNames[i] == ',')
                {
                    directors.push_back(build);
                    build = "";
                }
                else if (allDirectorNames[i] != ',')
                {
                    build += allDirectorNames[i];
                    if (i == allDirectorNames.length()-1)
                    {
                        directors.push_back(build);
                    }
                    
                }
            }
            
            vector<string> actors;
            string allActorNames;
            getline(inputFile, allActorNames);
            build = "";
            for (int i = 0; i < allActorNames.length(); i++)
            {
                if (allActorNames[i] == ',')
                {
                    actors.push_back(build);
                    build = "";
                }
                else if (allActorNames[i] != ',')
                {
                    build += allActorNames[i];
                    if (i == allActorNames.length()-1)
                    {
                        actors.push_back(build);
                    }
                }
            }
            
            vector<string> genres;
            string allGenreNames;
            getline(inputFile, allGenreNames);
            build = "";
            for (int i = 0; i < allGenreNames.length(); i++)
            {
                if (allGenreNames[i] == ',')
                {
                    genres.push_back(build);
                    build = "";
                }
                else if (allGenreNames[i] != ',')
                {
                    build += allGenreNames[i];
                    if (i == allGenreNames.length()-1)
                    {
                        genres.push_back(build);
                    }
                }
            }
            
            string rating;
            getline(inputFile, rating);
            info.push_back(rating);
            
            getline(inputFile, allGenreNames);              //Skip the blank line
            
            float ratingFl = stof(rating);
            
            Movie* create1 = new Movie(ID, name, year, directors, actors, genres, ratingFl);
            pointers.push_back(create1);
            movies.insert(ID, create1);
            
            for (int i = 0; i < actors.size(); i++)
            {
                string convert = actors[i];
                for (int i = 0; i < convert.length(); i++)
                {
                    convert[i] = toupper(convert[i]);
                }
                Movie* create2 = new Movie(ID, name, year, directors, actors, genres, ratingFl);
                actorT.insert(convert, create2);
            }
            
            for (int i = 0; i < directors.size(); i++)
            {
                string convert = directors[i];
                for (int i = 0; i < convert.length(); i++)
                {
                    convert[i] = toupper(convert[i]);
                }
                Movie* create3 = new Movie(ID, name, year, directors, actors, genres, ratingFl);
                directorT.insert(convert, create3);
            }
            
            for (int i = 0; i < genres.size(); i++)
            {
                string convert = genres[i];
                for (int i = 0; i < convert.length(); i++)
                {
                    convert[i] = toupper(convert[i]);
                }
                Movie* create4 = new Movie(ID, name, year, directors, actors, genres, ratingFl);
                genreT.insert(convert, create4);
            }
            
        }
        return true;
    }
    else
        return false;
}
                                      
Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    TreeMultimap<string, Movie*>::Iterator it = movies.find(id);      //BST lookup: O(logN)
    if (it.is_valid())
    {
        Movie* find = it.get_value();
        return find;
    }
    return nullptr;
}

vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    string convert = director;
    for (int i = 0; i < convert.length(); i++)
    {
        convert[i] = toupper(convert[i]);
    }
    TreeMultimap<string, Movie*>::Iterator itD = directorT.find(convert);
    vector<Movie*> ans;
    while (itD.is_valid())
    {
        Movie* find = itD.get_value();
        ans.push_back(find);
        itD.advance();
    }
    return ans;
}

vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    string convert = actor;
    for (int i = 0; i < convert.length(); i++)
    {
        convert[i] = toupper(convert[i]);
    }
    TreeMultimap<string, Movie*>::Iterator itA = actorT.find(convert);
    vector<Movie*> ans;
    while (itA.is_valid())
    {
        Movie* find = itA.get_value();
        ans.push_back(find);
        itA.advance();
    }
    return ans;
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    string convert = genre;
    for (int i = 0; i < convert.length(); i++)
    {
        convert[i] = toupper(convert[i]);
    }
    TreeMultimap<string, Movie*>::Iterator itG = genreT.find(convert);
    vector<Movie*> ans;
    while (itG.is_valid())
    {
        Movie* find = itG.get_value();
        ans.push_back(find);
        itG.advance();
    }
    return ans;
}

MovieDatabase::~MovieDatabase()
{
    for (int i = 0; i < pointers.size(); i++)
    {
        delete pointers[i];
    }
}
 

