#include <cassert>
#include <cmath>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <vector>

using namespace std;

class invalid_argument : public exception
{
  public:
    invalid_argument(const char *s) : err(s) {}
    const char* what() const noexcept { return err; }

    const char *err;
};

class Rpn
{
  public:
    Rpn()
    {
        stk = {};
        mf = {{"+", [](double a, double b) { return a + b; }},
              {"-", [](double a, double b) { return a - b; }},
              {"*", [](double a, double b) { return a * b; }},
              {"/", [](double a, double b) { return a / b; }},
              {"^", [](double a, double b) { return pow(a, b); }},
              {"%", [](double a, double b) { return fmod(a, b); }}};
    }

    double rpn_pop()
    {
        auto ret = stk.top();
        stk.pop();

        return ret;
    }

    template <class IT> double erase_rpn(IT start, IT end)
    {
        for (auto it = start; it != end; ++it) {
            stringstream ss{*it};
            if (double dv; ss >> dv) {
                stk.push(dv);
            }
            else {
                auto a1 = rpn_pop();
                auto a2 = rpn_pop();
                auto op = *it;

                try {
                    double res = mf.at(op)(a1, a2);
                    stk.push(res);
                }
                catch (const out_of_range &) {
                    throw ::invalid_argument((*it).c_str());
                }
            }
        }
        return stk.top();
    }

  protected:
    stack<double> stk;
    map<string, double (*)(double, double)> mf;
};

int main()
{
    try {
        Rpn call{};
        stringstream s {"1 2 3 + -"};
        double result = call.erase_rpn(istream_iterator<string>{s}, {});
        double result2 = call.erase_rpn(istream_iterator<string>{cin}, {});
        cout << "( " <<  s.str() << " )" << " --> " << result2 << "\n";
        cout << "( " <<  s.str() << " )" << " --> " << result << "\n";
    }
    catch (const ::invalid_argument &e) {
        cout << e.what() << "\n";
    }

    return 0;
}