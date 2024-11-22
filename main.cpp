#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <cctype>

#define HT_SIZE 26
#define HT_BASE 11

using namespace std;

typedef unsigned long long ull;
typedef char letter;
typedef string symbol;

class HashTable {
private:
    list<pair<symbol, letter>> array[HT_SIZE];

    ull fastExp(ull base, ull expoent, ull mod) {
        ull ans = 1;
        while (expoent) {
            if (expoent & 1)
                ans = ((ans % mod) * (base % mod)) % mod;
            base = ((base % mod) * (base % mod)) % mod;
            expoent >>= 1;
        }
        return ans % mod;
    }

    ull hash(string value) {
        ull hash = 0;
        ull keySize = value.size();
        for (ull i = 0; i < keySize; i++)
            hash = (hash + value[i] * (fastExp(HT_BASE, keySize - i - 1, HT_SIZE))) % HT_SIZE;
        return hash % HT_SIZE;
    }

public:
    void insert(const string& symbol, char letter) {
        int index = hash(symbol);
        array[index].push_back({symbol, letter});
    }

    char getLetterBySymbol(const string& symbol) {
        int index = hash(symbol);
        for (auto& pair : array[index]) {
            if (pair.first == symbol) {
                return pair.second;
            }
        }
        return '\0';
    }

    void removeBySymbol(const string& symbol) {
        int index = hash(symbol);
        for (auto it = array[index].begin(); it != array[index].end(); ++it) {
            if (it->first == symbol) {
                array[index].erase(it);
                return;
            }
        }
    }

    void show() {
        cout << "[==== HASH TABLE ====]" << endl;
        for (int i = 0; i < HT_SIZE; ++i) {
            cout << i << "| ";
            for (auto& pair : array[i]) {
                cout << "(" << pair.first << ", " << pair.second << ") ";
            }
            if (array[i].empty())
                cout << "(NULL)";
            cout << endl;
        }
        cout << "[=====================]" << endl;
    }
};

vector<int> computeSkipVector(string &P)
{
  int M = P.size();
  vector<int> R;
  R.resize(256, -1);
  for (int i = 0; i < M; i++)
    R[P[i]] = i;
  return R;
}
// problema aqui
string boyerMoore(string &T, string &P)
{
  string segmentation;
  int N = T.size(), M = P.size(), skip = 0;
  for (int i = 0; i <= N - M; i += skip)
  {
    skip = 0;
    for (int j = M - 1; j >= 0; j--)
    {
      if (P[j] != T[i + j])
      {
        vector<int> R = computeSkipVector(P);
        skip = j - R[T[i + j]];
        if (skip < 1)
          skip = 1;
        break;
      }
    }
    segmentation += to_string(skip);
    segmentation.push_back(' ');
    if (skip == 0)
    {
      segmentation.push_back('(');
      segmentation += to_string(i);
      segmentation.push_back(')');
      segmentation.push_back(' ');
      skip = 1;
    }
  }
  return segmentation;
}

string decoder(string &T, int k)
{
  string decoded_text;
  for (char c : T)
  {
    if (c != ' ' && c != '.')
      decoded_text += char((c - 'A' - k + 26) % 26 + 'A');
    else
      decoded_text += c;
  }
  return decoded_text;
}

list<string> cesarDecoder(string &T, list<string> &words)
{
  string decoded_text, segmentation;
  list<string> decoded_words;
  bool flag = false;
  for (int i = 0; i < 26; ++i)
  { 
    decoded_text = decoder(T, i);
    // problema aqui
    for (string &P : words)
    {
      segmentation = boyerMoore(T, P);
      decoded_words.push_back(segmentation);
      for (auto w : segmentation)
      {
        if (w == '(')
        {
          cout << w << " ";
          flag = true;
          break;
        }
      }
    }
    if (flag)
        return decoded_words;
    else
        decoded_words.clear();
  }
  decoded_words.push_back("-1");
  return decoded_words;
}

int main() {
  // create dict
  HashTable ht;
  vector<pair<symbol, letter>> dict = {
    {":::", 'A'}, {".::", 'B'}, {":.:", 'C'}, {"::.", 'D'},
    {":..", 'E'}, {".:.", 'F'}, {"..:", 'G'}, {"...", 'H'},
    {"|::", 'I'}, {":|:", 'J'}, {"::|", 'K'}, {"|.:", 'L'},
    {".|:", 'M'}, {".:|", 'N'}, {"|:.", 'O'}, {":|.", 'P'},
    {":.|", 'Q'}, {"|..", 'R'}, {".|.", 'S'}, {"..|", 'T'},
    {".||", 'U'}, {"|.|", 'V'}, {"||.", 'W'}, {"-.-", 'X'},
    {".--", 'Y'}, {"--.", 'Z'}, {"---", ' '}, {"~~~", '.'}
  };
  for (auto &pair : dict)
    ht.insert(pair.first, pair.second);
  // store decoded text
  string symbols_text, translated_text;
  while (getline(cin, symbols_text))
  {
    for (int i = 0; i < symbols_text.size(); i += 3) {
      string symbol = symbols_text.substr(i, 3);
      translated_text += ht.getLetterBySymbol(symbol);
    }
    if (translated_text.substr(translated_text.size() - 1) == ".")
      break;
  }
  // store target words
  list<string> target_words;
  string word;
  while (getline(cin, word) && word != "fim")
  {
    string uppercase_word;
    for (auto c : word)
      uppercase_word += toupper(c);
    target_words.push_back(uppercase_word);
  }

  list<string> decoded_text = cesarDecoder(translated_text, target_words);
  auto i = target_words.begin();
  auto j = decoded_text.begin();

  while (i != target_words.end() && j != decoded_text.end())
  {
    cout << *i << ": ";
    cout << *j << endl;
    i++;
    j++;
  }
  return 0;
}