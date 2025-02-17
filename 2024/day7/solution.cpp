#include <print>
#include <fstream>
#include <string>
#include <vector>

struct Equation
{
    int64_t              result;
    std::vector<int64_t> numbers;
};

std::vector<Equation> parse_input(std::istream& input)
{
    std::vector<Equation> equations;

    for (std::string line; std::getline(input, line);)
    {
        Equation equation = {};
        size_t   i;

        for (i = 0; i < line.size(); ++i)
        {
            char c = line[i];
            if (c == ':')
            {
                break;
            }
            equation.result = equation.result * 10 + (c - '0');
        }

        int64_t number = 0;

        for (i = i + 2; i < line.size(); ++i)
        {
            char c = line[i];
            if (c == ' ')
            {
                equation.numbers.push_back(number);
                number = 0;
                continue;
            }
            number = number * 10 + (c - '0');
        }
        equation.numbers.push_back(number);
        equations.push_back(equation);
    }

    return equations;
}

enum class Operation
{
    add,
    multiply,
    concatenate,
    count,
};

int main(int argc, char* argv[])
{
    std::string   filename = argc < 2 ? "input.txt" : argv[1];
    std::ifstream inputFile(filename);

    if (!inputFile.is_open())
    {
        std::print("Couldn't open {}\n", filename);
        return -1;
    }

    auto equations = parse_input(inputFile);

    int64_t total_without_concat = 0;
    int64_t total_with_concat    = 0;

    for (const auto& equation : equations)
    {
        std::vector<Operation> ops(equation.numbers.size() - 1, Operation::add);

        size_t total_permutations = 1;
        for (size_t i = 0; i < ops.size(); ++i)
        {
            total_permutations *= static_cast<size_t>(Operation::count);
        }

        bool without_concat_done = false;
        bool with_concat_done    = false;

        for (size_t i = 0; i < total_permutations; ++i)
        {
            int64_t result_without_concat = equation.numbers[0];
            int64_t result_with_concat    = equation.numbers[0];
            bool    is_concat             = false;

            for (size_t j = 0; j < ops.size(); ++j)
            {
                int64_t n = equation.numbers[j + 1];
                if (ops[j] == Operation::add)
                {
                    result_without_concat += n;
                    result_with_concat += n;
                }
                else if (ops[j] == Operation::multiply)
                {
                    result_without_concat *= n;
                    result_with_concat *= n;
                }
                else
                {
                    is_concat   = true;
                    int64_t mag = 10;
                    int64_t x   = n;
                    while (x /= 10)
                    {
                        mag *= 10;
                    }
                    result_with_concat = result_with_concat * mag + n;
                }
            }

            if (!without_concat_done && equation.result == result_without_concat && !is_concat)
            {
                total_without_concat += equation.result;
                without_concat_done = true;
            }
            if (!with_concat_done && equation.result == result_with_concat)
            {
                total_with_concat += equation.result;
                with_concat_done = true;
            }

            if (without_concat_done && with_concat_done)
            {
                break;
            }

            // Do the next permutation of operations
            for (Operation& op : ops)
            {
                op = static_cast<Operation>(static_cast<int>(op) + 1);
                if (op == Operation::count)
                {
                    op = Operation::add;
                }
                else
                {
                    break;
                }
            }
        }
    }

    std::print("Part 1: {}\n", total_without_concat);
    std::print("Part 2: {}\n", total_with_concat);

    return 0;
}
