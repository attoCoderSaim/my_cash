#include <bits/stdc++.h>
#include <ctime>
#include <cstdlib>

using namespace std;

class Member
{
public:
    string mobile;
    string name;
    double amount;
    string pin;

    Member(string mobile, string name, double amount, string pin)
        : mobile(mobile), name(name), amount(amount), pin(pin) {}
};

class MyCashApp
{
private:
    vector<Member> members;
    int currentUserIndex = -1;
    string currentMobile;
    int transactionId;
    string historyFile;

public:
    MyCashApp()
    {
        loadMembers();
    }

    void loadMembers()
    {
        ifstream file("members.txt");
        if (file)
        {
            string mobile, name, pin;
            double amount;
            while (file >> mobile >> name >> amount >> pin)
            {
                members.push_back(Member(mobile, name, amount, pin));
            }
            file.close();
        }
    }

    void saveMembers()
    {
        ofstream file("members.txt");
        for (const Member& member : members)
        {
            file << member.mobile << " " << member.name << " " << member.amount << " " << member.pin << endl;
        }
        file.close();
    }

    bool isMobileExist(const string& mobile)
    {
        if (members.empty()) {
            return false;
        }
        for (const Member& member : members)
        {
            if (member.mobile == mobile)
            {
                return true;
            }
        }
        return false;
    }

    int generateOTP()
    {
        srand(time(0));
        return 1000 + rand() % 9000;
    }

    void clearScreen()
    {
        system("cls");
    }

    void pause()
    {
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }

    void registerMember()
    {
        clearScreen();

        string mobile, name, pin;
        double amount;
        cout << "Enter Mobile No. (11-digit): ";
        cin >> mobile;
        cin.ignore();
        if (mobile.length() != 11)
        {
            cout << "Please enter an 11-digit mobile number" << endl;
            pause();
            return;
        }

        if (isMobileExist(mobile)==1)
        {
            cout << "Already registered!" << endl;
            pause();
            return;
        }

        cout << "Enter Name (use underscore or single name, no spaces): ";
        cin.ignore();
        getline(cin, name);

        if (name.find(' ') != string::npos)
        {
            cout << "Invalid name. Please use an underscore or a single name with no spaces. " << endl;
            pause();
            return;
        }

        cout << "Enter pin (5-digit): ";
        cin >> pin;

        string confirmPin;
        cout << "Reconfirm pin: ";
        cin >> confirmPin;

        if (pin != confirmPin)
        {
            cout << "Pins must be the same" << endl;
            pause();
            return;
        }

        int otp = generateOTP();
        cout << "Your OTP is: " << otp << endl;

        int enteredOTP;
        cout << "Enter OTP: ";
        cin >> enteredOTP;

        if (enteredOTP != otp)
        {
            cout << "OTP does NOT match" << endl;
            pause();
            return;
        }

        members.push_back(Member(mobile, name, 0.0, pin));
        saveMembers();
        cout << "Registration is Successful" << endl;
        pause();
    }


    bool loginMember()
    {
        clearScreen();
        string mobile, pin;
        cout << "Enter Mobile No. (11-digit): ";
        cin >> mobile;

        int memberIndex = -1;
        for (int i = 0; i < members.size(); i++)
        {
            if (members[i].mobile == mobile)
            {
                memberIndex = i;
                break;
            }
        }

        if (memberIndex == -1)
        {
            cout << "Member NOT exists" << endl;
            pause();
            return false;
        }

        cout << "Enter pin: ";
        cin >> pin;

        if (members[memberIndex].pin != pin)
        {
            cout << "Invalid login" << endl;
            pause();
            return false;
        }

        currentUserIndex = memberIndex;
        currentMobile = mobile;
        cout << "Login is Successful" << endl;
        pause();
        return true;
    }

    void mainMenu()
    {
        if (currentUserIndex == -1)
        {
            cout << "You must log in first" << endl;
            pause();
            return;
        }

        while (true)
        {
            clearScreen();
            cout << "********** MyCash Menu ********" << endl;
            cout << "1. Update Me" << endl;
            cout << "2. Remove Me" << endl;
            cout << "3. Send Money" << endl;
            cout << "4. Cash-in" << endl;
            cout << "5. Cash-out" << endl;
            cout << "6. Pay Bill" << endl;
            cout << "7. Check Balance" << endl;
            cout << "8. History" << endl;
            cout << "9. Logout" << endl;
            cout << "Enter Your Option (1-9): ";
            int option;
            cin >> option;

            switch (option)
            {
            case 1:
                updateMember();
                break;
            case 2:
                removeMember();
                break;
            case 3:
                sendMoney();
                break;
            case 4:
                cashIn();
                break;
            case 5:
                cashOut();
                break;
            case 6:
                payBill();
                break;
            case 7:
                checkBalance();
                break;
            case 8:
                displayTransactionHistory();
                break;
            case 9:
                logout();
                return;
            default:
                cout << "Invalid Option" << endl;
                pause();
            }
        }
    }

    void updateMember()
    {
        if (currentUserIndex == -1)
        {
            cout << "You must log in first" << endl;
            pause();
            return;
        }
        clearScreen();
        Member& member = members[currentUserIndex];
        cout << "Old Name: " << member.name << endl;

        string newName;
        cout << "New Name (enter to ignore): ";
        cin.ignore();
        getline(cin, newName);

        if (!newName.empty())
        {
            member.name = newName;
        }

        string oldPin = member.pin;
        string newPin;
        string confirmNewPin;

        cout << "Old pin: " << string(oldPin.length(), '*') << endl;
        cout << "New pin (enter to ignore): ";
        cin >> newPin;

        if (!newPin.empty())
        {
            cout << "Confirm New pin: ";
            cin >> confirmNewPin;
            if (newPin != confirmNewPin)
            {
                cout << "Pins must be the same" << endl;
                pause();
                return;
            }
            member.pin = newPin;
        }

        int otp = generateOTP();
        cout << "myCash OTP: " << otp << endl;

        int enteredOTP;
        cout << "Enter OTP: ";
        cin >> enteredOTP;

        if (enteredOTP != otp)
        {
            cout << "OTP does NOT match" << endl;
            pause();
            return;
        }

        saveMembers();
        cout << "Update is Successful" << endl;
        pause();
    }

    void removeMember()
    {
        if (currentUserIndex == -1)
        {
            cout << "You must log in first" << endl;
            pause();
            return;
        }
        clearScreen();
        Member& member = members[currentUserIndex];
        int otp = generateOTP();
        cout << "myCash OTP: " << otp << endl;

        int enteredOTP;
        cout << "Enter OTP: ";
        cin >> enteredOTP;

        if (enteredOTP != otp)
        {
            cout << "OTP does NOT match" << endl;
            pause();
            return;
        }

        members.erase(members.begin() + currentUserIndex);
        saveMembers();
        currentUserIndex = -1;
        currentMobile = "";
        cout << "Remove is Successful" << endl;
        pause();
        return;
    }

    void sendMoney()
    {
        if (currentUserIndex == -1)
        {
            cout << "You must log in first" << endl;
            pause();
            return;
        }

        clearScreen();
        cout << "Enter Destination no. (11-digit): ";
        string destinationMobile;
        cin >> destinationMobile;

        if (!isMobileExist(destinationMobile))
        {
            cout << "Destination Mobile no. is invalid" << endl;
            pause();
            return;
        }

        Member& sender = members[currentUserIndex];
        double amount;
        cout << "Enter Amount: ";
        cin >> amount;

        if (sender.amount < amount)
        {
            cout << "Insufficient Fund" << endl;
            pause();
            return;
        }

        int otp = generateOTP();
        cout << "myCash OTP: " << otp << endl;

        int enteredOTP;
        cout << "Enter OTP: ";
        cin >> enteredOTP;

        if (enteredOTP != otp)
        {
            cout << "OTP does NOT match" << endl;
            pause();
            return;
        }

        int recipientIndex = -1;
        for (size_t i = 0; i < members.size(); i++)
        {
            if (members[i].mobile == destinationMobile)
            {
                recipientIndex = i;
                break;
            }
        }

        if (recipientIndex == -1)
        {
            cout << "Recipient not found" << endl;
            pause();
            return;
        }

        sender.amount -= amount;
        members[recipientIndex].amount += amount;
        saveMembers();

        logTransactionRecieved(amount, destinationMobile, recipientIndex);
        logTransaction("Send-Money", -amount);

        cout << "Send Money is Successful" << endl;
        pause();
    }
    void cashIn()
    {
        if (currentUserIndex == -1)
        {
            cout << "You must log in first" << endl;
            pause();
            return;
        }

        clearScreen();
        cout << "Enter Mobile No. to Cash-in (11-digit): ";
        string recipientMobile;
        cin >> recipientMobile;

        if (!isMobileExist(recipientMobile))
        {
            cout << "Recipient Mobile no. is invalid" << endl;
            pause();
            return;
        }

        double a;
        cout << "Enter Amount: ";
        cin >> a;

        cout << "Cash-in " << a << endl;
        cout << "Are you sure (Y/N)? ";
        char confirm;
        cin >> confirm;

        if (confirm != 'Y' && confirm != 'y')
        {
            cout << "Cash-in operation canceled" << endl;
            pause();
            return;
        }


        int recipientIndex = -1;
        for (int i = 0; i < members.size(); i++)
        {
            if (members[i].mobile == recipientMobile)
            {
                recipientIndex = i;
                break;
            }
        }

        if (recipientIndex != -1)
        {
            members[recipientIndex].amount += a;
            saveMembers();
            logTransaction("Cash-in", a);
            cout << "Cash-in is Successful" << endl;
        }
        else
        {
            cout << "Recipient not found. Cash-in operation canceled." << endl;
        }
        pause();
    }

    void cashOut()
    {
        if (currentUserIndex == -1)
        {
            cout << "You must log in first." << endl;
            pause();
            return;
        }
        clearScreen();
        Member& member = members[currentUserIndex];

        double amount;
        cout << "Enter Amount to Cash-out: ";
        cin >> amount;

        string pin;
        cout << "Enter Your PIN: ";
        cin >> pin;

        if (pin != member.pin)
        {
            cout << "Invalid PIN. Cash-out operation canceled." << endl;
            pause();
            return;
        }

        if (amount > member.amount)
        {
            cout << "Insufficient funds. Cash-out operation canceled." << endl;
            pause();
            return;
        }

        member.amount -= amount;
        saveMembers();
        logTransaction("Cash-out", -amount);

        cout << "Cash-out is Successful" << endl;
        pause();
    }

    void payBill()
    {
        if (currentUserIndex == -1)
        {
            cout << "You must log in first" << endl;
            pause();
            return;
        }
        clearScreen();
        Member& payer = members[currentUserIndex];

        string recipientMobile;
        double billAmount;
        string pin;

        cout << "Enter Recipient's Mobile No. (11-digit): ";
        cin >> recipientMobile;

        if (!isMobileExist(recipientMobile))
        {
            cout << "Recipient Mobile no. is invalid" << endl;
            pause();
            return;
        }

        int recipientIndex = -1;
        for (int i = 0; i < members.size(); i++)
        {
            if (members[i].mobile == recipientMobile)
            {
                recipientIndex = i;
                break;
            }
        }

        cout << "Enter Bill Amount: ";
        cin >> billAmount;

        if (billAmount <= 0)
        {
            cout << "Invalid Bill Amount. Payment canceled." << endl;
            pause();
            return;
        }

        cout << "Enter Your PIN: ";
        cin >> pin;

        if (pin != payer.pin)
        {
            cout << "Invalid PIN. Bill payment canceled." << endl;
            pause();
            return;
        }

        if (billAmount > payer.amount)
        {
            cout << "Insufficient funds. Bill payment canceled." << endl;
            pause();
            return;
        }

        int otp = generateOTP();
        cout << "myCash OTP: " << otp << endl;

        int enteredOTP;
        cout << "Enter OTP: ";
        cin >> enteredOTP;

        if (enteredOTP != otp)
        {
            cout << "OTP does NOT match. Bill payment canceled." << endl;
            pause();
            return;
        }

        payer.amount -= billAmount;
        members[recipientIndex].amount += billAmount;
        saveMembers();
        logTransaction("Bill-payment", -billAmount);

        logTransactionBill(billAmount, recipientMobile, recipientIndex);
        cout << "Bill Payment is Successful." << endl;
        pause();
    }

    void checkBalance()
    {
        if (currentUserIndex == -1)
        {
            cout << "You must log in first." << endl;
            pause();
            return;
        }
        clearScreen();
        Member& member = members[currentUserIndex];
        cout << "Balance: " << member.amount << endl;
        pause();
    }

    void displayTransactionHistory()
    {
        if (currentUserIndex == -1)
        {
            cout << "You must log in first." << endl;
            pause();
            return;
        }
        clearScreen();
        string loggedInMobile = members[currentUserIndex].mobile;
        historyFile = "transaction_history_" + members[currentUserIndex].mobile + ".txt";

        ifstream file(historyFile);
        if (file)
        {
            cout << left << setw(20) << "Transaction ID" << setw(20) << "Description" << setw(20) << "Amount" << "Balance" << endl;
            string line;
            bool foundTransactions = false;
            while (getline(file, line))
            {
                istringstream iss(line);
                long long tranId;
                string description;
                double amount, balance;
                iss >> tranId >> description >> amount >> balance;

                if (to_string(balance) == loggedInMobile || description == "Cash-in" || description == "Cash-out" || description == "Received-Bill" || description == "Bill-payment" || description == "Send-Money" || description == "Received")
                {
                    cout << left << setw(20) << tranId << setw(20) << description << setw(20) << amount << balance << endl;
                    foundTransactions = true;
                }
            }

            file.close();

            if (!foundTransactions)
            {
                cout << "No transactions found for the current user." << endl;
            }
        }
        else
        {
            cout << "Transaction history file not found or could not be opened." << endl;
        }

        pause();
    }

    void logout()
    {
        currentUserIndex = -1;
        currentMobile = "";
    }

    long long generateTransactionID()
    {
        auto now = chrono::system_clock::now();
        auto timestamp = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()).count();

        return timestamp;
    }

    void logTransactionRecieved(double amount, const string& recipientMobile = "", int recipientIndex=-1)
    {
        historyFile = "transaction_history_" + recipientMobile + ".txt";
        Member& memu = members[recipientIndex];
        ofstream file(historyFile, ios::app);
        long long transactionID = generateTransactionID();
        if (file)
        {
            file << transactionID << " " << "Received-Money" << " " << amount << " " << memu.amount << endl;
            file.close();
        }
    }

    void logTransactionBill(double amount, const string& recipientMobile = "", int recipientIndex=-1)
    {
        historyFile = "transaction_history_" + recipientMobile + ".txt";
        Member& memu = members[recipientIndex];
        ofstream file(historyFile, ios::app);
        long long transactionID = generateTransactionID();
        if (file)
        {
            file << transactionID << " " << "Received-Bill" << " " << amount << " " << memu.amount << endl;
            file.close();
        }
    }

    void logTransaction(const string& description, double amount)
    {
        if (currentUserIndex == -1)
        {
            return;
        }

        Member& member = members[currentUserIndex];

        historyFile = "transaction_history_" + member.mobile + ".txt";
        long long transactionID = generateTransactionID();


        ofstream file(historyFile, ios::app);
        if (file)
        {
            file << transactionID << " " << description << " " << amount << " " << member.amount << endl;
            file.close();
        }
    }
};

int main()
{
    MyCashApp app;
    cout << fixed << setprecision(2);
    while (true)
    {
        app.clearScreen();
        cout << "*** MyCash Login ***" << endl;
        cout << "1. Login" << endl;
        cout << "2. Register" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter Your Option: ";
        int option;
        cin >> option;

        switch (option)
        {
        case 1:
            if (app.loginMember())
            {
                app.mainMenu();
            }
            break;
        case 2:
            app.registerMember();
            break;
        case 3:
            app.saveMembers();
            exit(0);
        default:
            cout << "Invalid option. Please try again." << endl;
            app.pause();
        }
    }
    return 0;
}
