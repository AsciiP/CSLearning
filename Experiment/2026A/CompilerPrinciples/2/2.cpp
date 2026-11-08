#include <iostream>
#include <string>
#include <cctype>
#include <map>
#include <vector>

using namespace std;

// ===== 词法分析器（复用原程序） =====

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
        case '#': return { 0, "#" };
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

// ===== 递归下降语法分析 =====

Token currToken;
Lexer* g_lexer = nullptr;
bool hasError = false;

void parseProgram();
void parseStatementList();
void parseStatement();
void parseAssignment();
void parseExpression();
void parseTerm();
void parseFactor();

void match(int expected) {
    if (currToken.syn == expected) {
        currToken = g_lexer->nextToken();
        return;
    }

    cout << "语法错误: 期望 " << expected
        << " 但得到 " << currToken.syn << " (" << currToken.value << ")" << endl;
    hasError = true;
}

// program -> begin statement-list end
void parseProgram() {
    match(1);               // 匹配 begin
    if (hasError) return;
    parseStatementList();
    match(6);               // end
}

// statement-list -> statement { ; statement }
void parseStatementList() {
    parseStatement();
    while (!hasError && currToken.syn == 26) {
        match(26);
        parseStatement();
    }
}

void parseStatement() {
    parseAssignment();
}

// assignment -> ID := expression
void parseAssignment() {
    if (currToken.syn != 10) {
        cout << "语法错误: 赋值语句应以标识符开头" << endl;
        hasError = true;
        return;
    }
    match(10);
    match(18);
    parseExpression();
}

// expression -> term { (+ | -) term }
void parseExpression() {
    parseTerm();
    while (!hasError && (currToken.syn == 13 || currToken.syn == 14)) {
        match(currToken.syn);
        parseTerm();
    }
}

// term -> factor { (* | /) factor }
void parseTerm() {
    parseFactor();
    while (!hasError && (currToken.syn == 15 || currToken.syn == 16)) {
        match(currToken.syn);
        parseFactor();
    }
}

// factor -> ID | NUM | ( expression )
void parseFactor() {
    if (currToken.syn == 10 || currToken.syn == 11) {
        match(currToken.syn);
    }
    else if (currToken.syn == 27) {
        match(27);
        parseExpression();
        match(28);
    }
    else {
        cout << "语法错误: 因子应为标识符、数字或左括号" << endl;
        hasError = true;
    }
}

// ===== 主程序 =====
int main() {
    string source, line;
    cout << "请输入源代码 (以 # 结束, Ctrl+Z 结束输入):" << endl;
    while (getline(cin, line)) {
        source += line + "\n";
    }

    // 词法分析：获取所有token
    Lexer lexer(source);
    vector<Token> tokens;
    Token tok;
    do {
        tok = lexer.nextToken();
        if (tok.syn != -1) tokens.push_back(tok);
    } while (tok.syn != 0);

    cout << "单词串: ";
    for (size_t i = 0; i < tokens.size(); i++) {
        if (i > 0) cout << " ";
        if (tokens[i].syn == 10)
            cout << "(" << tokens[i].syn << ",'" << tokens[i].value << "')";
        else
            cout << "(" << tokens[i].syn << "," << tokens[i].value << ")";
    }
    cout << endl;

    Lexer lexer2(source);
    g_lexer = &lexer2;
    currToken = lexer2.nextToken();

    parseProgram();

    if (!hasError && currToken.syn == 0) {
        cout << "结果：success" << endl;
    }
    else {
        cout << "结果：error" << endl;
    }

    return 0;
}
