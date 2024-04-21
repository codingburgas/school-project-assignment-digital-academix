﻿#include "raylib.h"
#include <string> 
#include <fstream>
#include <iostream>


class CREDENTIAL_BOX
{
public:
    char input[16];
    int charCount;
    Rectangle box;
    int framesCounter;
};

class MAIN_MENU
{
public:
    CREDENTIAL_BOX usernameBox;
    CREDENTIAL_BOX passwordBox;
    Rectangle continueButton;
    Rectangle createAccountButton;
};

class User {
public:
    std::string Username;
};

class REGISTRATION_FORM
{
public:
    CREDENTIAL_BOX firstNameBox{ "", 0, {700, 140, 200, 30}, 0 };
    CREDENTIAL_BOX lastNameBox{ "", 0, {700, 200, 200, 30}, 0 };
    CREDENTIAL_BOX usernameBox{ "", 0, {700, 260, 200, 30}, 0 };
    CREDENTIAL_BOX passwordBox{ "", 0, {700, 320, 200, 30}, 0 };
    CREDENTIAL_BOX confirmPasswordBox{ "", 0, {700, 380, 200, 30}, 0 };
    CREDENTIAL_BOX items{ "", 0, {700, 380, 200, 30}, 0 };
    Rectangle registerButton{ 700, 700, 200, 40 };
};

CREDENTIAL_BOX firstNameBox{ "", 0, {GetScreenWidth() / 2 + 300, 140, 200, 30}, 0 };
CREDENTIAL_BOX lastNameBox{ "", 0, {GetScreenWidth() / 2 + 300, 200, 200, 30}, 0 };
CREDENTIAL_BOX usernameBox{ "", 0, {GetScreenWidth() / 2 + 300, 260, 200, 30}, 0 };
CREDENTIAL_BOX passwordBox{ "", 0, {GetScreenWidth() / 2 + 300, 320, 200, 30}, 0 };
CREDENTIAL_BOX confirmPasswordBox{ "", 0, {GetScreenWidth() / 2 + 300, 380, 200, 30}, 0 };
Rectangle registerButton{ GetScreenWidth() / 2 + 300, 100, 200, 40 };

enum ActiveBox {
    FIRST_NAME,
    LAST_NAME,
    USERNAME,
    PASSWORD,
    CONFIRM_PASSWORD
};

ActiveBox activeBox = USERNAME;
bool registrationFormActive = false;

MAIN_MENU mainMenu = {
    { "", 0, {GetScreenWidth() / 2 + 310, GetScreenHeight() / 2 + 240, 200, 30}, 0 },
    { "", 0, {GetScreenWidth() / 2 + 310, GetScreenHeight() / 2 + 310, 200, 30}, 0 },
    { GetScreenWidth() / 2 + 310, GetScreenHeight() / 2 + 400, 200, 40 },
    { GetScreenWidth() / 2 + 310, GetScreenHeight() / 2 + 450, 200, 40 }
};

REGISTRATION_FORM registrationForm =
{
    { "", 0, {GetScreenWidth() / 2 + 300, 140, 200, 30}, 0 },
    { "", 0, {GetScreenWidth() / 2 + 300, 200, 200, 30}, 0 },
    { "", 0, {GetScreenWidth() / 2 + 300, 260, 200, 30}, 0 },
    { "", 0, {GetScreenWidth() / 2 + 300, 320, 200, 30}, 0 },
    { "", 0, {GetScreenWidth() / 2 + 300, 380, 200, 30}, 0 },
    { GetScreenWidth() / 2 + 500, 700, 200, 40 }
};

Rectangle dropdownRect = { 300, 430, 200, 30 };
int selectedItem = -1;
const char* items[2] = { "Student", "Teacher" };
bool dropdownActive = false;

bool isMouseOverBox(Rectangle box)
{
    return CheckCollisionPointRec(GetMousePosition(), box);
}

void drawTextBox(CREDENTIAL_BOX& textBox, const char* label, bool showCharacters)
{
    DrawText(label, (int)textBox.box.x, (int)textBox.box.y - 19, 20, BLACK);
    DrawRectangleLinesEx(textBox.box, 3, BLACK);

    if (showCharacters)
    {
        DrawText(textBox.input, (int)textBox.box.x + 5, (int)textBox.box.y + 4, 20, BLACK);
    }
    else {
        for (int i = 0; i < textBox.charCount; i++)
        {
            DrawText("*", (int)textBox.box.x + 5 + i * MeasureText("*", 20), (int)textBox.box.y + 4, 20, BLACK);
        }
    }

    if (isMouseOverBox(textBox.box))
    {
        if (textBox.charCount < 13)
        {
            if (((textBox.framesCounter / 20) % 2) == 0)
            {
                DrawText("|", (int)textBox.box.x + 8 + MeasureText(textBox.input, 20), (int)textBox.box.y + 6, 20, BLACK);
            }
        }
    }
}


bool checkRequirements(const char* str) {
    bool hasCapital = false;
    bool hasLower = false;
    bool hasDigit = false;
    bool hasSpecial = false;

    const char* specialCharacters = "!@#$%^&*()-_+=[]{}|;:'\",.<>?";

    for (const char* temp = str; *temp != '\0'; temp++) {
        if (isupper(*temp)) {
            hasCapital = true;
        }
        else if (islower(*temp)) {
            hasLower = true;
        }
        else if (isdigit(*temp)) {
            hasDigit = true;
        }
        else if (strchr(specialCharacters, *temp) != nullptr)
        {
            hasSpecial = true;
        }
    }

    return hasCapital && hasLower && hasDigit && hasSpecial && (strlen(str) >= 6);
}
class userSaver {
public:

    void saveUserData(const char* firstName, const char* lastName, const char* username, const char* password, std::string items)
    {
        std::ofstream file("users.txt", std::ios::app);
        if (file.is_open()) {
            file << "First Name: " << firstName << std::endl;
            file << "Last Name: " << lastName << std::endl;
            file << "Username: " << username << std::endl;
            file << "Password: " << password << std::endl;
            file << "Type: " << selectedItem << std::endl;
            file << "--------------------------" << std::endl;
            file.close();
        }
        else {
            std::cout << "Unable to open file for writing." << std::endl;
        }
    }

    const char* firstName;
    const char* lastName;

private:
    const char* username;
    const char* password;
    std::string items;
};


bool checkAccount(const char* username, const char* password) {
    std::ifstream file("users.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file 'users.txt'" << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.find("Username: " + std::string(username)) != std::string::npos) {
            if (std::getline(file, line)) {
                if (line.find("Password: " + std::string(password)) != std::string::npos) {
                    file.close(); // Close the file before returning
                    return true;
                }
                else {
                    continue;
                }
            }
            else {
                std::cerr << "Error: Unexpected end of file while reading password" << std::endl;
                file.close(); // Close the file before returning
                return false;
            }
        }
    }

    std::cerr << "Error: Username '" << username << "' not found" << std::endl;
    file.close(); // Close the file before returning
    return false;
}

userSaver currentUser;


void mathTest() {
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 600;
    typedef struct {
        char question[256];
        char answers[4][50];
        int correctAnswer;
    } Question;

    // Array of questions
    Question questions[10] = {
        {"What is the solution to the system of equations:\n2x + 3y = 7\n4x - 5y = -2?", {"(1, 2)", "(2, 1)", "(3, 2)", "(2, 3)"}, 0},
        {"What is the sum of the roots of the biquadratic equation:\nx^4 - 5x^2 + 4 = 0?", {"-2", "-1", "1", "2"}, 1},
        {"What is the solution to the system of inequalities:\n2x - y > 3\n3x + y < 6?", {"(1, 4)", "(2, 3)", "(3, 2)", "(4, 1)"}, 0},
        {"What is the value of 'a' in the biquadratic inequality:\n(a - 1)(a + 2)(a - 3)(a + 4) > 0?", {"a < -4 or -2 < a < 1 or a > 3", "a < -4 or -2 < a < 3 or a > 1", "a < -4 or -1 < a < 3 or a > 2", "a < -3 or -1 < a < 2 or a > 4"}, 1},
        {"What is the solution to the system of equations:\n3x - 2y = 1\n4x + 3y = 7?", {"(1, 1)", "(2, 2)", "(3, 3)", "(4, 4)"}, 0},
        {"What are the roots of the biquadratic equation:\n2x^4 - 8x^2 + 6 = 0?", {"{3, -3}", "{2, -2}", "{2, -3}", "{3, -2}"}, 0},
        {"What is the solution to the system of inequalities:\n3x + 2y < 5\n2x - y > 1?", {"(1, 1)", "(2, 2)", "(3, 3)", "(4, 4)"}, 0},
        {"What is the value of 'a' in the biquadratic inequality:\n(a - 2)(a + 1)(a - 4)(a + 3) < 0?", {"-3 < a < -1 or 2 < a < 4", "-4 < a < -2 or 1 < a < 3", "-3 < a < -1 or 1 < a < 3", "-4 < a < -2 or 2 < a < 4"}, 2},
        {"What is the solution to the system of equations:\nx + y = 6\n2x - y = 2?", {"(2, 4)", "(3, 3)", "(4, 2)", "(5, 1)"}, 1},
        {"What is the solution to the biquadratic equation:\nx^4 - 10x^2 + 9 = 0?", {"{3, -3}", "{2, -2}", "{3, -2}", "{2, -3}"}, 2},
    };

    int currentPage = 0;
    int selectedAnswer[10] = { 0 }; // Selected answer for each question

    bool quizFinished = false;

    while (!WindowShouldClose()) {
        // Update
        if (!quizFinished) {
            // TODO: Implement logic for handling input and updating selected answers
        }

        // Draw
        BeginDrawing();

        ClearBackground(GOLD);

        if (!quizFinished) {
            // Draw questions for current page
            int y = 30;
            for (int i = currentPage * 2; i < (currentPage + 1) * 2 && i < 10; i++) {
                DrawText(questions[i].question, 100, y, 20, BLACK);

                // Draw answer options as clickable rectangles
                for (int j = 0; j < 4; j++) {
                    Rectangle answerRect = { 270, y + 60 + 50 * j, 300, 40 };
                    bool answerHovered = CheckCollisionPointRec(GetMousePosition(), answerRect);

                    // Change color if answer is selected or hovered
                    if (selectedAnswer[i] == j || answerHovered) {
                        DrawRectangleRec(answerRect, answerHovered ? DARKBLUE : BLUE);
                    }
                    else {
                        DrawRectangleRec(answerRect, BLUE);
                    }

                    DrawText(questions[i].answers[j], answerRect.x + 5, answerRect.y + 8, 20, WHITE);

                    // Check if an answer option is clicked
                    if (answerHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        selectedAnswer[i] = j;
                    }
                }

                y += 270;
            }

            // Draw "Next Page" button
            if ((currentPage + 1) * 2 < 10) {
                Rectangle nextPageRec = { screenWidth - 150, screenHeight - 50, 120, 40 };
                bool nextPageHovered = CheckCollisionPointRec(GetMousePosition(), nextPageRec);

                DrawRectangleRec(nextPageRec, nextPageHovered ? DARKBLUE : BLUE);
                DrawText("Next Page", nextPageRec.x + 5, nextPageRec.y + 8, 20, WHITE);

                // Check if "Next Page" button is clicked
                if (nextPageHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    currentPage++;
                    // Deselect all answers on page change
                    for (int i = currentPage * 2; i < (currentPage + 1) * 2 && i < 10; i++) {
                        selectedAnswer[i] = 0;
                    }
                }
            }

            // Draw "Finish" button
            else {
                Rectangle finishRec = { screenWidth - 150, screenHeight - 50, 120, 40 };
                bool finishHovered = CheckCollisionPointRec(GetMousePosition(), finishRec);

                DrawRectangleRec(finishRec, finishHovered ? DARKBLUE : DARKBLUE);
                DrawText("Finish", finishRec.x + 25, finishRec.y + 8, 20, WHITE);

                // Check if "Finish" button is clicked
                if (finishHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    quizFinished = true;
                }
            }
        }
        else {
            int score = 0;
            for (int i = 0; i < 10; i++) {
                if (selectedAnswer[i] == questions[i].correctAnswer) {
                    score++;
                }
            }
            char scoreText[30];
            sprintf_s(scoreText, "Your Score: %d/%d", score, 10);
            DrawText(scoreText, 100, 100, 30, BLACK);
        }

        EndDrawing();
    }

    CloseWindow();

}


void biologyTest() {
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 600;
    typedef struct {
        char question[256];
        char answers[4][50];
        int correctAnswer;
    } Question;

    Question questions[10] = {
        {"How many traits are traced in a dihybrid cross?", {"1", "3", "2", "more than three"}, 2},
        {"Orgenesis is the process of forming:", {"sperm cells", "egg cells", "both sperm and egg cells", "none of the above"}, 1},
        {"Which of these are genetic disorders in humans", {"Down syndrome", "Cystic fibrosis", "Influenza", "Hypertension"}, 0},
        {"Microevolution leads to:", {"emergence of new species", "formation of super-species", "continuous change", "extinction"}, 2},
        {"The ecological niche refers to:", {"physical space an organism", "its role in the ecosystem", "its genetic makeup", "its reproductive habits"}, 1},
        {"Cata-morphosis examples include:", {"the evolution of flight", "sight in cave-dwelling", "changes in mating behavior", "development plant structure"}, 1},
        {"Dominant traits are represented by:", {"capital letters", "lowercase letters", "italicized letters", "bold letters)"}, 0},
        {"Probability of a child using the left hand with Dd and dd is:", {"25%", "50%", "75%", "100%"}, 0},
        {"What determines handedness in humans?", {"Dominant genetic factors", "Environmental influences", "Cultural practices", "Random chance"}, 0},
        {"What influences population size", {"Emigration", "Immigration", "Birth rate", "All of the above"}, 3},
    };

    int currentPage = 0;
    int selectedAnswer[10] = { 0 }; // Selected answer for each question

    bool quizFinished = false;

    while (!WindowShouldClose()) {
        // Update
        if (!quizFinished) {
            // TODO: Implement logic for handling input and updating selected answers
        }

        // Draw
        BeginDrawing();

        ClearBackground(GOLD);

        if (!quizFinished) {
            // Draw questions for current page
            int y = 30;
            for (int i = currentPage * 2; i < (currentPage + 1) * 2 && i < 10; i++) {
                DrawText(questions[i].question, 100, y, 20, BLACK);

                // Draw answer options as clickable rectangles
                for (int j = 0; j < 4; j++) {
                    Rectangle answerRect = { 270, y + 60 + 50 * j, 300, 40 };
                    bool answerHovered = CheckCollisionPointRec(GetMousePosition(), answerRect);

                    // Change color if answer is selected or hovered
                    if (selectedAnswer[i] == j || answerHovered) {
                        DrawRectangleRec(answerRect, answerHovered ? DARKBLUE : DARKBLUE);
                    }
                    else {
                        DrawRectangleRec(answerRect, BLUE);
                    }

                    DrawText(questions[i].answers[j], answerRect.x + 5, answerRect.y + 8, 20, WHITE);

                    // Check if an answer option is clicked
                    if (answerHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        selectedAnswer[i] = j;
                    }
                }

                y += 270;
            }

            // Draw "Next Page" button
            if ((currentPage + 1) * 2 < 10) {
                Rectangle nextPageRec = { screenWidth - 150, screenHeight - 50, 120, 40 };
                bool nextPageHovered = CheckCollisionPointRec(GetMousePosition(), nextPageRec);

                DrawRectangleRec(nextPageRec, nextPageHovered ? DARKBLUE : BLUE);
                DrawText("Next Page", nextPageRec.x + 5, nextPageRec.y + 8, 20, WHITE);

                // Check if "Next Page" button is clicked
                if (nextPageHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    currentPage++;
                    // Deselect all answers on page change
                    for (int i = currentPage * 2; i < (currentPage + 1) * 2 && i < 10; i++) {
                        selectedAnswer[i] = 0;
                    }
                }
            }

            // Draw "Finish" button
            else {
                Rectangle finishRec = { screenWidth - 150, screenHeight - 50, 120, 40 };
                bool finishHovered = CheckCollisionPointRec(GetMousePosition(), finishRec);

                DrawRectangleRec(finishRec, finishHovered ? DARKBLUE : BLUE);
                DrawText("Finish", finishRec.x + 25, finishRec.y + 8, 20, WHITE);

                // Check if "Finish" button is clicked
                if (finishHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    quizFinished = true;
                }
            }
        }
        else {
            int score = 0;
            for (int i = 0; i < 10; i++) {
                if (selectedAnswer[i] == questions[i].correctAnswer) {
                    score++;
                }
            }
            char scoreText[30];
            sprintf_s(scoreText, "Your Score: %d/%d", score, 10);
            DrawText(scoreText, 100, 100, 30, BLACK);
        }

        EndDrawing();
    }

    CloseWindow();

}

void studentMenu()
{
    std::cout << "mazna";
    std::string fName = currentUser.firstName;
    std::string lName = currentUser.lastName;
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        DrawText("Hello", 20, 20, 50, BLACK);
        EndDrawing();
        std::cout << "mazn";
    }
    
}

void RegisterForm()
{
    int key = 0;
    activeBox = USERNAME;

    while (!WindowShouldClose())
    {
        key = GetCharPressed();

        if (isMouseOverBox(registrationForm.firstNameBox.box))
        {
            activeBox = FIRST_NAME;
            while (key > 0)
            {

                if ((key >= 32) && (key <= 125) && (registrationForm.firstNameBox.charCount < 15))
                {

                    registrationForm.firstNameBox.input[registrationForm.firstNameBox.charCount] = (char)key;
                    registrationForm.firstNameBox.input[registrationForm.firstNameBox.charCount + 1] = '\0';
                    registrationForm.firstNameBox.charCount++;

                }
                key = GetCharPressed();
            }
        }

        if (isMouseOverBox(registrationForm.lastNameBox.box))
        {
            activeBox = LAST_NAME;
            while (key > 0)
            {
                if ((key >= 32) && (key <= 125) && (registrationForm.lastNameBox.charCount < 15))
                {
                    registrationForm.lastNameBox.input[registrationForm.lastNameBox.charCount] = (char)key;
                    registrationForm.lastNameBox.input[registrationForm.lastNameBox.charCount + 1] = '\0';
                    registrationForm.lastNameBox.charCount++;
                }
                key = GetCharPressed();
            }
        }

        if (isMouseOverBox(registrationForm.usernameBox.box))
        {
            activeBox = USERNAME;
            while (key > 0)
            {
                if ((key >= 32) && (key <= 125) && (registrationForm.usernameBox.charCount < 15))
                {
                    registrationForm.usernameBox.input[registrationForm.usernameBox.charCount] = (char)key;
                    registrationForm.usernameBox.input[registrationForm.usernameBox.charCount + 1] = '\0';
                    registrationForm.usernameBox.charCount++;
                }
                key = GetCharPressed();
            }
        }

        if (isMouseOverBox(registrationForm.passwordBox.box))
        {
            activeBox = PASSWORD;
            while (key > 0)
            {
                if ((key >= 32) && (key <= 125) && (registrationForm.passwordBox.charCount < 15))
                {
                    registrationForm.passwordBox.input[registrationForm.passwordBox.charCount] = (char)key;
                    registrationForm.passwordBox.input[registrationForm.passwordBox.charCount + 1] = '\0';
                    registrationForm.passwordBox.charCount++;
                }
                key = GetCharPressed();
            }
        }

        if (isMouseOverBox(registrationForm.confirmPasswordBox.box))
        {
            activeBox = CONFIRM_PASSWORD;
            while (key > 0)
            {
                if ((key >= 32) && (key <= 125) && (registrationForm.confirmPasswordBox.charCount < 15))
                {
                    registrationForm.confirmPasswordBox.input[registrationForm.confirmPasswordBox.charCount] = (char)key;
                    registrationForm.confirmPasswordBox.input[registrationForm.confirmPasswordBox.charCount + 1] = '\0';
                    registrationForm.confirmPasswordBox.charCount++;
                }
                key = GetCharPressed();
            }
        }

        BeginDrawing();
        ClearBackground(GOLD);
        DrawRectangleRec(dropdownRect, GOLD);
        DrawRectangleLines(dropdownRect.x, dropdownRect.y, dropdownRect.width, dropdownRect.height, BLACK);

        if (selectedItem != -1) {
            DrawText(items[selectedItem], dropdownRect.x + 10, dropdownRect.y + 8, 20, BLACK);
        }

        if (dropdownActive) {
            for (int i = 0; i < 2; i++) {
                Rectangle itemRect = { dropdownRect.x, dropdownRect.y + dropdownRect.height + 2 + 30 * i, dropdownRect.width, 30 };
                DrawRectangleRec(itemRect, GOLD);
                DrawRectangleLines(itemRect.x, itemRect.y, itemRect.width, itemRect.height, BLACK);
                DrawText(items[i], itemRect.x + 10, itemRect.y + 8, 20, BLACK);

                if (CheckCollisionPointRec(GetMousePosition(), itemRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    selectedItem = i;
                    dropdownActive = false;
                }
            }
        }

        if (CheckCollisionPointRec(GetMousePosition(), dropdownRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            dropdownActive = !dropdownActive;
        }
        DrawText("Register", GetScreenWidth() / 2 - MeasureText("Register", 30) / 2, 50, 30, BLACK);
        drawTextBox(registrationForm.firstNameBox, "First Name:", true);
        drawTextBox(registrationForm.lastNameBox, "Last Name:", true);
        drawTextBox(registrationForm.usernameBox, "Username:", true);
        drawTextBox(registrationForm.passwordBox, "Password:", false);
        drawTextBox(registrationForm.confirmPasswordBox, "Confirm Password:", false);

        registrationForm.registerButton.x = GetScreenWidth() / 2 - registrationForm.registerButton.width / 2;
        registrationForm.registerButton.y = GetScreenHeight() / 2 + 220;


        if (IsKeyPressed(KEY_BACKSPACE)) {
            switch (activeBox) {
            case FIRST_NAME:
                if (registrationForm.firstNameBox.charCount > 0) {
                    registrationForm.firstNameBox.input[registrationForm.firstNameBox.charCount - 1] = '\0';
                    registrationForm.firstNameBox.charCount--;
                }
                break;
            case LAST_NAME:
                if (registrationForm.lastNameBox.charCount > 0) {
                    registrationForm.lastNameBox.input[registrationForm.lastNameBox.charCount - 1] = '\0';
                    registrationForm.lastNameBox.charCount--;
                }
                break;
            case USERNAME:
                if (registrationForm.usernameBox.charCount > 0) {
                    registrationForm.usernameBox.input[registrationForm.usernameBox.charCount - 1] = '\0';
                    registrationForm.usernameBox.charCount--;
                }
                break;
            case PASSWORD:
                if (registrationForm.passwordBox.charCount > 0) {
                    registrationForm.passwordBox.input[registrationForm.passwordBox.charCount - 1] = '\0';
                    registrationForm.passwordBox.charCount--;
                }
                break;
            case CONFIRM_PASSWORD:
                if (registrationForm.confirmPasswordBox.charCount > 0) {
                    registrationForm.confirmPasswordBox.input[registrationForm.confirmPasswordBox.charCount - 1] = '\0';
                    registrationForm.confirmPasswordBox.charCount--;
                }
                break;
            }
        }

        DrawText(checkRequirements(registrationForm.passwordBox.input) ? "The password must contain: a special character, a capital letter, and be at least 6 characters long." : "The password must contain: a special character, a capital letter, and be at least 6 characters long.", GetScreenWidth() / 2 - MeasureText("Register", 20) / 2 - 260, 570, 13, checkRequirements(registrationForm.passwordBox.input) ? GREEN : RED);

        if (isMouseOverBox(registrationForm.registerButton))
        {
            DrawRectangleRec(registrationForm.registerButton, BLUE);
        }
        else
        {
            DrawRectangleRec(registrationForm.registerButton, DARKBLUE);
        }
        DrawText("Register", registrationForm.registerButton.x + 60, registrationForm.registerButton.y + 10, 20, WHITE);

        if (isMouseOverBox(registrationForm.registerButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (strcmp(registrationForm.passwordBox.input, registrationForm.confirmPasswordBox.input) == 0 && checkRequirements(registrationForm.passwordBox.input)) {

                if (strlen(registrationForm.firstNameBox.input) > 0 && strlen(registrationForm.lastNameBox.input) > 0) {

                    currentUser.saveUserData(registrationForm.firstNameBox.input, registrationForm.lastNameBox.input, registrationForm.usernameBox.input, registrationForm.passwordBox.input, registrationForm.items.input);

                    registrationForm.firstNameBox.charCount = 0;
                    registrationForm.lastNameBox.charCount = 0;
                    registrationForm.usernameBox.charCount = 0;
                    registrationForm.passwordBox.charCount = 0;
                    registrationForm.confirmPasswordBox.charCount = 0;

                    memset(registrationForm.firstNameBox.input, 0, sizeof(registrationForm.firstNameBox.input));
                    memset(registrationForm.lastNameBox.input, 0, sizeof(registrationForm.lastNameBox.input));
                    memset(registrationForm.usernameBox.input, 0, sizeof(registrationForm.usernameBox.input));
                    memset(registrationForm.passwordBox.input, 0, sizeof(registrationForm.passwordBox.input));
                    memset(registrationForm.confirmPasswordBox.input, 0, sizeof(registrationForm.confirmPasswordBox.input));

                    registrationFormActive = false;
                    break;
                }
            }
        }

        if (CheckCollisionPointRec(GetMousePosition(), registrationForm.registerButton))
        {
            DrawRectangleRec(registrationForm.registerButton, BLUE);
            DrawText("Register", registrationForm.registerButton.x + 60, registrationForm.registerButton.y + 10, 20, WHITE);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                const char* firstName = "";
                const char* lastName = "";
                const char* USERNAME = "";
                const char* password = "";
                std::string items = "";

                currentUser.saveUserData(firstName, lastName, USERNAME, password, items);
                studentMenu();
                std::cout << "mazna69";
            }
        }

        EndDrawing();
    }
}

void DrawMainMenu()
{
    int key = 0;
    activeBox = USERNAME;

    while (!WindowShouldClose())
    {
        key = GetCharPressed();

        if (isMouseOverBox(mainMenu.usernameBox.box))
        {
            activeBox = FIRST_NAME;
            while (key > 0)
            {

                if ((key >= 32) && (key <= 125) && (mainMenu.usernameBox.charCount < 15))
                {

                    mainMenu.usernameBox.input[mainMenu.usernameBox.charCount] = (char)key;
                    mainMenu.usernameBox.input[mainMenu.usernameBox.charCount + 1] = '\0';
                    mainMenu.usernameBox.charCount++;

                }
                key = GetCharPressed();
            }
        }

        if (isMouseOverBox(mainMenu.passwordBox.box))
        {
            activeBox = FIRST_NAME;
            while (key > 0)
            {

                if ((key >= 32) && (key <= 125) && (mainMenu.passwordBox.charCount < 15))
                {

                    mainMenu.passwordBox.input[mainMenu.passwordBox.charCount] = (char)key;
                    mainMenu.passwordBox.input[mainMenu.passwordBox.charCount + 1] = '\0';
                    mainMenu.passwordBox.charCount++;

                }
                key = GetCharPressed();
            }
        }
        BeginDrawing();
        ClearBackground(GOLD);

        DrawText("Digital Academix", GetScreenWidth() / 2 - MeasureText("Digital Academix", 60) / 2, 50, 60, BLUE);

        if (isMouseOverBox(mainMenu.usernameBox.box))
        {
            activeBox = USERNAME;
        }
        else if (isMouseOverBox(mainMenu.passwordBox.box))
        {
            activeBox = PASSWORD;
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            switch (activeBox) {
            case USERNAME:
                if (registrationForm.usernameBox.charCount > 0) {
                    registrationForm.usernameBox.input[registrationForm.usernameBox.charCount - 1] = '\0';
                    registrationForm.usernameBox.charCount--;
                }
                break;
            case PASSWORD:
                if (registrationForm.passwordBox.charCount > 0) {
                    registrationForm.passwordBox.input[registrationForm.passwordBox.charCount - 1] = '\0';
                    registrationForm.passwordBox.charCount--;
                }
                break;
            }
        }

        mainMenu.usernameBox.box.x = GetScreenWidth() / 2 - mainMenu.usernameBox.box.width / 2;
        mainMenu.usernameBox.box.y = GetScreenHeight() / 2 - 60;
        mainMenu.passwordBox.box.x = GetScreenWidth() / 2 - mainMenu.passwordBox.box.width / 2;
        mainMenu.passwordBox.box.y = GetScreenHeight() / 2;

        drawTextBox(mainMenu.usernameBox, "Username:", true);
        drawTextBox(mainMenu.passwordBox, "Password:", false);

        mainMenu.continueButton.x = GetScreenWidth() / 2 - mainMenu.continueButton.width / 2;
        mainMenu.continueButton.y = GetScreenHeight() / 2 + 60;
        mainMenu.createAccountButton.x = GetScreenWidth() / 2 - mainMenu.createAccountButton.width / 2;
        mainMenu.createAccountButton.y = GetScreenHeight() / 2 + 135;

        if (isMouseOverBox(mainMenu.continueButton))
        {
            DrawRectangleRec(mainMenu.continueButton, BLUE);
        }
        else
        {
            DrawRectangleRec(mainMenu.continueButton, DARKBLUE);
        }

       
        DrawText("Log in", mainMenu.continueButton.x + 60, mainMenu.continueButton.y + 10, 20, WHITE);

        if (CheckCollisionPointRec(GetMousePosition(), mainMenu.continueButton))
        {

            DrawRectangleRec(mainMenu.continueButton, BLUE);
            DrawText("Log in", mainMenu.continueButton.x + 60, mainMenu.continueButton.y + 10, 20, WHITE);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                const char* username = "";
                const char* password = "";
                if (checkAccount(username, password))
                {
                    studentMenu();
                }
                else
                {
                    std::cout << "Invalid Username or password. Please try again." << std::endl;
                }
            }
        }


        if (CheckCollisionPointRec(GetMousePosition(), mainMenu.createAccountButton))
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                RegisterForm();
            }
        }
        DrawLine(280, mainMenu.createAccountButton.y - 10, 520, mainMenu.createAccountButton.y - 10, BLUE);
        DrawText("or", mainMenu.createAccountButton.x + 85, mainMenu.createAccountButton.y - 5, 20, BLACK);
        DrawText("Create Account", mainMenu.createAccountButton.x + 20, mainMenu.createAccountButton.y + 20, 20, BLACK);
        EndDrawing();
    }
}



int main()
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Digital Academix");

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(LIGHTGRAY);
        //DrawMainMenu();
        biologyTest();
    }
}




