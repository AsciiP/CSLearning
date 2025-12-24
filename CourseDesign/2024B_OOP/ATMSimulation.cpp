#include <iostream>
#include <fstream>
#include <sstream>

#include <iomanip>

#include <ctime>

#include <openssl/sha.h>

std::string generateSalt(int length = 16)
{
    const char charset[] = "0123456789ABCDEF";
    std::string salt;
    srand(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i < length; i++)
    {
        salt += charset[rand() % (sizeof(charset) - 1)];
    }
    return salt;
}

std::string sha256(const std::string &input) // SHA256 from OpenSSL
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char *>(input.c_str()), input.size(), hash);

    std::ostringstream oss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
    {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return oss.str();
}

class User
{
private:
    std::string hashSalt;
    std::string passwordHash;

public:
    long long int balance = 0LL; // Currency in cents
    std::string cardNo;

public:
    User(const std::string &__cardNo, long long int _balance = 0LL, const std::string &__passwordHashSalt = "", const std::string &__passwordHash = "")
        : cardNo(__cardNo), balance(_balance), hashSalt(__passwordHashSalt), passwordHash(__passwordHash) {};

    std::string toCSV() const
    {
        return cardNo + "," + std::to_string(balance) + "," + hashSalt + "," + passwordHash;
    }

    bool verifyPassword(const std::string &password) const
    {
        return passwordHash == sha256(password + std::to_string(balance) + hashSalt);
    }

    bool updateHash(const long long int &newBalance, const std::string &newPassword)
    {
        hashSalt = generateSalt();
        passwordHash = sha256(newPassword + std::to_string(newBalance) + hashSalt);
        return true;
    }

    ~User() = default;
};

class ATM
{
private:
    std::string userDataFilePath;
    std::string adminHashSalt = "";
    std::string adminPasswordHash = "";

public:
    User *currentUser = nullptr;
    std::string weakPasswordsFilePath = "weakpasswords.txt";

public:
    ATM(const std::string &file) : userDataFilePath(file) {};

    bool isWeakPassword(const std::string &password)
    {
        if (password.empty())
        {
            std::cout << "Password cannot be empty." << std::endl;
            return true;
        }

        if (password.length() < 6)
        {
            std::cout << "Password must be at least 6 characters long." << std::endl;
            return true;
        }

        bool allSame = true;
        for (size_t i = 1; i < password.size(); ++i)
        {
            if (password[i] != password[0])
            {
                allSame = false;
                break;
            }
        }
        if (allSame)
        {
            std::cout << "Password cannot be all the same digit." << std::endl;
            return true;
        }

        std::ifstream infile(weakPasswordsFilePath);
        if (!infile.is_open())
        {
            std::cerr << "Error opening weak passwords file." << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(infile, line))
        {
            if (line == password)
            {
                infile.close();
                std::cout << "Common password detected." << std::endl;
                return true;
            }
        }
        infile.close();
        return false;
    }

    void setAdminPassword(const std::string &adminPassword)
    {
        adminHashSalt = generateSalt();
        adminPasswordHash = sha256(adminPassword + adminHashSalt);
        std::cout << "Admin password set successfully." << std::endl;
    }

    void adminRegisterUser(const std::string &cardNo, const std::string &password)
    {
        std::ifstream infile(userDataFilePath, std::ios::app);
        if (!infile.is_open())
        {
            std::cerr << "Error opening user data file." << std::endl;
            return;
        }

        // Check if cardNo already exists
        std::string line;
        while (std::getline(infile, line))
        {
            std::istringstream iss(line);
            std::string matchedCardNo;
            if (std::getline(iss, matchedCardNo, ','))
            {
                if (matchedCardNo == cardNo)
                {
                    std::cout << "Card number already exists." << std::endl;
                    infile.close();
                    return;
                }
            }
        }

        User *newUser = new User(cardNo);
        newUser->updateHash(newUser->balance, password);
        std::ofstream file(userDataFilePath, std::ios::app);
        file << newUser->toCSV() << std::endl;
        delete newUser;
        file.close();
        std::cout << "User registered successfully." << std::endl;
    }

    void adminDeleteUser(const std::string &cardNo)
    {
        std::ifstream file(userDataFilePath);
        if (!file.is_open())
        {
            std::cerr << "Error opening user data file." << std::endl;
            return;
        }

        std::string line;
        std::ostringstream buffer;
        bool found = false;

        while (std::getline(file, line))
        {
            if (line.find(cardNo + ",") == 0)
            {
                std::istringstream iss(line);
                std::string matchedCardNo, balance, salt, hash;
                if (std::getline(iss, matchedCardNo, ',') &&
                    std::getline(iss, balance, ',') &&
                    std::getline(iss, salt, ',') &&
                    std::getline(iss, hash))
                {
                    if (balance != "0")
                    {
                        std::cerr << "Cannot delete user with non-zero balance." << std::endl;
                        file.close();
                        return;
                    }
                    found = true;
                }
                else
                {
                    std::cerr << "Error parsing user data." << std::endl;
                }
            }
            else
            {
                buffer << line << std::endl;
            }
        }
        file.close();

        if (found)
        {
            std::ofstream outFile(userDataFilePath);
            outFile << buffer.str();
            outFile.close();
            std::cout << "User deleted successfully." << std::endl;
        }
        else
        {
            std::cerr << "User not found." << std::endl;
        }
    }

    void adminChangePassword(const std::string &cardNo, const std::string &newPassword)
    {
        std::ifstream file(userDataFilePath);
        if (!file.is_open())
        {
            std::cerr << "Error opening user data file." << std::endl;
            return;
        }

        std::string line;
        std::ostringstream buffer;
        bool found = false;

        while (std::getline(file, line))
        {
            if (line.find(cardNo + ",") == 0)
            {
                std::istringstream iss(line);
                std::string matchedCardNo, balance, salt, hash;
                if (std::getline(iss, matchedCardNo, ',') &&
                    std::getline(iss, balance, ',') &&
                    std::getline(iss, salt, ',') &&
                    std::getline(iss, hash))
                {
                    User *changedUser = new User(matchedCardNo, std::stoll(balance), salt);
                    changedUser->updateHash(std::stoll(balance), newPassword);
                    buffer << changedUser->toCSV() << std::endl;
                    std::cout << "Password changed successfully." << std::endl;
                    delete changedUser;
                    found = true;
                }
                else
                {
                    std::cerr << "Error parsing user data." << std::endl;
                }
            }
            else
            {
                buffer << line << std::endl;
            }
        }
        file.close();

        if (found)
        {
            std::ofstream outFile(userDataFilePath);
            outFile << buffer.str();
            outFile.close();
        }
        else
        {
            std::cerr << "User not found." << std::endl;
        }
    }

    bool verifyAdminPassword(const std::string &password)
    {
        return adminPasswordHash == sha256(password + adminHashSalt);
    }

    bool userLogin(const std::string &cardNo, const std::string &password)
    {
        std::ifstream file(userDataFilePath);
        if (!file.is_open())
        {
            std::cerr << "Error opening user data file." << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(file, line))
        {
            if (line.find(cardNo + ",") == 0)
            {
                std::istringstream iss(line);
                std::string matchedCardNo, salt, hash, balance;
                if (std::getline(iss, matchedCardNo, ',') &&
                    std::getline(iss, balance, ',') &&
                    std::getline(iss, salt, ',') &&
                    std::getline(iss, hash))
                {
                    if (matchedCardNo == cardNo && hash == sha256(password + balance + salt))
                    {
                        currentUser = new User(cardNo, std::stoll(balance), salt, hash);
                        file.close();
                        return true;
                    }
                    else
                    {
                        std::cerr << "Incorrect password." << std::endl;
                        return false;
                    }
                }
            }
        }
        file.close();
        std::cerr << "User not found." << std::endl;
        return false;
    }

    void userDeposit(const std::string &amountStr, const std::string &password)
    {
        if (currentUser)
        {
            long long int amount = 0LL;
            try
            {
                size_t dotPos = amountStr.find('.');
                if (dotPos != std::string::npos)
                {
                    if (amountStr.size() - dotPos - 1 > 2)
                    {
                        std::cerr << "Deposit amount can have at most two decimal places." << std::endl;
                        return;
                    }
                }
                double amountDouble = std::stod(amountStr);
                amount = static_cast<long long int>(amountDouble * 100);
                if (amount <= 0LL)
                {
                    std::cerr << "Deposit amount must be greater than zero." << std::endl;
                    return;
                }
                else if (amount <= 0LL)
                {
                    std::cerr << "Invalid deposit amount." << std::endl;
                    return;
                }
            }
            catch (const std::invalid_argument &)
            {
                std::cerr << "Invalid deposit amount." << std::endl;
                return;
            }
            catch (const std::out_of_range &)
            {
                std::cerr << "Deposit amount is too large." << std::endl;
                return;
            }

            currentUser->balance += amount;
            currentUser->updateHash(currentUser->balance, password);

            std::ifstream file(userDataFilePath, std::ios::app);
            if (!file.is_open())
            {
                std::cerr << "Error opening user data file." << std::endl;
                return;
            }

            std::string line;
            std::ostringstream buffer;
            bool found = false;

            while (std::getline(file, line))
            {
                if (line.find(this->currentUser->cardNo + ",") == 0)
                {
                    std::istringstream iss(line);
                    std::string matchedCardNo, balance, salt, hash;
                    if (std::getline(iss, matchedCardNo, ',') &&
                        std::getline(iss, balance, ',') &&
                        std::getline(iss, salt, ',') &&
                        std::getline(iss, hash))
                    {
                        buffer << currentUser->toCSV() << std::endl;
                        found = true;
                    }
                    else
                    {
                        std::cerr << "Error parsing user data." << std::endl;
                    }
                }
                else
                {
                    buffer << line << std::endl;
                }
            }

            if (found)
            {
                std::ofstream outFile(userDataFilePath);
                outFile << buffer.str();
                outFile.close();
            }

            std::cout << "Deposit successful. Your current balance: "
                      << currentUser->balance / 100 << "."
                      << std::setfill('0') << std::setw(2) << currentUser->balance % 100
                      << " CNY" << std::endl;
        }
        else
        {
            std::cerr << "No user logged in." << std::endl;
        }
    }

    void userWithdraw(const std::string &amountStr, const std::string &password)
    {
        if (currentUser)
        {
            long long int amount = 0LL;
            try
            {
                size_t dotPos = amountStr.find('.');
                if (dotPos != std::string::npos)
                {
                    if (amountStr.size() - dotPos - 1 > 2)
                    {
                        std::cerr << "Withdraw amount can have at most two decimal places." << std::endl;
                        return;
                    }
                }
                double amountDouble = std::stod(amountStr);
                amount = static_cast<long long int>(amountDouble * 100);
                if (amount <= 0LL)
                {
                    std::cerr << "Withdraw amount must be greater than zero." << std::endl;
                    return;
                }
                if (amount % 10000 != 0)
                {
                    std::cerr << "Withdraw amount must be a multiple of 100 CNY." << std::endl;
                    return;
                }
                if (currentUser->balance < amount)
                {
                    std::cerr << "Insufficient balance." << std::endl;
                    return;
                }
            }
            catch (const std::invalid_argument &)
            {
                std::cerr << "Invalid withdraw amount." << std::endl;
                return;
            }
            catch (const std::out_of_range &)
            {
                std::cerr << "Withdraw amount is too large." << std::endl;
                return;
            }

            currentUser->balance -= amount;
            currentUser->updateHash(currentUser->balance, password);

            std::ifstream file(userDataFilePath, std::ios::app);
            if (!file.is_open())
            {
                std::cerr << "Error opening user data file." << std::endl;
                return;
            }

            std::string line;
            std::ostringstream buffer;
            bool found = false;

            while (std::getline(file, line))
            {
                if (line.find(this->currentUser->cardNo + ",") == 0)
                {
                    std::istringstream iss(line);
                    std::string matchedCardNo, balance, salt, hash;
                    if (std::getline(iss, matchedCardNo, ',') &&
                        std::getline(iss, balance, ',') &&
                        std::getline(iss, salt, ',') &&
                        std::getline(iss, hash))
                    {
                        buffer << currentUser->toCSV() << std::endl;
                        found = true;
                    }
                    else
                    {
                        std::cerr << "Error parsing user data." << std::endl;
                    }
                }
                else
                {
                    buffer << line << std::endl;
                }
            }

            if (found)
            {
                std::ofstream outFile(userDataFilePath);
                outFile << buffer.str();
                outFile.close();
            }

            std::cout << "Withdrawal successful. Your current balance: "
                      << currentUser->balance / 100 << "."
                      << std::setfill('0') << std::setw(2) << currentUser->balance % 100
                      << " CNY" << std::endl;
        }
        else
        {
            std::cerr << "No user logged in." << std::endl;
        }
    }

    void userCheckBalance()
    {
        if (currentUser)
        {
            std::cout << "Your current balance: "
                      << currentUser->balance / 100 << "."
                      << std::setfill('0') << std::setw(2) << currentUser->balance % 100
                      << " CNY" << std::endl;
        }
        else
        {
            std::cerr << "No user logged in." << std::endl;
        }
    }

    void userChangePassword(const std::string &newPassword)
    {
        if (currentUser)
        {

            std::ifstream file(userDataFilePath);
            if (!file.is_open())
            {
                std::cerr << "Error opening user data file." << std::endl;
                return;
            }

            std::string line;
            std::ostringstream buffer;
            bool found = false;

            while (std::getline(file, line))
            {
                if (line.find(currentUser->cardNo + ",") == 0)
                {
                    User *changedUser = new User(currentUser->cardNo, currentUser->balance);
                    changedUser->updateHash(currentUser->balance, newPassword);
                    buffer << changedUser->toCSV() << std::endl;
                    std::cout << "Password changed successfully." << std::endl;
                    delete changedUser;
                    found = true;
                }
                else
                {
                    buffer << line << std::endl;
                }
            }
            file.close();

            if (found)
            {
                std::ofstream outFile(userDataFilePath);
                outFile << buffer.str();
                outFile.close();
            }
            else
            {
                std::cerr << "User not found." << std::endl;
            }
        }
        else
        {
            std::cerr << "No user logged in." << std::endl;
        }
    }

    void userLogout()
    {
        if (currentUser)
        {
            delete currentUser;
            currentUser = nullptr;
        }
    }

    ~ATM()
    {
        if (currentUser)
        {
            delete currentUser;
            currentUser = nullptr;
        }
    }
};

int main()
{
    system("cls");

    ATM atm("userdata.csv");

    std::string adminPassword;
    std::cout << "\nPlease set the admin password: ";
    std::getline(std::cin, adminPassword);
    while (atm.isWeakPassword(adminPassword))
    {
        std::cout << "Please set the admin password: ";
        std::getline(std::cin, adminPassword);
    }

    atm.setAdminPassword(adminPassword);
    system("pause");

    while (true)
    {
        system("cls");
        std::cout << "\nWelcome to the ATM System:\n"
                  << "1. Admin login...\n"
                  << "2. User login...\n"
                  << "0. Shutdown\n";
        std::cout << std::endl;

        std::string choice;
        std::cout << "Please enter your choice: ";
        std::getline(std::cin, choice);

        if (choice == "1")
        {
            std::string adminPassword;
            std::cout << "Please enter admin password: ";
            std::getline(std::cin, adminPassword);

            if (atm.verifyAdminPassword(adminPassword))
            {
                while (true)
                {
                    system("cls");
                    std::cout << "\nWelcome, administrator.\n"
                              << "1. Register user\n"
                              << "2. Delete user\n"
                              << "3. Change user password\n"
                              << "0. Logout\n";
                    std::cout << std::endl;

                    std::string adminChoice;
                    std::cout << "Please enter your choice: ";
                    std::getline(std::cin, adminChoice);

                    if (adminChoice == "1")
                    {
                        std::string cardNo, password;
                        std::cout << "Enter card number: ";
                        std::getline(std::cin, cardNo);
                        std::cout << "Enter password: ";
                        std::getline(std::cin, password);

                        if (atm.isWeakPassword(password))
                        {
                            std::cout << "This is a weak password. Still want to register? (Y/N): ";
                            std::string confirm;
                            std::getline(std::cin, confirm);
                            if (confirm == "N" || confirm == "n")
                            {
                                std::cout << "Registration cancelled.\n";
                            }
                            else if (confirm == "Y" || confirm == "y")
                            {
                                atm.adminRegisterUser(cardNo, password);
                            }
                            else
                            {
                                std::cout << "Invalid input. Please enter Y or N.\n";
                            }
                        }
                        else
                        {
                            atm.adminRegisterUser(cardNo, password);
                        }
                    }
                    else if (adminChoice == "2")
                    {
                        std::string cardNo;
                        std::cout << "Enter card number to delete: ";
                        std::getline(std::cin, cardNo);
                        atm.adminDeleteUser(cardNo);
                    }
                    else if (adminChoice == "3")
                    {
                        std::string cardNo, newPassword;
                        std::cout << "Enter card number: ";
                        std::getline(std::cin, cardNo);
                        std::cout << "Enter new password: ";
                        std::getline(std::cin, newPassword);

                        if (atm.isWeakPassword(newPassword))
                        {
                            std::cout << "This is a weak password. Still want to change? (Y/N): ";
                            std::string confirm;
                            std::getline(std::cin, confirm);
                            if (confirm == "N" || confirm == "n")
                            {
                                std::cout << "Password change cancelled.\n";
                                continue;
                            }
                            else if (confirm == "Y" || confirm == "y")
                            {
                                atm.adminChangePassword(cardNo, newPassword);
                            }
                            else
                            {
                                std::cout << "Invalid input. Please enter Y or N.\n";
                                continue;
                            }
                        }
                        else
                        {
                            atm.adminChangePassword(cardNo, newPassword);
                        }
                    }
                    else if (adminChoice == "0")
                    {
                        break;
                    }
                    else
                    {
                        std::cout << "\nInvalid choice. Please try again.\n";
                    }

                    system("pause");
                }
            }
            else
            {
                std::cout << "Admin login failed.\n";
            }
        }
        else if (choice == "2")
        {
            std::string cardNo, password;
            std::cout << "Enter card number: ";
            std::getline(std::cin, cardNo);
            std::cout << "Enter password: ";
            std::getline(std::cin, password);

            if (atm.userLogin(cardNo, password))
            {
                std::string maskedCardNo;
                if (cardNo.length() >= 2)
                {
                    maskedCardNo = cardNo.front() + std::string("**") + cardNo.back();
                }
                else
                {
                    maskedCardNo = "**";
                }

                while (true)
                {
                    system("cls");
                    std::cout << "\nWelcome, " << maskedCardNo << ".\n"
                              << "1. Deposit\n"
                              << "2. Withdraw\n"
                              << "3. Change password\n"
                              << "4. Check balance\n"
                              << "0. Logout\n";
                    std::cout << std::endl;

                    if (atm.isWeakPassword(password))
                    {
                        std::cout << "Your password is weak. Please consider changing it.\n\n";
                    }

                    std::string userChoice;
                    std::cout << "Please enter your choice: ";
                    std::getline(std::cin, userChoice);

                    if (userChoice == "1")
                    {
                        std::string amountStr;
                        std::cout << "Enter deposit amount (CNY): ";
                        std::getline(std::cin, amountStr);
                        atm.userDeposit(amountStr, password);
                    }
                    else if (userChoice == "2")
                    {
                        std::string amountStr;
                        std::cout << "Enter withdraw amount (CNY, must be a multiple of 100): ";
                        std::getline(std::cin, amountStr);
                        atm.userWithdraw(amountStr, password);
                    }
                    else if (userChoice == "3")
                    {
                        std::string newPassword;
                        std::cout << "Enter new password: ";
                        std::getline(std::cin, newPassword);

                        while (newPassword == password)
                        {
                            std::cout << "New password cannot be the same as the old one.\n"
                                      << "Enter new password: ";
                            std::getline(std::cin, newPassword);
                        }
                        while (atm.isWeakPassword(newPassword))
                        {
                            std::cout << "This is a weak password. Please enter a stronger password: ";
                            std::getline(std::cin, newPassword);
                        }

                        std::string confirmPassword;
                        std::cout << "Confirm new password: ";
                        std::getline(std::cin, confirmPassword);
                        if (newPassword == confirmPassword)
                        {
                            atm.userChangePassword(newPassword);
                            password = newPassword;
                        }
                        else
                        {
                            std::cout << "Passwords do not match.\n";
                        }
                    }
                    else if (userChoice == "4")
                    {
                        atm.userCheckBalance();
                    }
                    else if (userChoice == "0")
                    {
                        atm.userLogout();
                        break;
                    }
                    else
                    {
                        std::cout << "Invalid choice. Please try again.\n";
                    }

                    system("pause");
                }
            }
            else
            {
                std::cout << "User login failed.\n";
            }
        }
        else if (choice == "0")
        {
            break;
        }
        else
        {
            std::cout << "Invalid choice. Please try again.\n";
        }

        system("pause");
    }

    return 0;
}