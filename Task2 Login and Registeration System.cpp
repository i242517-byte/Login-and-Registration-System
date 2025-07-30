#include <iostream>
#include <fstream>
#include <cstring> // for strcmp

using namespace std;

const int MAX_USERS = 100;
const int MAX_USERNAME_LENGTH = 50;
const int MAX_PASSWORD_LENGTH = 50;

struct User {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
};

User users[MAX_USERS];
int userCount = 0;

void loadUsers();
void saveUsers();
void registerUser();
bool loginUser();
bool isUsernameTaken(const char* username);
void clearInputBuffer();

int main() {
    loadUsers();
    
    int choice;
    do {
        cout << "\nWelcome to the Login System\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        clearInputBuffer();
        
        switch(choice) {
            case 1:
                registerUser();
                break;
            case 2:
                if(loginUser()) {
                    cout << "\nLogin successful! Welcome!\n";
                    return 0;
                } else {
                    cout << "\nLogin failed. Invalid username or password.\n";
                }
                break;
            case 3:
                cout << "Exiting program...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while(choice != 3);
    
    saveUsers();
    return 0;
}

void loadUsers() {
    ifstream file("credentials.txt");
    if (!file.is_open()) return;
    
    char line[MAX_USERNAME_LENGTH + MAX_PASSWORD_LENGTH + 2]; // +2 for ':' and null terminator
    while(file.getline(line, sizeof(line))) {
        int i = 0;
        while(line[i] != ':' && line[i] != '\0') {
            users[userCount].username[i] = line[i];
            i++;
        }
        users[userCount].username[i] = '\0';
        
        if(line[i] == ':') {
            i++;
            int j = 0;
            while(line[i] != '\0' && j < MAX_PASSWORD_LENGTH - 1) {
                users[userCount].password[j] = line[i];
                i++;
                j++;
            }
            users[userCount].password[j] = '\0';
            userCount++;
        }
    }
    file.close();
}

void saveUsers() {
    ofstream file("credentials.txt");
    for(int i = 0; i < userCount; i++) {
        file << users[i].username << ":" << users[i].password << endl;
    }
    file.close();
}

void registerUser() {
    if(userCount >= MAX_USERS) {
        cout << "Maximum number of users reached.\n";
        return;
    }
    
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    
    cout << "\n--- Registration ---\n";
    
    // Get username with validation
    while(true) {
        cout << "Enter username (no spaces, at least 4 characters): ";
        cin.getline(username, MAX_USERNAME_LENGTH);
        
        bool valid = true;
        bool hasSpace = false;
        int length = 0;
        
        // Check for spaces and length
        while(username[length] != '\0') {
            if(username[length] == ' ') {
                hasSpace = true;
                break;
            }
            length++;
        }
        
        if(hasSpace) {
            cout << "Username cannot contain spaces. Try again.\n";
            continue;
        }
        
        if(length < 4) {
            cout << "Username must be at least 4 characters long. Try again.\n";
            continue;
        }
        
        if(isUsernameTaken(username)) {
            cout << "Username already taken. Please choose another.\n";
            continue;
        }
        
        break;
    }
    
    // Get password with validation
    while(true) {
        cout << "Enter password (at least 6 characters): ";
        cin.getline(password, MAX_PASSWORD_LENGTH);
        
        int length = 0;
        while(password[length] != '\0') {
            length++;
        }
        
        if(length < 6) {
            cout << "Password must be at least 6 characters long. Try again.\n";
            continue;
        }
        
        break;
    }
    
    // Add new user
    strcpy(users[userCount].username, username);
    strcpy(users[userCount].password, password);
    userCount++;
    
    saveUsers();
    cout << "\nRegistration successful! You can now login.\n";
}

bool loginUser() {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    
    cout << "\n--- Login ---\n";
    cout << "Enter username: ";
    cin.getline(username, MAX_USERNAME_LENGTH);
    cout << "Enter password: ";
    cin.getline(password, MAX_PASSWORD_LENGTH);
    
    for(int i = 0; i < userCount; i++) {
        if(strcmp(users[i].username, username) == 0 && 
           strcmp(users[i].password, password) == 0) {
            return true;
        }
    }
    return false;
}

bool isUsernameTaken(const char* username) {
    for(int i = 0; i < userCount; i++) {
        if(strcmp(users[i].username, username) == 0) {
            return true;
        }
    }
    return false;
}

void clearInputBuffer() {
    while(cin.get() != '\n') {
        continue;
    }
}
