#pragma once
#include "raylib.h"
#include <string> 
#include <fstream>
#include <iostream>

// Define a class for credential input box
class CREDENTIAL_BOX
{
public:
    char input[16];
    int charCount;
    Rectangle box;
    int framesCounter;
};
// Define a class for the main menu
class MAIN_MENU
{
public:
    CREDENTIAL_BOX usernameBox;
    CREDENTIAL_BOX passwordBox;
    Rectangle continueButton;
    Rectangle createAccountButton;
};
// Define a class for user details
class User {
public:
    std::string Username;
};
// Define a class for registration form
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
// Define global input boxes and buttons
CREDENTIAL_BOX firstNameBox{ "", 0, {GetScreenWidth() / 2 + 300, 140, 200, 30}, 0 };
CREDENTIAL_BOX lastNameBox{ "", 0, {GetScreenWidth() / 2 + 300, 200, 200, 30}, 0 };
CREDENTIAL_BOX usernameBox{ "", 0, {GetScreenWidth() / 2 + 300, 260, 200, 30}, 0 };
CREDENTIAL_BOX passwordBox{ "", 0, {GetScreenWidth() / 2 + 300, 320, 200, 30}, 0 };
CREDENTIAL_BOX confirmPasswordBox{ "", 0, {GetScreenWidth() / 2 + 300, 380, 200, 30}, 0 };
Rectangle registerButton{ GetScreenWidth() / 2 + 300, 100, 200, 40 };
// Enum to track active input box
enum ActiveBox {
    FIRST_NAME,
    LAST_NAME,
    USERNAME,
    PASSWORD,
    CONFIRM_PASSWORD
};
// Active input box
ActiveBox activeBox = USERNAME;
bool registrationFormActive = false; // Flag to indicate registration form activation
// Define main menu components

MAIN_MENU mainMenu = {
    { "", 0, {GetScreenWidth() / 2 + 310, GetScreenHeight() / 2 + 240, 200, 30}, 0 },
    { "", 0, {GetScreenWidth() / 2 + 310, GetScreenHeight() / 2 + 310, 200, 30}, 0 },
    { GetScreenWidth() / 2 + 310, GetScreenHeight() / 2 + 400, 200, 40 },
    { GetScreenWidth() / 2 + 310, GetScreenHeight() / 2 + 450, 200, 40 }
};
// Define registration form components

REGISTRATION_FORM registrationForm =
{
    { "", 0, {GetScreenWidth() / 2 + 300, 140, 200, 30}, 0 },
    { "", 0, {GetScreenWidth() / 2 + 300, 200, 200, 30}, 0 },
    { "", 0, {GetScreenWidth() / 2 + 300, 260, 200, 30}, 0 },
    { "", 0, {GetScreenWidth() / 2 + 300, 320, 200, 30}, 0 },
    { "", 0, {GetScreenWidth() / 2 + 300, 380, 200, 30}, 0 },
    { GetScreenWidth() / 2 + 500, 700, 200, 40 }
};
// Rectangle for dropdown menu
Rectangle dropdownRect = { 300, 430, 200, 30 };
int selectedItem = -1;
const char* items[2] = { "Student", "Teacher" };
bool dropdownActive = false;

class userSaver {
public:

    void saveUserData(const char* firstName, const char* lastName, const char* username, const char* password, std::string items);

    const char* firstName;
    const char* lastName;

private:
    const char* username;
    const char* password;
    std::string items;
};

typedef struct {
    char subject[20];
    int grade;
    bool completed;
} Test;

bool isMouseOverBox(Rectangle box);
void drawTextBox(CREDENTIAL_BOX& textBox, const char* label, bool showCharacters);
bool checkRequirements(const char* str);
bool checkAccount(const char* username, const char* password);
userSaver currentUser;
void studentMenu();
void mathTest();
void biologyTest();
void geographyTest();
void physicsTest();
void chemistryTest();
void englishTest();
void historyTest();
void studentMenu();//
void RegisterForm();
void DrawMainMenu();

