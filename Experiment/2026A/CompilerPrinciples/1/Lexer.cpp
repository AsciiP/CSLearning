#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <map>

using namespace std;

const map<string, int> keywordTable = {
    {"begin", 1}, {"if", 2}, {"then", 3},
    {"while", 4}, {"do", 5}, {"end", 6}
};

struct Token {
    int    syn;
    string value;
};

class Lexer {
private:
    string source;
    int    pos;
    int    line;

    char peek() const {
        return (pos < (int)source.size()) ? source[pos] : '\0';
    }

    char advance() {
        if (pos < (int)source.size()) {
            char ch = source[pos++];
            if (ch == '\n') line++;
            return ch;
        }
        return '\0';
    }

    char lookAhead() const {
        return (pos + 1 < (int)source.size()) ? source[pos + 1] : '\0';
    }

    void skipWhitespace() {
        while (pos < (int)source.size() && isspace((unsigned char)source[pos])) {
            if (source[pos] == '\n') line++;
            pos++;
        }
    }

    // 
    bool skipComment() {
        if (peek() == '/' && lookAhead() == '*') {
            advance(); advance();
            while (pos < (int)source.size()) {
                if (peek() == '*' && lookAhead() == '/') {
                    advance(); advance();
                    return true;
                }
                advance();
            }
            cerr << "[Error] Line " << line << " : Comment not closed!" << endl;
            return true;
        }
        return false;
    }

    Token readIdentifierOrKeyword() {
        string word;
        while (pos < (int)source.size() &&
            (isalpha((unsigned char)source[pos]) || isdigit((unsigned char)source[pos]))) {
            word += advance();
        }
        auto it = keywordTable.find(word);
        if (it != keywordTable.end()) return { it->second, word };
        return { 10, word };
    }

    Token readNumber() {
        string num;
        while (pos < (int)source.size() && isdigit((unsigned char)source[pos])) {
            num += advance();
        }
        return { 11, num };
    }

    Token readOperatorOrDelimiter() {
        char ch = advance();
        switch (ch) {
        case '+': return { 13, "+" };
        case '-': return { 14, "-" };
        case '*': return { 15, "*" };
        case '/': return { 16, "/" };
        case '=': return { 25, "=" };
        case ';': return { 26, ";" };
        case '(': return { 27, "(" };
        case ')': return { 28, ")" };
        case '#': return { 0,  "#" };
        case ':':
            if (peek() == '=') { advance(); return { 18, ":=" }; }
            return { 17, ":" };
        case '<':
            if (peek() == '>') { advance(); return { 21, "<>" }; }
            if (peek() == '=') { advance(); return { 22, "<=" }; }
            return { 20, "<" };
        case '>':
            if (peek() == '=') { advance(); return { 24, ">=" }; }
            return { 23, ">" };
        default:
            cerr << "[Error] Line " << line << " : Invalid character '" << ch << "'" << endl;
            return { -1, string(1, ch) };
        }
    }

public:
    explicit Lexer(const string& src) : source(src), pos(0), line(1) {}

    Token nextToken() {
        while (pos < (int)source.size()) {
            skipWhitespace();
            if (skipComment()) continue;
            if (pos >= (int)source.size()) break;

            char ch = peek();
            if (isalpha((unsigned char)ch)) return readIdentifierOrKeyword();
            if (isdigit((unsigned char)ch)) return readNumber();
            return readOperatorOrDelimiter();
        }
        return { 0, "#" };
    }
};

void printToken(const Token& tok) {
    if (tok.syn == 10)
        cout << "(" << tok.syn << ",'" << tok.value << "')";
    else if (tok.syn == 11)
        cout << "(" << tok.syn << "," << tok.value << ")";
    else
        cout << "(" << tok.syn << "," << tok.value << ")";
}

int main(int argc, char* argv[]) {
    string filename;

    if (argc >= 2) {
        filename = argv[1];
    }
    else {
        cout << "[Info] Source file path: ";
        getline(cin, filename);
    }

    ifstream fin(filename);
    if (!fin.is_open()) {
        cerr << "[Error] Cannot open file: " << filename << endl;
        return 1;
    }

    string source, line;
    while (getline(fin, line)) {
        source += line + "\n";
    }
    fin.close();

    Lexer lexer(source);
    Token tok;

    do {
        tok = lexer.nextToken();
        if (tok.syn == -1) continue;
        printToken(tok);
    } while (tok.syn != 0);

    return 0;
}