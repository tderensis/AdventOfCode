#include <print>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

std::vector<char> parse_input(std::istream& input)
{
    std::vector<char> result;

    for (std::string line; std::getline(input, line);)
    {
        for (char c : line)
        {
            result.push_back(c);
        }
    }
    return result;
}

enum class MulParseState
{
    init,
    m,
    u,
    l,
    open_paren,
    param1,
    comma,
    param2,
    d,
    o,
    n,
    app,
    t,
    open_paren_do,
};

MulParseState restart(char c)
{
    if (c == 'd')
    {
        return MulParseState::d;
    }
    else if (c == 'm')
    {
        return MulParseState::m;
    }
    else
    {
        return MulParseState::init;
    }
}

int main(int argc, char* argv[])
{
    std::string   filename = argc < 2 ? "input.txt" : argv[1];
    std::ifstream inputFile(filename);

    if (!inputFile.is_open())
    {
        std::print("Couldn't open {}\n", filename);
        return -1;
    }

    auto input = parse_input(inputFile);

    char          parse1[11]  = {};
    size_t        parseIndex1 = 0;
    char          parse2[11]  = {};
    size_t        parseIndex2 = 0;
    MulParseState state       = MulParseState::init;
    bool          isDo        = false;
    bool          mulEnabled  = true;

    int resultPart1 = 0;
    int resultPart2 = 0;

    for (char c : input)
    {
        switch (state)
        {
        case MulParseState::init: state = restart(c); break;
        case MulParseState::m:
            if (c == 'u')
            {
                state = MulParseState::u;
            }
            else
            {
                state = restart(c);
            }
            break;
        case MulParseState::u:
            if (c == 'l')
            {
                state = MulParseState::l;
            }
            else
            {
                state = restart(c);
            }
            break;
        case MulParseState::l:
            if (c == '(')
            {
                state = MulParseState::open_paren;
            }
            else
            {
                state = restart(c);
            }
            break;
        case MulParseState::open_paren:
            if (c >= '0' && c <= '9')
            {
                parseIndex1           = 0;
                state                 = MulParseState::param1;
                parse1[parseIndex1++] = c;
            }
            else
            {
                state = restart(c);
            }
            break;
        case MulParseState::param1:
            if (c >= '0' && c <= '9')
            {
                parse1[parseIndex1++] = c;
            }
            else if (c == ',')
            {
                state = MulParseState::comma;
            }
            else
            {
                state = restart(c);
            }
            break;
        case MulParseState::comma:
            if (c >= '0' && c <= '9')
            {
                parseIndex2           = 0;
                state                 = MulParseState::param2;
                parse2[parseIndex2++] = c;
            }
            else
            {
                state = restart(c);
            }
            break;
        case MulParseState::param2:
            if (c >= '0' && c <= '9')
            {
                parse2[parseIndex2++] = c;
            }
            else if (c == ')')
            {
                // parse the numbers and multiply
                parse1[parseIndex1] = '\0';
                parse2[parseIndex2] = '\0';
                int         param1;
                int         param2;
                std::string s1{parse1};
                std::string s2{parse2};

                std::stringstream(s1) >> param1;
                std::stringstream(s2) >> param2;

                resultPart1 += param1 * param2;

                if (mulEnabled)
                {
                    resultPart2 += param1 * param2;
                }
                state = MulParseState::init;
            }
            else
            {
                state = restart(c);
            }
            break;
        case MulParseState::d:
            if (c == 'o')
            {
                state = MulParseState::o;
            }
            else
            {
                state = restart(c);
            }
            break;
        case MulParseState::o:
            if (c == 'n')
            {
                state = MulParseState::n;
            }
            else if (c == '(')
            {
                isDo  = true;
                state = MulParseState::open_paren_do;
            }
            else
            {
                state = restart(c);
            }
            break;
        case MulParseState::n:
            if (c == '\'')
            {
                state = MulParseState::app;
            }
            else
            {
                state = restart(c);
            }
            break;
        case MulParseState::app:
            if (c == 't')
            {
                state = MulParseState::t;
            }
            else
            {
                state = restart(c);
            }
            break;
        case MulParseState::t:
            if (c == '(')
            {
                isDo  = false;
                state = MulParseState::open_paren_do;
            }
            else
            {
                state = restart(c);
            }
            break;
        case MulParseState::open_paren_do:
            if (c == ')')
            {
                mulEnabled = isDo;
                state      = MulParseState::init;
            }
            else
            {
                state = restart(c);
            }
            break;
        }
    }

    std::print("Part 1: {}\n", resultPart1);
    std::print("Part 2: {}\n", resultPart2);

    return 0;
}
