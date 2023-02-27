#include <iostream>
#include <vector>
#include <string>
using namespace std;


class IDGenerator {
private:
    static IDGenerator *instance;
    int nextID;
    IDGenerator() { nextID = 0; }

public:
    static IDGenerator* getInstance() {
        if (!instance)
            instance = new IDGenerator();
        return instance;
    }

    int getNextID() { return nextID++; }
};
IDGenerator* IDGenerator::instance = 0;


class Movie {
private:
    int id;
    string title;
    string director;
    double rating;

public:
    Movie(string title, string director, double rating)
            : title(title), director(director), rating(rating) {
        id = IDGenerator::getInstance()->getNextID();
    }

    int getID() { return id; }
    string getTitle() { return title; }
    string getDirector() { return director; }
    double getRating() { return rating; }

    virtual void output() const{
        cout << "ID: " << id << endl;
        cout << "Title: " << title << endl;
        cout << "Director: " << director << endl;
        cout << "Rating: " << rating << endl;
    }
    friend ostream & operator<<(ostream &output, const Movie &m) {
        m.output();
        return output;
    }
    virtual Movie* clone() {
        return new Movie(*this);
    }
};


class NetflixOriginal : public Movie {
private:
    string producer;

public:
    NetflixOriginal(string title, string director, double rating, string producer)
            : Movie(title, director, rating), producer(producer) {}

    string getProducer() { return producer; }
    void output() const override{
        Movie::output();
        cout << "Producer: " << producer << endl;
    }
    virtual NetflixOriginal* clone() {
        return new NetflixOriginal(*this);
    }

};


template <typename T>
class MovieCollection {
private:
    vector<T*> movies;

public:
    void addMovie(T* movie) { movies.push_back(movie); }
    T* getMovie(int id) {
        for (T* movie : movies) {
            if (movie->getID() == id) return movie;
        }
        return nullptr;
    }

    class Iterator {
    private:
        typename vector<T*>::iterator it;
    public:
        Iterator(typename vector<T*>::iterator it) : it(it) {}
        Iterator operator++() { return Iterator(++it); }
        T* operator*() { return *it; }
        bool operator!=(const Iterator& other) { return it != other.it; }
    };

    Iterator begin() { return Iterator(movies.begin()); }
    Iterator end() { return Iterator(movies.end()); }
    template <typename U>
    T* findByTitle(U title) {
        for (T* movie : movies) {
            if (movie->getTitle() == title) return movie;
        }
        return nullptr;
    }
};

template<>
class MovieCollection<NetflixOriginal> {
private:
    vector<NetflixOriginal*> movies;
public:
    void addMovie(NetflixOriginal* movie) { movies.push_back(movie); }
    NetflixOriginal* getMovie(int id) {
        for (NetflixOriginal* movie : movies) {
            if (movie->getID() == id) return movie;
        }
        return nullptr;
    }
    class Iterator {
    private:
        vector<NetflixOriginal*>::iterator it;
    public:
        Iterator(vector<NetflixOriginal*>::iterator it) : it(it) {}
        Iterator operator++() { return Iterator(++it); }
        NetflixOriginal* operator*() { return *it; }
        bool operator!=(const Iterator& other) { return it != other.it; }
    };

    Iterator begin() { return Iterator(movies.begin()); }
    Iterator end() { return Iterator(movies.end()); }
};

int main() {
    MovieCollection<Movie> movieCollection;
    movieCollection.addMovie(new Movie("The Shawshank Redemption", "Frank Darabont", 9.3));
    movieCollection.addMovie(new Movie("The Godfather", "Francis Ford Coppola", 9.2));
    movieCollection.addMovie(new NetflixOriginal("Stranger Things", "The Duffer Brothers", 8.8, "Netflix"));
    for (Movie *movie: movieCollection) {
        cout << *movie <<endl;
    }
    Movie* movieFind = movieCollection.findByTitle("The Shawshank Redemption");
    cout << movieFind->getTitle();
    return 0;
}
