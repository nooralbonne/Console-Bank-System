#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

string ClientsFileName = "Clients.txt";
void ShowMainMenue();
void ShowTransactionsMenue();
void GoBackToMainMenue();

struct sClient {
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

vector<string> SplittingString(string S1, string Delim) {
    vector<string> vClients;
    int pos = 0;
    string sWord;
    while ((pos = S1.find(Delim)) != string::npos) {
        sWord = S1.substr(0, pos);
        if (sWord != " ") {
            vClients.push_back(sWord);
        }
        S1.erase(0, pos + Delim.length());
    }

    if (S1 != " ") {
        vClients.push_back(S1);
    }
    return vClients;
}

sClient ConvertLineToRecord(string S1, string Delim) {
    sClient Client;
    vector<string> LineInFile = SplittingString(S1, Delim);

    if (LineInFile.size() >= 5) {
        Client.AccountNumber = LineInFile[0];
        Client.PinCode = LineInFile[1];
        Client.Name = LineInFile[2];
        Client.Phone = LineInFile[3];
        Client.AccountBalance = stod(LineInFile[4]);
    }
    else {
        cout << "Error: Invalid line format in file" << endl;
    }
    return Client;
}

vector<sClient> LoadClientsDataFromFile(string FileName) {
    vector<sClient> vClients;
    fstream myFile;
    myFile.open(FileName, ios::in);

    if (!myFile.is_open()) {
        cout << "Error opening file: " << FileName << endl;
        return vClients;
    }

    string DataLine;
    sClient Client;
    while (getline(myFile, DataLine)) {
        Client = ConvertLineToRecord(DataLine, "#$$#");
        vClients.push_back(Client);
    }
    myFile.close();

    return vClients;
}

void PrintClientRecordLine(sClient Client) {
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
    cout << " |" << endl;
}

void ShowAllClientsScreen()
{
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else
        for (sClient Client : vClients)
        {
            PrintClientRecordLine(Client);
            cout << endl;
        }
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

//=============AddNewClient==========
sClient ReadNewClient() {
    sClient Client;
    cout << "Adding New Client " << endl;
    cout << "Enter Account Number?" << endl;
    getline(cin >> ws, Client.AccountNumber);
    cout << "Enter Your Name?" << endl;
    getline(cin, Client.Name);
    cout << "Enter PinCode?" << endl;
    cin >> Client.PinCode;
    cout << "Enter phone Number?" << endl;
    cin >> Client.Phone;
    cout << "Enter Account Balance?" << endl;
    cin >> Client.AccountBalance;

    return Client;
}

string ConvertRecordToLine(sClient Client, string Separator) {
    string Line = "";
    Line += Client.AccountNumber + Separator;
    Line += Client.PinCode + Separator;
    Line += Client.Name + Separator;  // Missing Name field added
    Line += Client.Phone + Separator;
    Line += to_string(Client.AccountBalance);
    return Line;
}

void AddDataToFile(string FileName, string stData) {
    fstream myFile;
    myFile.open(FileName, ios::app);
    if (myFile.is_open()) {
        myFile << stData << endl;
    }
    myFile.close();
}

void AddNewClient() {
    sClient Client;
    Client = ReadNewClient();
    AddDataToFile(ClientsFileName, ConvertRecordToLine(Client, "#$$#"));
}

void AddNewClientScreen() {
    cout << "-----------------------------" << endl;
    cout << "\tAdd New Client Screen" << endl;
    cout << "-----------------------------" << endl;
    char answer = 'Y';
    do {
        AddNewClient();
        cout << "Do You Want to add more?" << endl;
        cin >> answer;
    } while (answer == 'Y' || answer == 'y');
}

//===≈=======Delete Client===================
string ReadAccountNumber() {
    string AccountNumber;
    cout << "Please enter Account Number: ";
    cin >> AccountNumber;
    return AccountNumber;
}

bool FindClientByAccountNumber(const string& AccountNumber, vector<sClient>& vClients, sClient& Client) {
    for (sClient& C : vClients) {
        if (C.AccountNumber == AccountNumber) {
            Client = C;
            return true;
        }
    }
    return false;
}

void PrintClientCard(sClient Client) {
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number: " << Client.AccountNumber;
    cout << "\nPin Code : " << Client.PinCode;
    cout << "\nName : " << Client.Name;
    cout << "\nPhone : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";
}

vector <sClient> SaveCleintsDataToFile(string FileName, vector  <sClient> vClients) {
    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite
    string DataLine;
    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {
            if (C.MarkForDelete == false)
            {
                DataLine = ConvertRecordToLine(C, "#$$#");
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vClients;
}

bool MarkForDelete(string AccountNumber, vector<sClient>& vClients) {
    for (sClient& C : vClients) {
        if (C.AccountNumber == AccountNumber) {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient>& vClients) {
    sClient Client;
    char Answer;

    if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
        PrintClientCard(Client);
        cout << "\nAre you sure you want to delete this client? (y/n): ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y') {
            MarkForDelete(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);
            vClients = LoadClientsDataFromFile(ClientsFileName);
            cout << "\nClient Deleted Successfully.\n";
            return true;
        }
    }
    else {
        cout << "\nClient with Account Number (" << AccountNumber << ") Not Found!\n";
    }
    return false;
}


void DeleteClientScreen() {
    cout << "-----------------------------\n";
    cout << "\tDelete Client Screen\n";
    cout << "-----------------------------\n";

    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);
}
//===================Update Client=====================
sClient ChangeClientRecord(string AccountNumber) {
    sClient Client;
    cout << "update Client " << endl;

    Client.AccountNumber = AccountNumber;

    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);
    cout << "Enter Name? ";
    getline(cin, Client.Name);
    cout << "Enter Phone? ";
    getline(cin, Client.Phone);
    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;
    return Client;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient>& vClients) {
    sClient Client;
    char Answer;

    if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
        PrintClientCard(Client);
        cout << "\nAre you sure you want to Update this client? (y/n): ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y') {
            for (sClient& C : vClients) {
                if (C.AccountNumber == AccountNumber) {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }
            SaveCleintsDataToFile(ClientsFileName, vClients);
            cout << "Updated successfully" << endl;
        }
    }
    else {
        cout << "\nClient with Account Number (" << AccountNumber << ") Not Found!\n";
    }
    return false;
}

void UpdateClientScreen() {
    cout << "-----------------------------\n";
    cout << "\tUpdate Client Screen\n";
    cout << "-----------------------------\n";

    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);
}
//===================Find Client=====================
void ShowFindClientScreen() {
    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";
    vector <sClient> vClients =
        LoadClientsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadAccountNumber();
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";

}

//////========project 2=========///////////
//============ Deposit ==============
bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector<sClient>& vClients) {
    char answer = 'Y';
    cout << "Are you sure you want to do this Transaction? (y/n): ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        for (sClient& C : vClients) {
            if (C.AccountNumber == AccountNumber) {
                C.AccountBalance += Amount; // Use the Amount provided by user
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "Deposit Done successfully" << endl;
                return true;
            }
        }
        cout << "Client not found" << endl;
        return false;
    }
    return false;
}

void ShowDepositScreen() {
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";

    // Load client data
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    // Get account number and validate
    string AccountNumber = ReadAccountNumber();
    sClient Client;

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client)) {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadAccountNumber();
    }

    // Print client card
    PrintClientCard(Client);

    // Get deposit amount
    double Amount = 0;
    cout << "\nPlease enter deposit amount: ";
    cin >> Amount;

    // Deposit balance to client
    DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);
}

//============ withDraw ==============
void ShowWithDrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";
    sClient Client;
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadAccountNumber();
    while (!FindClientByAccountNumber(AccountNumber, vClients,
        Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadAccountNumber();
    }
    PrintClientCard(Client);
    double Amount = 0;
    cout << "\nPlease enter withdraw amount? ";
    cin >> Amount;
    //Validate that the amount does not exceeds the balance
    while (Amount > Client.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
        cout << "Please enter another amount? ";
        cin >> Amount;
    }
    DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);
}



//============ Total Balance ==============
void PrintClientRecordBalanceLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void showTotalBalance()
{
    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).\n";
    cout << "___________";
    cout << "_________\n" << endl;
    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n___________";
    cout << "_________\n" << endl;

    double TotalBalances = 0;

    if (vClients.size() == 0) {
        cout << "\t\t\t\tNo Clients Available In the System!" << endl;
    }
    else {
        for (sClient Client : vClients) {
            PrintClientRecordBalanceLine(Client);
            TotalBalances += Client.AccountBalance;
            cout << endl;
        }
    }

    cout << "___________";
    cout << "_________\n" << endl;
    cout << "\t\t\t\t\tTotal Balances: $" << fixed << setprecision(2) << TotalBalances << endl;
}

void ShowTotalBalanceScreen() {
    showTotalBalance();
}
//==≈===============================≈=========

short ReadTransactionsMenueOption() {
    short choice;
    cout << "Choose what do you want to do? [ 1 to 4 ]? ";
    cin >> choice;
    return choice;
}
enum enTransactionsMenueOptions {
    eDeposit = 1, eWithdraw = 2,
    eShowTotalBalance = 3, eShowMainMenue = 4
};
void PerfromTranactionsMenueOption(enTransactionsMenueOptions Option) {

    switch (Option) {
    case enTransactionsMenueOptions::eDeposit: {
        system("cls");
        ShowDepositScreen();
        GoBackToMainMenue();
        break;
    }
    case enTransactionsMenueOptions::eWithdraw: {
        system("cls");
        ShowWithDrawScreen();
        GoBackToMainMenue();
        break;
    }
    case enTransactionsMenueOptions::eShowTotalBalance: {
        system("cls");
        ShowTotalBalanceScreen();
        GoBackToMainMenue();
        break;
    }
    case enTransactionsMenueOptions::eShowMainMenue: {
        ShowMainMenue();
    }
    default:
        cout << "Invalid choice, please try again!" << endl;
        ShowMainMenue();
        break;
    }
}
void ShowTransactionsMenue() {
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tTransactions Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";
    cout << "===========================================\n";
    PerfromTranactionsMenueOption((enTransactionsMenueOptions)ReadTransactionsMenueOption());
}

////////////////////=============///////////////
short ReadChoice() {
    short choice;
    cout << "Choose what do you want to do? [ 1 to 6 ]? ";
    cin >> choice;
    return choice;
}

enum enMainMenueOptions {
    eListClients = 1, eAddNewClient = 2,
    eDeleteClient = 3, eUpdateClient = 4,
    eFindClient = 5, eTransactions = 6,
    eExit = 7
};

void GoBackToMainMenue() {
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();
}
void PerfromMainMenueOption(enMainMenueOptions Option) {
    switch (Option) {
    case enMainMenueOptions::eListClients: {
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eAddNewClient: {
        system("cls");
        AddNewClientScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eDeleteClient: {
        // Implementation for deleting a client
        system("cls");
        DeleteClientScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eUpdateClient: {

        system("cls");
        UpdateClientScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eFindClient: {
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;
        break;
    }
    case enMainMenueOptions::eTransactions: {
        ShowTransactionsMenue();
        break;
    }
    case enMainMenueOptions::eExit: {
        cout << "Exiting the system. Goodbye!" << endl;
        exit(0);
    }
    default:
        cout << "Invalid choice, please try again!" << endl;
        ShowMainMenue();
        break;
    }
}

void ShowMainMenue() {
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Exit.\n";
    cout << "===========================================\n";

    PerfromMainMenueOption((enMainMenueOptions)ReadChoice());
}

int main() {
    ShowMainMenue();
    return 0;
}