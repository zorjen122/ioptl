#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

class En_Count
{
public:

    En_Count() = default;
    string filter_punctuation(string s)
    {
        const char* table {",.?!:"};
        auto start { s.find_first_not_of(table) };
        auto end { s.find_last_not_of(table) };

        return s.substr(start, end - start + 1);
    }

    void parse()
    {
        string ipt;

        while(cin >> ipt)
        {
            auto en {filter_punctuation(ipt)};
            ++mf[en];

            max_size = max(max_size, en.length());
        }
        
        vector<pair<string, size_t>> vec;
        vec.reserve(mf.size());

        move(begin(mf), end(mf), back_inserter(vec));

        sort(begin(vec), end(vec), [](auto r1, auto r2) { return r1.second < r2.second; });

        cout << "max en-lenght is [ " << max_size << " ]\n";
        for(auto [first, second] : vec)
        {
            cout << first << "#  :  " << second << "\n";
        }
    }

    map<string, size_t> mf;
    size_t max_size;
};


int main()
{
    En_Count call;

    call.parse();

    return 0;
}
