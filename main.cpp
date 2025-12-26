// main.cpp
#include<iostream>
#include<termios.h>
#include<unistd.h>
#include "password_storage.h"
#include "password_generator.h"
using namespace std;

// Reads password without displaying on screen for security
string getPasswordInput(const string& prompt){
  cout<<prompt;
  cout.flush();
  termios oldt;
  tcgetattr(STDIN_FILENO, &oldt);
  termios newt = oldt;
  newt.c_lflag &= ~ECHO;  // Disable terminal echo
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  string password;
  getline(cin, password);
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  // Restore terminal echo
  cout<<endl;
  return password;
}

void printHeader(){
  cout<<"\n";
  cout<<"========================================\n";
  cout<<"   Secure Password Manager v1.0\n";
  cout<<"   Keep your passwords safe & secure\n";
  cout<<"========================================\n";
  cout<<"\n";
}

void printMenu(){
  cout<<"\n--- What would you like to do? ---\n";
  cout<<"1. Generate and save a new password\n";
  cout<<"2. Get a saved password\n";
  cout<<"3. See all your saved entries\n";
  cout<<"4. Delete an entry\n";
  cout<<"5. Exit\n";
  cout<<"\nYour choice: ";
}

int main(){
  try{
    printHeader();
    // Get and verify master password
    string masterPassword = getPasswordInput("Enter your master password: ");
    if(masterPassword.length()<8){
      cout<<"\n[ERROR] Master password must be at least 8 characters!\n";
      return 1;
    }
    // Initialize storage with master password
    PasswordStorage storage("passwords_secure.json", masterPassword);
    cout<<"\nLoading your password vault...";
    try{
      storage.load();
      cout<<" Done!\n";
      cout<<"Found "<<storage.getEntryCount()<<" saved password(s).\n";
    }catch(const exception& e){
      cout<<"\n[WARNING] "<<e.what()<<"\n";
      cout<<"Starting with a fresh vault.\n";
    }
    PasswordGenerator generator;
    // Main program loop
    while(true){
      printMenu();
      int choice;
      cin>>choice;
      cin.ignore();
      if(choice==1){
        // Generate and save new password
        cout<<"\n--- Generate New Password ---\n";
        string service, username, notes;
        int length;
        cout<<"Service name (e.g., Gmail, GitHub): ";
        getline(cin, service);
        cout<<"Username or email: ";
        getline(cin, username);
        cout<<"Password length (minimum 8): ";
        cin>>length;
        cin.ignore();
        cout<<"Notes (optional, press Enter to skip): ";
        getline(cin, notes);
        try{
          string password = generator.generate(length);
          int strength = generator.calculateStrength(password);
          cout<<"\nGenerated Password: "<<password<<"\n";
          cout<<"Strength: "<<strength<<"/100 ("<<generator.getStrengthDescription(strength)<<")\n";
          storage.addEntry(service, username, password, notes);
          storage.save();
          cout<<"\n[SUCCESS] Password saved securely!\n";
        }catch(const exception& e){
          cout<<"\n[ERROR] "<<e.what()<<"\n";
        }
      }else if(choice==2){
        // Retrieve password
        cout<<"\n--- Retrieve Password ---\n";
        string service, username;
        cout<<"Service name: ";
        getline(cin, service);
        cout<<"Username (press Enter to skip): ";
        getline(cin, username);
        try{
          string password = storage.getPassword(service, username);
          cout<<"\nPassword: "<<password<<"\n";
        }catch(const exception& e){
          cout<<"\n[ERROR] "<<e.what()<<"\n";
        }
      }else if(choice==3){
        // List all entries
        auto entries = storage.listEntries();
        cout<<"\n========================================\n";
        cout<<"         Your Saved Passwords\n";
        cout<<"========================================\n\n";
        if(entries.empty()){
          cout<<"  (No passwords saved yet)\n";
        }else{
          for(size_t i=0; i<entries.size(); i++){
            const auto& e = entries[i];
            cout<<"  ["<<(i+1)<<"] "<<e.service<<"\n";
            cout<<"      Username: "<<e.username<<"\n";
            cout<<"      Created: "<<e.timestamp<<"\n";
            if(!e.notes.empty()){
              cout<<"      Notes: "<<e.notes<<"\n";
            }
            cout<<"      ----------------------------\n";
          }
        }
      }else if(choice==4){
        // Delete entry
        cout<<"\n--- Delete Entry ---\n";
        string service, username;
        cout<<"Service name: ";
        getline(cin, service);
        cout<<"Username (press Enter to delete all for this service): ";
        getline(cin, username);
        cout<<"\n[WARNING] Are you sure? This cannot be undone! (y/n): ";
        char confirm;
        cin>>confirm;
        cin.ignore();
        if(confirm=='y' || confirm=='Y'){
          if(storage.deleteEntry(service, username)){
            storage.save();
            cout<<"\n[SUCCESS] Entry deleted successfully!\n";
          }else{
            cout<<"\n[ERROR] Entry not found!\n";
          }
        }else{
          cout<<"\nDeletion cancelled.\n";
        }
      }else if(choice==5){
        // Exit program
        cout<<"\nThanks for using Secure Password Manager!\n";
        cout<<"Stay safe online!\n\n";
        break;
      }else{
        cout<<"\n[ERROR] Invalid choice! Please select 1-5.\n";
      }
    }
  }catch(const exception& e){
    cerr<<"\n[FATAL ERROR] "<<e.what()<<"\n";
    return 1;
  }
  return 0;
}
