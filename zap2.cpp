#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

class User {
public:
    std::string _name;
    std::string _login;
    std::string _pass;

    User(const std::string& name, const std::string& login, const std::string& pass)
        : _name(name), _login(login), _pass(pass) {}

    friend std::ostream& operator<<(std::ostream& out, const User& user) {
        out << "User: " << user._name << ", Login: " << user._login << ", Password: " << user._pass;
        return out;
    }
};

class Message {
public:
    std::string _text;
    std::string _sender;
    std::string _receiver;

    Message(const std::string& text, const std::string& sender, const std::string& receiver)
        : _text(text), _sender(sender), _receiver(receiver) {}

    friend std::ostream& operator<<(std::ostream& out, const Message& message) {
        out << "Message: " << message._text << ", Sender: " << message._sender << ", Receiver: " << message._receiver;
        return out;
    }
};

void setFilePermissions(const std::string& filename) {
    fs::perms new_perms = fs::perms::owner_read | fs::perms::owner_write;
    fs::permissions(filename, new_perms);
}

void processFile(const std::string& inputFilename, const std::string& outputFilename) {
    std::ifstream infile(inputFilename);
    if (!infile) {
        std::cerr << "Unable to open input file!" << std::endl;
        return;
    }

    std::ofstream outfile(outputFilename);
    if (!outfile) {
        std::cerr << "Unable to open output file!" << std::endl;
        return;
    }

    std::string line;
    outfile << "Users:\n";
    while (std::getline(infile, line)) {
        if (line.find("User {") != std::string::npos) {
            std::istringstream iss(line);
            std::string name, login, pass;
            iss >> name >> login >> pass;
            User user(name, login, pass);
            outfile << user << "\n";
        } else if (line.find("Message {") != std::string::npos) {
            std::istringstream iss(line);
            std::string text, sender, receiver;
            iss >> text >> sender >> receiver;
            Message message(text, sender, receiver);
            outfile << message << "\n";
        }
    }

    infile.close();
    outfile.close();
}

int main() {
    setFilePermissions("input.txt");

    processFile("input.txt", "output.txt");

    setFilePermissions("output.txt");

    return 0;
}
