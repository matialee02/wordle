/**
 * Matia Lee
 * October 3, 2023 
 * CS3307 Individual Assignment
 */

/**
 * Vīta brevis, 
 * ars longa,
 * occāsiō praeceps,
 * experīmentum perīculōsum,
 * iūdicium difficile.
 */

#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <stdlib.h>
#include <time.h>

#include "wordlist.hpp"

class wordleApplication : public Wt::WApplication
{
public:
    wordleApplication(const Wt::WEnvironment& env);

private:
    Wt::WContainerWidget* mainContainer_;
    Wt::WLineEdit* wordInput_;
    Wt::WPushButton* submitButton_;
    Wt::WText* resultText_;
    Wt::WText* remainingAttemptsText_; 

    std::string targetWord_; // Word to guess
    int Attempts = 6;
    int INDEX; 

    void initializeGame();
    void checkGuess();
    void endGame();
    bool is_a_word(std::string word);
};

wordleApplication::wordleApplication(const Wt::WEnvironment& env) : Wt::WApplication(env) {
    setTitle("Wordle Game");
    useStyleSheet("styles.css");
    mainContainer_ = root();
    initializeGame();
}

void wordleApplication::initializeGame() {
    // Select a random word from the list
    srand(time(NULL)); // NOTE: rand() keeps producing same numbers in the same order without this line
    INDEX = rand() % wordList.size(); 
    targetWord_ = wordList[INDEX];
    // Capitalizing target word
    transform(targetWord_.begin(), targetWord_.end(), targetWord_.begin(), ::toupper);

    root()->addNew<Wt::WText>("Wordle"); 
    root()->addNew<Wt::WBreak>();
    root()->addNew<Wt::WBreak>();
    // Short message about the wordle game and explaining the rules
    const char* message = "Rules: The object of this game is to correctly"
        " guess a five-letter word. You get 6 attempts to enter a five-letter word of"
        " your guess. For each input, if a letter matches the correct word in the correct"
        " position, it will be colored GREEN, and if there is a letter in your guess that is"
        " also in the correct word but not in the same position, it will be colored RED."
        " Letters in your guess that do not appear in the correct word will not be"
        " colored. Have fun!";
    root()->addNew<Wt::WText>(message);
    root()->addNew<Wt::WBreak>();
    root()->addNew<Wt::WBreak>();
    
    // Remaining attempts box text widget
    remainingAttemptsText_ = root()->addNew<Wt::WText>("Remaining attempts: " + std::to_string(Attempts));
    root()->addNew<Wt::WBreak>();
    root()->addNew<Wt::WBreak>();

    // Take user input 
    wordInput_ = mainContainer_->addWidget(std::make_unique<Wt::WLineEdit>());
    wordInput_->setWidth(130);
    wordInput_->setPlaceholderText("Enter a 5-letter word");

    // Go to checkGuess if Enter is pressed
    wordInput_->enterPressed().connect(this, &wordleApplication::checkGuess);
    // Or if Submit is clicked
    submitButton_ = mainContainer_->addWidget(std::make_unique<Wt::WPushButton>("Submit"));
    submitButton_->clicked().connect(this, &wordleApplication::checkGuess);

    root()->addNew<Wt::WBreak>();
}

void wordleApplication::checkGuess() {
    // Putting guess word in a string variable
    std::string guess = wordInput_->text().toUTF8();
    // Capitalizing user input
    transform(guess.begin(), guess.end(), guess.begin(), ::toupper);
    // Clear input box
    wordInput_->setText("");

    // Discard the input and don't count it as an attempt if it's < 5 chars
    if (guess.length() != 5) { // I wanted to add a !is_a_word(guess) condition here as well but that takes up too much resources (time & space)
        root()->addNew<Wt::WBreak>();
        root()->addNew<Wt::WText>("Invalid input. Enter a 5-letter word. ");
        return; 
    }

    // If user input counts as an attempt then update remaining attempts text
    Attempts--;
    remainingAttemptsText_->setText("Remaining attempts: " + std::to_string(Attempts)); 

    // Game over! User loses
    if (Attempts == 0) {
        root()->addNew<Wt::WBreak>();
        root()->addNew<Wt::WText>("You Lose :( The word is " + targetWord_);
        endGame();
        return;
    }

    // For colouring characters in the user input according to the rules, using CSS
    Wt::WContainerWidget* coloredWordContainer = root()->addNew<Wt::WContainerWidget>();
    coloredWordContainer->setStyleClass("input-container"); 
    for (int i = 0; i < 5; i++) {
        char guessChar = guess[i];
        char targetChar = targetWord_[i];

        Wt::WText* charText = coloredWordContainer->addWidget(std::make_unique<Wt::WText>(std::string(1, guessChar)));
        charText->setInline(true);

        if (guessChar == targetChar) {
            charText->setStyleClass("correct-position"); 
        } else if (targetWord_.find(guessChar) != std::string::npos) {
            charText->setStyleClass("wrong-position");
        }
    }

    // User wins
    if (guess == targetWord_) {
        root()->addNew<Wt::WBreak>();
        root()->addNew<Wt::WText>("You win! :) The word is: " + targetWord_);
        endGame();
    }
}

// Displays game over message
void wordleApplication::endGame() {
    root()->addNew<Wt::WBreak>();
    root()->addNew<Wt::WText>("Game over. Refresh to start over");
}

// Driver function
int main(int argc, char** argv) {
    return Wt::WRun(argc, argv, [](const Wt::WEnvironment& env) {
        return std::make_unique<wordleApplication>(env);
    });
}

// is_a_word IS NOT USED ANYWHERE, KEPT HERE ONLY FOR FUTURE IMPROVEMENTS
bool wordleApplication::is_a_word(std::string word) {
    const char *temp; 
    const char *temp2;
    for (int i = 0; i < wordList.size(); i++) {
        temp = wordList[i].c_str();
        temp2 = word.c_str();
        if (strcmp(temp, temp2) == 0) {
            return true; 
        }
    }
    return false; 
}