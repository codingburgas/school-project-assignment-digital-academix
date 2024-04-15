#include "raylib.h"
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
    CREDENTIAL_BOX idBox;
    CREDENTIAL_BOX passwordBox;
    Rectangle continueButton;
    Rectangle createAccountButton;
};

class User {
public:
    std::string studentID;
};

class REGISTRATION_FORM
{
public:
    CREDENTIAL_BOX firstNameBox{ "", 0, {700, 140, 200, 30}, 0 };
    CREDENTIAL_BOX lastNameBox{ "", 0, {700, 200, 200, 30}, 0 };
    CREDENTIAL_BOX idBox{ "", 0, {700, 260, 200, 30}, 0 };
    CREDENTIAL_BOX passwordBox{ "", 0, {700, 320, 200, 30}, 0 };
    CREDENTIAL_BOX confirmPasswordBox{ "", 0, {700, 380, 200, 30}, 0 };
    CREDENTIAL_BOX items{ "", 0, {700, 380, 200, 30}, 0 };
    Rectangle registerButton{ 700, 700, 200, 40 };
};

CREDENTIAL_BOX firstNameBox{ "", 0, {GetScreenWidth() / 2 + 300, 140, 200, 30}, 0 };
CREDENTIAL_BOX lastNameBox{ "", 0, {GetScreenWidth() / 2 + 300, 200, 200, 30}, 0 };
CREDENTIAL_BOX idBox{ "", 0, {GetScreenWidth() / 2 + 300, 260, 200, 30}, 0 };
CREDENTIAL_BOX passwordBox{ "", 0, {GetScreenWidth() / 2 + 300, 320, 200, 30}, 0 };
CREDENTIAL_BOX confirmPasswordBox{ "", 0, {GetScreenWidth() / 2 + 300, 380, 200, 30}, 0 };
Rectangle registerButton{ GetScreenWidth() / 2 + 300, 100, 200, 40 };

enum ActiveBox {
    FIRST_NAME,
    LAST_NAME,
    SCHOOL_ID,
    PASSWORD,
    CONFIRM_PASSWORD
};

ActiveBox activeBox = SCHOOL_ID;
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

bool isValidNumber(const std::string& input) {
    try {
        std::stof(input);
        return true;
    }
    catch (...) {
        return false;
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

void saveUserData(const char* firstName, const char* lastName, const char* schoolID, const char* password, std::string items)
{
    std::ofstream file("users.txt", std::ios::app);
    if (file.is_open()) {
        file << "First Name: " << firstName << std::endl;
        file << "Last Name: " << lastName << std::endl;
        file << "Student ID: " << schoolID << std::endl;
        file << "Password: " << password << std::endl;
        file << "Type: " << selectedItem << std::endl;
        file << "--------------------------" << std::endl;  
        file.close();
    }
    else {
        std::cout << "Unable to open file for writing." << std::endl;
    }
}

bool checkExistingAccount(const char* schoolID, const char* password)
{
    std::ifstream file("users.txt");
    std::string line;

    while (std::getline(file, line)) {
        if (line.find("Student ID: " + std::string(schoolID)) != std::string::npos) {
            std::getline(file, line);
            return line.find("Password: " + std::string(password)) != std::string::npos;
        }
    }

    return false;
}



struct TextBox {
    std::string text;
    Rectangle rect;
};

struct Button {
    Rectangle rect;
    std::string label;
};

User currentUser;

void RegisterForm()
{
    int key = 0;
    activeBox = SCHOOL_ID;

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

        if (isMouseOverBox(registrationForm.idBox.box))
        {
            activeBox = SCHOOL_ID;
            while (key > 0)
            {
                if ((key >= 32) && (key <= 125) && (registrationForm.idBox.charCount < 15))
                {
                    registrationForm.idBox.input[registrationForm.idBox.charCount] = (char)key;
                    registrationForm.idBox.input[registrationForm.idBox.charCount + 1] = '\0';
                    registrationForm.idBox.charCount++;
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
        ClearBackground(WHITE);
        DrawRectangleRec(dropdownRect, WHITE);
        DrawRectangleLines(dropdownRect.x, dropdownRect.y, dropdownRect.width, dropdownRect.height, BLACK);

        if (selectedItem != -1) {
            DrawText(items[selectedItem], dropdownRect.x + 10, dropdownRect.y + 8, 20, BLACK);
        }

        if (dropdownActive) {
            for (int i = 0; i < 2; i++) {
                Rectangle itemRect = { dropdownRect.x, dropdownRect.y + dropdownRect.height + 2 + 30 * i, dropdownRect.width, 30 };
                DrawRectangleRec(itemRect, WHITE);
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
        drawTextBox(registrationForm.idBox, "Student ID:", true);
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
            case SCHOOL_ID:
                if (registrationForm.idBox.charCount > 0) {
                    registrationForm.idBox.input[registrationForm.idBox.charCount - 1] = '\0';
                    registrationForm.idBox.charCount--;
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

                    saveUserData(registrationForm.firstNameBox.input, registrationForm.lastNameBox.input, registrationForm.idBox.input, registrationForm.passwordBox.input, registrationForm.items.input);

                    registrationForm.firstNameBox.charCount = 0;
                    registrationForm.lastNameBox.charCount = 0;
                    registrationForm.idBox.charCount = 0;
                    registrationForm.passwordBox.charCount = 0;
                    registrationForm.confirmPasswordBox.charCount = 0;

                    memset(registrationForm.firstNameBox.input, 0, sizeof(registrationForm.firstNameBox.input));
                    memset(registrationForm.lastNameBox.input, 0, sizeof(registrationForm.lastNameBox.input));
                    memset(registrationForm.idBox.input, 0, sizeof(registrationForm.idBox.input));
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
                const char* studentID = "";
                const char* password = "";
                std::string items = "";

                saveUserData(firstName, lastName, studentID, password, items);
                //draw student 
            }
        }

        EndDrawing();
    }
}

void DrawMainMenu()
{
    int key = 0;
    activeBox = SCHOOL_ID;

    while (!WindowShouldClose())
    {
        key = GetCharPressed();

        if (isMouseOverBox(mainMenu.idBox.box))
        {
            activeBox = FIRST_NAME;
            while (key > 0)
            {

                if ((key >= 32) && (key <= 125) && (mainMenu.idBox.charCount < 15))
                {

                    mainMenu.idBox.input[mainMenu.idBox.charCount] = (char)key;
                    mainMenu.idBox.input[mainMenu.idBox.charCount + 1] = '\0';
                    mainMenu.idBox.charCount++;

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
        ClearBackground(RAYWHITE);

        DrawText("Digital Academix", GetScreenWidth() / 2 - MeasureText("Digital Academix", 60) / 2, 50, 60, BLUE);

        if (isMouseOverBox(mainMenu.idBox.box))
        {
            activeBox = SCHOOL_ID;
        }
        else if (isMouseOverBox(mainMenu.passwordBox.box))
        {
            activeBox = PASSWORD;
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            switch (activeBox) {
            case SCHOOL_ID:
                if (registrationForm.idBox.charCount > 0) {
                    registrationForm.idBox.input[registrationForm.idBox.charCount - 1] = '\0';
                    registrationForm.idBox.charCount--;
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

        mainMenu.idBox.box.x = GetScreenWidth() / 2 - mainMenu.idBox.box.width / 2;
        mainMenu.idBox.box.y = GetScreenHeight() / 2 - 60;
        mainMenu.passwordBox.box.x = GetScreenWidth() / 2 - mainMenu.passwordBox.box.width / 2;
        mainMenu.passwordBox.box.y = GetScreenHeight() / 2;

        drawTextBox(mainMenu.idBox, "Student ID:", true);
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
                const char* studentID = "";
                const char* password = "";

                if (checkExistingAccount(studentID, password))
                {
                    //Display Student Menu
                }
                else
                {
                    std::cout << "Invalid Student ID or password. Please try again." << std::endl;
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
        DrawMainMenu();
    }
}
