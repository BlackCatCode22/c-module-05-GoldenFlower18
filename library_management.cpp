#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <sstream>
#include <algorithm>

// Base Book class
class Book {
protected:
    std::string title;
    std::string author;
    std::string genre;
    int publicationYear;
    std::string ISBN;

public:
    // Constructor
    Book(const std::string& title, const std::string& author, const std::string& genre, 
         int publicationYear, const std::string& ISBN)
        : title(title), author(author), genre(genre), 
          publicationYear(publicationYear), ISBN(ISBN) {}
    
    // Virtual destructor for proper inheritance
    virtual ~Book() {}
    
    // Getters
    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    std::string getGenre() const { return genre; }
    int getPublicationYear() const { return publicationYear; }
    std::string getISBN() const { return ISBN; }
    
    // Setters
    void setTitle(const std::string& newTitle) { title = newTitle; }
    void setAuthor(const std::string& newAuthor) { author = newAuthor; }
    void setGenre(const std::string& newGenre) { genre = newGenre; }
    void setPublicationYear(int year) { publicationYear = year; }
    void setISBN(const std::string& newISBN) { ISBN = newISBN; }
    
    // Virtual method for book-specific details
    virtual std::string getBookDetails() const {
        return "Standard book";
    }
    
    // Virtual method for checkout restrictions
    virtual bool hasSpecialCheckoutRules() const {
        return false;
    }
};

// Fiction Book subclass
class FictionBook : public Book {
private:
    bool isSeriesPart;
    std::string seriesName;
    int seriesNumber;

public:
    FictionBook(const std::string& title, const std::string& author, int year, 
                const std::string& ISBN, bool isSeriesPart = false, 
                const std::string& seriesName = "", int seriesNumber = 0)
        : Book(title, author, "Fiction", year, ISBN), 
          isSeriesPart(isSeriesPart), seriesName(seriesName), seriesNumber(seriesNumber) {}
    
    std::string getBookDetails() const override {
        if (isSeriesPart) {
            return "Fiction - Part " + std::to_string(seriesNumber) + 
                   " of " + seriesName + " series";
        } else {
            return "Fiction - Standalone novel";
        }
    }
    
    bool getIsSeriesPart() const { return isSeriesPart; }
    std::string getSeriesName() const { return seriesName; }
    int getSeriesNumber() const { return seriesNumber; }
    
    void setSeriesInfo(bool isSeries, const std::string& name, int number) {
        isSeriesPart = isSeries;
        if (isSeries) {
            seriesName = name;
            seriesNumber = number;
        } else {
            seriesName = "";
            seriesNumber = 0;
        }
    }
};

// Non-fiction Book subclass
class NonFictionBook : public Book {
private:
    std::string subject;
    bool isAcademic;

public:
    NonFictionBook(const std::string& title, const std::string& author, int year, 
                  const std::string& ISBN, const std::string& subject, bool isAcademic = false)
        : Book(title, author, "Non-Fiction", year, ISBN), 
          subject(subject), isAcademic(isAcademic) {}
    
    std::string getBookDetails() const override {
        return "Non-Fiction - " + subject + (isAcademic ? " (Academic)" : "");
    }
    
    bool hasSpecialCheckoutRules() const override {
        return isAcademic; // Academic books may have special checkout rules
    }
    
    std::string getSubject() const { return subject; }
    bool getIsAcademic() const { return isAcademic; }
    
    void setSubject(const std::string& newSubject) { subject = newSubject; }
    void setIsAcademic(bool academic) { isAcademic = academic; }
};

// Reference Book subclass
class ReferenceBook : public Book {
private:
    bool canCheckout;
    std::string edition;

public:
    ReferenceBook(const std::string& title, const std::string& author, int year, 
                 const std::string& ISBN, const std::string& edition, bool canCheckout = false)
        : Book(title, author, "Reference", year, ISBN), 
          canCheckout(canCheckout), edition(edition) {}
    
    std::string getBookDetails() const override {
        return "Reference - " + edition + " Edition" + 
               (canCheckout ? "" : " (In-library use only)");
    }
    
    bool hasSpecialCheckoutRules() const override {
        return !canCheckout; // Reference books often can't be checked out
    }
    
    bool getCanCheckout() const { return canCheckout; }
    std::string getEdition() const { return edition; }
    
    void setCanCheckout(bool checkout) { canCheckout = checkout; }
    void setEdition(const std::string& newEdition) { edition = newEdition; }
};

// Children's Book subclass
class ChildrensBook : public Book {
private:
    std::string ageRange;
    bool hasIllustrations;

public:
    ChildrensBook(const std::string& title, const std::string& author, int year, 
                 const std::string& ISBN, const std::string& ageRange, bool hasIllustrations = true)
        : Book(title, author, "Children's", year, ISBN), 
          ageRange(ageRange), hasIllustrations(hasIllustrations) {}
    
    std::string getBookDetails() const override {
        return "Children's Book - For ages " + ageRange + 
               (hasIllustrations ? " (Illustrated)" : "");
    }
    
    std::string getAgeRange() const { return ageRange; }
    bool getHasIllustrations() const { return hasIllustrations; }
    
    void setAgeRange(const std::string& range) { ageRange = range; }
    void setHasIllustrations(bool illustrated) { hasIllustrations = illustrated; }
};

// Factory function to create the appropriate book type
Book* createBook(const std::string& genre, const std::string& title, const std::string& author, 
                int year, const std::string& ISBN, const std::string& extraInfo) {
    if (genre == "Fiction") {
        return new FictionBook(title, author, year, ISBN);
    } else if (genre == "Non-Fiction") {
        return new NonFictionBook(title, author, year, ISBN, extraInfo);
    } else if (genre == "Reference") {
        return new ReferenceBook(title, author, year, ISBN, extraInfo);
    } else if (genre == "Children's") {
        return new ChildrensBook(title, author, year, ISBN, extraInfo);
    } else {
        // Default case for unknown genre
        return new Book(title, author, genre, year, ISBN);
    }
}

int main() {
    // Using lists to store books by genre
    std::map<std::string, std::list<Book*>> genreLists;
    
    // Read from input file
    std::ifstream inputFile("newBooks.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open input file." << std::endl;
        return 1;
    }
    
    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string title, author, genre, ISBN, extraInfo;
        int year;
        
        // Parse line format: Title,Author,Year,Genre,ISBN,ExtraInfo
        if (std::getline(iss, title, ',') && 
            std::getline(iss, author, ',') && 
            iss >> year && iss.ignore() && 
            std::getline(iss, genre, ',') && 
            std::getline(iss, ISBN, ',') &&
            std::getline(iss, extraInfo)) {
            
            // Create appropriate book type using factory function
            Book* book = createBook(genre, title, author, year, ISBN, extraInfo);
            
            // Add to the appropriate genre list
            genreLists[genre].push_back(book);
        }
    }
    inputFile.close();
    
    // Write to output file
    std::ofstream outputFile("libraryInventory.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open output file." << std::endl;
        
        // Clean up memory before exiting
        for (auto& genrePair : genreLists) {
            for (Book* book : genrePair.second) {
                delete book;
            }
        }
        return 1;
    }
    
    // Write report header
    outputFile << "Library Inventory Report" << std::endl;
    outputFile << "========================" << std::endl << std::endl;
    
    // Process each genre type
    for (auto& genrePair : genreLists) {
        const std::string& genre = genrePair.first;
        std::list<Book*>& bookList = genrePair.second;
        
        outputFile << genre << " Books:" << std::endl;
        outputFile << std::string(genre.length() + 7, '-') << std::endl;
        
        // List all books of this genre
        for (Book* book : bookList) {
            outputFile << "\"" << book->getTitle() << "\" by " 
                     << book->getAuthor() << " (" << book->getPublicationYear() << ")"
                     << " ISBN: " << book->getISBN();
            
            // Add the book details if available
            std::string details = book->getBookDetails();
            if (details != "Standard book") {
                outputFile << " - " << details;
            }
            
            // Add checkout restrictions if any
            if (book->hasSpecialCheckoutRules()) {
                outputFile << " [Special checkout rules apply]";
            }
            
            outputFile << std::endl;
        }
        
        // Add total count for this genre
        outputFile << "Total " << genre << " Books: " << bookList.size() << std::endl << std::endl;
    }
    
    // Calculate total number of books
    size_t totalBooks = 0;
    for (const auto& genrePair : genreLists) {
        totalBooks += genrePair.second.size();
    }
    
    // Add overall total
    outputFile << "Total books in inventory: " << totalBooks << std::endl;
    
    outputFile.close();
    
    std::cout << "Book processing complete. Inventory report generated in libraryInventory.txt" << std::endl;
    
    // Clean up memory
    for (auto& genrePair : genreLists) {
        for (Book* book : genrePair.second) {
            delete book;
        }
    }
    
    return 0;
}