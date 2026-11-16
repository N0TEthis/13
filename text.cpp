#include <iostream>
#include <string>
#include <clocale>
using namespace std;

class Text {
private:
    string text;// весь текст одной строкой

    // является ли символ концом предложения
    static bool isEnd(char c) {
        return c == '.' || c == '!' || c == '?';
    }

    // буква ли символ
    static bool isLetter(char ch) {
        unsigned char c = ch;
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
            return true;
        return c >= 0xC0;
    }

    // симвл слова
    static bool isWordChar(char ch) {
        unsigned char c = ch;
        return isLetter(ch) || (c >= '0' && c <= '9') || c >= 0x80;
    }

    // убрать пробелы по краям и добавить точку, если нет знака конца
    static string prepare(string s) {
        int a = 0;
        while (a < (int)s.length() && s[a] == ' ')
            a++;
        int b = (int)s.length() - 1;
        while (b >= a && s[b] == ' ')
            b--;
        s = s.substr(a, b - a + 1);
        if (s.length() > 0 && !isEnd(s[s.length() - 1]))
            s += '.';
        return s;
    }

    // привести весь текст к норме.... без пробелов в конце, со знаком конца
    void closeText() {
        while (text.length() > 0 && text[text.length() - 1] == ' ')
            text.erase(text.length() - 1, 1);
        if (text.length() > 0 && !isEnd(text[text.length() - 1]))
            text += '.';
    }

    // найти границы предложения с номером num 
    // start - позиция первого символа, end позиция после последнего
    bool findSentence(int num, int& start, int& end) const {
        int len = text.length();
        int pos = 0;
        int count = 0;
        while (pos < len) {
            while (pos < len && text[pos] == ' ') // пропускаем пробелы
                pos++;
            if (pos >= len)
                break;
            int st = pos;
            while (pos < len && !isEnd(text[pos])) // идём до знака конца
                pos++;
            while (pos < len && isEnd(text[pos])) // захватываем знаки (!?!?)
                pos++;
            count++;
            if (count == num) {
                start = st;
                end = pos;
                return true;
            }
        }
        return false;
    }

public:
    Text(string s = "") {
        text = s;
        closeText();
    }

    string getText() const {
        return text;
    }

    // добавить предложение в конец
    bool addSentence(string s) {
        s = prepare(s);
        if (s.length() == 0)
            return false;
        if (text.length() > 0)
            text += ' ';
        text += s;
        return true;
    }

    // вставить предложение перед предложением с номером num
    bool insertSentence(int num, string s) {
        s = prepare(s);
        if (s.length() == 0)
            return false;
        int n = countSentences();
        if (num < 1 || num > n + 1)
            return false;
        if (num == n + 1)
            return addSentence(s);
        int start, end;
        findSentence(num, start, end);
        text.insert(start, s + " ");
        return true;
    }

    // удалить предложение с номером num
    bool removeSentence(int num) {
        int start, end;
        if (!findSentence(num, start, end))
            return false;
        text.erase(start, end - start);
        while (start < (int)text.length() && text[start] == ' ')    // лишние пробелы
            text.erase(start, 1);
        while (text.length() > 0 && text[text.length() - 1] == ' ') // пробелы в конце
            text.erase(text.length() - 1, 1);
        return true;
    }

    // кол-во букв
    int countLetters() const {
        int n = 0;
        for (int i = 0; i < (int)text.length(); i++)
            if (isLetter(text[i]))
                n++;
        return n;
    }

    // кол-во слов 
    int countWords() const {
        int n = 0;
        bool inWord = false;
        for (int i = 0; i < (int)text.length(); i++) {
            if (isWordChar(text[i])) {
                if (!inWord) {
                    n++;
                    inWord = true;
                }
            }
            else {
                inWord = false;
            }
        }
        return n;
    }

    // кол-во предложений
    int countSentences() const {
        int start, end;
        int n = 0;
        while (findSentence(n + 1, start, end))
            n++;
        return n;}

    // предложение с номером num123 
    string getSentence(int num) const {
        int start, end;
        if (!findSentence(num, start, end))
            return "";
        return text.substr(start, end - start);}

    // саааамое длииинное предложееение
    string longestSentence() const {
        int n = countSentences();
        string best = "";
        for (int i = 1; i <= n; i++) {
            string cur = getSentence(i);
            if (i == 1 || cur.length() > best.length())
                best = cur;}
        return best;}

    // смое крткое прдлжение
    string shortestSentence() const {
        int n = countSentences();
        string best = "";
        for (int i = 1; i <= n; i++) {
            string cur = getSentence(i);
            if (i == 1 || cur.length() < best.length())
                best = cur;}
        return best;}

    // есть ли в тексте заданное предложение
    bool contains(string s) const {
        s = prepare(s);
        int n = countSentences();
        for (int i = 1; i <= n; i++)
            if (getSentence(i) == s)
                return true;
        return false;
    }

    // равны ли два текста
    bool isEqual(const Text& other) const {
        int n = countSentences();
        if (n != other.countSentences())
            return false;
        for (int i = 1; i <= n; i++)
            if (getSentence(i) != other.getSentence(i))
                return false;
        return true;}
};

// ввод строки
string readLine(const string& prompt) {
    string s;
    cout << prompt;
    getline(cin, s);
    return s;}

// ввод целого числа с проверкой
int readInt(const string& prompt) {
    int x;
    cout << prompt;
    while (!(cin >> x)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "нужно ввести число: ";}
    cin.ignore(10000, '\n');
    return x;
}

int main() {
    setlocale(LC_ALL, "RU");

    Text t(readLine("Введите текст: "));
    int choice;

    do {
        cout << "\nТекст: " << t.getText() << "\n";
        cout << "1 - добавить предложение\n"
             << "2 - вставить предложение\n"
             << "3 - удалить предложение\n"
             << "4 - количество букв, слов, предложений\n"
             << "5 - предложение по номеру\n"
             << "6 - самое длинное предложение\n"
             << "7 - самое короткое предложение\n"
             << "8 - есть ли в тексте предложение\n"
             << "9 - равны ли два текста\n"
             << "0 - выход\n";
        choice = readInt("ваш выбор: ");
        if (choice == 1) {
            string s = readLine("введите предложение: ");
            if (t.addSentence(s))
                cout << "предложение добавлено.\n";
            else
                cerr << "пустое предложение.\n";
        }
        else if (choice == 2) {
            int num = readInt("перед каким номером вставить: ");
            string s = readLine("введите предложение: ");
            if (t.insertSentence(num, s))
                cout << "предложение вставлено.\n";
            else
                cerr << "нельзя вставить...неверный номер или пустое предложение.\n";
        }
        else if (choice == 3) {
            int num = readInt("номер удаляемого предложения: ");
            if (t.removeSentence(num))
                cout << "предложение удалено.\n";
            else
                cerr << "предложения с таким номером нет.\n";
        }
        else if (choice == 4) {
            cout << "букв: " << t.countLetters() << "\n";
            cout << "слов: " << t.countWords() << "\n";
            cout << "предложений: " << t.countSentences() << "\n";
        }
        else if (choice == 5) {
            int num = readInt("номер предложения: ");
            string s = t.getSentence(num);
            if (s == "")
                cerr << "предложения с таким номером нет.\n";
            else
                cout << "предложение: " << s << "\n";
        }
        else if (choice == 6) {
            if (t.countSentences() == 0)
                cerr << "текст пуст.\n";
            else
                cout << "саааамое длииииинное: " << t.longestSentence() << "\n";
        }
        else if (choice == 7) {
            if (t.countSentences() == 0)
                cerr << "текст пуст.\n";
            else
                cout << "смое крткое: " << t.shortestSentence() << "\n";
        }
        else if (choice == 8) {
            string s = readLine("введите предложение для поиска: ");
            if (t.contains(s))
                cout << "да, такое предложение есть!11!\n";
            else
                cout << "нет, такого предложения нет((9(\n";
        }
        else if (choice == 9) {
            Text t2(readLine("введите второй текст: "));
            if (t.isEqual(t2))
                cout << "тексты равны.\n";
            else
                cout << "тексты не равны.\n";}
        else if (choice == 0) {
            cout << "До встречи 1!!\n";}
        else {
            cerr << "нет такого пункта меню.\n";}
    } while (choice != 0);

    return 0;
}

