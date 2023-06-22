#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <unordered_map>
#include <iomanip>

using namespace std;

// Класс для управления финансовыми операциями
class FinanceManager {
private:
    string accountNumber;

public:
    // Метод для создания нового счета
    void addAccount() {
        cout << "Enter account number: ";
        cin >> accountNumber;

        // Проверяем, содержит ли номер счета только числа
        if (!isNumber(accountNumber)) {
            cout << "Invalid account number. Please enter only numbers." << endl;
            return;
        }

        // Создаем файл с номером счета
        ofstream file(accountNumber + ".txt");
        if (file) {
            cout << "Account created successfully." << endl;
        }
        else {
            cout << "Failed to create account." << endl;
        }
    }

    // Метод для внесения депозита
    void deposit() {
        string accountNumber;
        string amountStr;
        double amount;
        string date;

        cout << "Enter account number: ";
        cin >> accountNumber;

        if (!isAccountExists(accountNumber)) {
            cout << "Account does not exist." << endl;
            return;
        }

        cout << "Enter deposit amount: ";
        cin >> amountStr;

        if (!isNumber(amountStr)) {
            cout << "Invalid amount. Please enter only numbers." << endl;
            return;
        }

        amount = stod(amountStr);

        cout << "Enter date (dd:mm:yyyy): ";
        cin >> date;

        if (!isValidDate(date)) {
            cout << "Invalid date. Please enter a valid date." << endl;
            return;
        }

        // Сохраняем депозит в файл с номером счета
        ofstream file(accountNumber + ".txt", ios::app);
        if (file) {
            file << "Deposit: " << amount << " Date: " << date << endl;
            cout << "Deposit added successfully." << endl;
        }
        else {
            cout << "Failed to add deposit." << endl;
        }
    }

    // Метод для снятия средств
    void withdraw() {
        string accountNumber;
        string amountStr;
        double amount;
        string date;
        int category;

        cout << "Enter account number: ";
        cin >> accountNumber;

        if (!isAccountExists(accountNumber)) {
            cout << "Account does not exist." << endl;
            return;
        }

        cout << "Enter withdrawal amount: ";
        cin >> amountStr;

        if (!isNumber(amountStr)) {
            cout << "Invalid amount. Please enter only numbers." << endl;
            return;
        }

        amount = stod(amountStr);

        cout << "Enter date (dd:mm:yyyy): ";
        cin >> date;

        if (!isValidDate(date)) {
            cout << "Invalid date. Please enter a valid date." << endl;
            return;
        }

        cout << "Select category: " << endl;
        cout << "1. Food" << endl;
        cout << "2. Clothing" << endl;
        cout << "3. Transport" << endl;
        cout << "4. Housing" << endl;
        cout << "5. Taxes" << endl;
        cout << "Enter category number: ";
        cin >> category;

        string categoryName;
        switch (category) {
        case 1:
            categoryName = "Food";
            break;
        case 2:
            categoryName = "Clothing";
            break;
        case 3:
            categoryName = "Transport";
            break;
        case 4:
            categoryName = "Housing";
            break;
        case 5:
            categoryName = "Taxes";
            break;
        default:
            cout << "Invalid category number." << endl;
            return;
        }

        // Сохраняем затраты в файл с номером счета
        ofstream file(accountNumber + ".txt", ios::app);
        if (file) {
            file << "Withdrawal: " << amount << " Date: " << date << " Category: " << categoryName << endl;
            cout << "Withdrawal added successfully." << endl;
        }
        else {
            cout << "Failed to add withdrawal." << endl;
        }
    }

    // Метод для отображения информации о счете
    void displayAccountInfo() {
        string accountNumber;
        cout << "Enter account number: ";
        cin >> accountNumber;

        if (!isAccountExists(accountNumber)) {
            cout << "Account does not exist." << endl;
            return;
        }

        ifstream file(accountNumber + ".txt");
        string line;
        double balance = 0.0;

        if (file) {
            while (getline(file, line)) {
                stringstream ss(line);
                string type;
                double amount;
                ss >> type >> amount;
                if (type == "Deposit:") {
                    balance += amount;
                }
                else if (type == "Withdrawal:") {
                    balance -= amount;
                }
            }
            cout << "Account balance: " << balance << endl;
        }
        else {
            cout << "Failed to retrieve account information." << endl;
        }
    }

    // Метод для отображения истории транзакций за период
    void displayTransactionHistory() {
        string accountNumber;
        string startDate;
        string endDate;

        cout << "Enter account number: ";
        cin >> accountNumber;

        if (!isAccountExists(accountNumber)) {
            cout << "Account does not exist." << endl;
            return;
        }

        cout << "Enter start date (dd:mm:yyyy): ";
        cin >> startDate;

        if (!isValidDate(startDate)) {
            cout << "Invalid start date. Please enter a valid date." << endl;
            return;
        }

        cout << "Enter end date (dd:mm:yyyy): ";
        cin >> endDate;

        if (!isValidDate(endDate)) {
            cout << "Invalid end date. Please enter a valid date." << endl;
            return;
        }

        ifstream file(accountNumber + ".txt");
        string line;

        if (file) {
            cout << "Transaction history for account " << accountNumber << ":" << endl;
            while (getline(file, line)) {
                stringstream ss(line);
                string type, date;
                ss >> type >> date;
                if (isDateInRange(date, startDate, endDate)) {
                    cout << line << endl;
                }
            }
        }
        else {
            cout << "Failed to retrieve transaction history." << endl;
        }
    }

    // Метод для отображения трех самых крупных затрат за период
    void displayTopExpenses() {
        string accountNumber;
        string startDate;
        string endDate;

        cout << "Enter account number: ";
        cin >> accountNumber;

        if (!isAccountExists(accountNumber)) {
            cout << "Account does not exist." << endl;
            return;
        }

        cout << "Enter start date (dd:mm:yyyy): ";
        cin >> startDate;

        if (!isValidDate(startDate)) {
            cout << "Invalid start date. Please enter a valid date." << endl;
            return;
        }

        cout << "Enter end date (dd:mm:yyyy): ";
        cin >> endDate;

        if (!isValidDate(endDate)) {
            cout << "Invalid end date. Please enter a valid date." << endl;
            return;
        }

        ifstream file(accountNumber + ".txt");
        string line;
        vector<pair<string, double>> expenses;

        if (file) {
            while (getline(file, line)) {
                stringstream ss(line);
                string type;
                double amount;
                ss >> type >> amount;
                if (type == "Withdrawal:" && isDateInRange(line.substr(line.find("Date:") + 6), startDate, endDate)) {
                    expenses.push_back(make_pair(line, amount));
                }
            }

            sort(expenses.begin(), expenses.end(), [](const pair<string, double>& a, const pair<string, double>& b) {
                return abs(a.second) > abs(b.second);
                });

            int count = min(3, static_cast<int>(expenses.size()));
            cout << "Top 3 expenses for account " << accountNumber << ":" << endl;
            for (int i = 0; i < count; i++) {
                cout << expenses[i].first << endl;
            }
        }
        else {
            cout << "Failed to retrieve expenses." << endl;
        }
    }

    // Метод для отображения трех категорий с наибольшими затратами
    void displayTopCategories() {
        string accountNumber;
        string startDate;
        string endDate;

        cout << "Enter account number: ";
        cin >> accountNumber;

        if (!isAccountExists(accountNumber)) {
            cout << "Account does not exist." << endl;
            return;
        }

        cout << "Enter start date (dd:mm:yyyy): ";
        cin >> startDate;

        if (!isValidDate(startDate)) {
            cout << "Invalid start date. Please enter a valid date." << endl;
            return;
        }

        cout << "Enter end date (dd:mm:yyyy): ";
        cin >> endDate;

        if (!isValidDate(endDate)) {
            cout << "Invalid end date. Please enter a valid date." << endl;
            return;
        }

        ifstream file(accountNumber + ".txt");
        string line;
        unordered_map<string, double> categoryExpenses;

        if (file) {
            while (getline(file, line)) {
                stringstream ss(line);
                string type, amountStr, category, date;
                double amount;
                ss >> type >> amountStr;
                if (type == "Withdrawal:" && isDateInRange(line.substr(line.find("Date:") + 6), startDate, endDate)) {
                    ss.ignore(numeric_limits<streamsize>::max(), ':');  // Пропуск ":"
                    ss >> date;
                    ss.ignore(numeric_limits<streamsize>::max(), ':');  // Пропуск ":"
                    ss >> category;
                    amount = stod(amountStr);
                    categoryExpenses[category] += abs(amount);
                }
            }

            vector<pair<string, double>> sortedCategories(categoryExpenses.begin(), categoryExpenses.end());
            sort(sortedCategories.begin(), sortedCategories.end(), [](const pair<string, double>& a, const pair<string, double>& b) {
                return a.second > b.second;
                });

            int count = min(3, static_cast<int>(sortedCategories.size()));
            cout << "Top 3 categories for account " << accountNumber << ":" << endl;
            for (int i = 0; i < count; i++) {
                cout << "Category: " << sortedCategories[i].first << " Amount: " << sortedCategories[i].second << endl;
            }
        }
        else {
            cout << "Failed to retrieve categories." << endl;
        }
    }




    // Вспомогательная функция для проверки, является ли строка числом
    bool isNumber(const string& str) {
        for (char c : str) {
            if (!isdigit(c)) {
                return false;
            }
        }
        return true;
    }

    // Вспомогательная функция для проверки существования файла с номером счета
    bool isAccountExists(const string& accountNumber) {
        ifstream file(accountNumber + ".txt");
        return file.good();
    }

    // Вспомогательная функция для проверки даты в формате dd:mm:yyyy
    bool isValidDate(const string& date) {
        struct tm tm;
        stringstream ss(date);
        ss >> get_time(&tm, "%d:%m:%Y");
        return !ss.fail();
    }

    // Вспомогательная функция для проверки, находится ли дата в заданном диапазоне
    bool isDateInRange(const string& date, const string& startDate, const string& endDate) {
        struct tm date_tm;
        struct tm start_tm;
        struct tm end_tm;

        stringstream ss_date(date);
        stringstream ss_start(startDate);
        stringstream ss_end(endDate);

        ss_date >> get_time(&date_tm, "%d:%m:%Y");
        ss_start >> get_time(&start_tm, "%d:%m:%Y");
        ss_end >> get_time(&end_tm, "%d:%m:%Y");

        time_t date_time = mktime(&date_tm);
        time_t start_time = mktime(&start_tm);
        time_t end_time = mktime(&end_tm);

        return (date_time >= start_time) && (date_time <= end_time);
    }
};

int main() {
    FinanceManager manager;

    cout << "Welcome to the Finance Manager!" << endl;

    while (true) {
        cout << "Please select an option:" << endl;
        cout << "1. Add an account" << endl;
        cout << "2. Make a deposit" << endl;
        cout << "3. Make a withdrawal" << endl;
        cout << "4. Display account information" << endl;
        cout << "5. Display transaction history" << endl;
        cout << "6. Display top expenses" << endl;
        cout << "7. Display top categories" << endl;
        cout << "8. Exit" << endl;
        cout << "Enter option number: ";

        int option;
        cin >> option;

        switch (option) {
        case 1:
            manager.addAccount();
            break;
        case 2:
            manager.deposit();
            break;
        case 3:
            manager.withdraw();
            break;
        case 4:
            manager.displayAccountInfo();
            break;
        case 5:
            manager.displayTransactionHistory();
            break;
        case 6:
            manager.displayTopExpenses();
            break;
        case 7:
            manager.displayTopCategories();
            break;
        case 8:
            cout << "Thank you for using the Finance Manager. Goodbye!" << endl;
            return 0;
        default:
            cout << "Invalid option. Please enter a valid option number." << endl;
            break;
        }

        cout << endl;
    }

    return 0;
}
