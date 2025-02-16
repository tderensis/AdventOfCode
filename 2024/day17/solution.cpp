#include <print>
#include <fstream>
#include <cstdint>
#include <string>
#include <vector>

struct Registers
{
    int64_t a;
    int64_t b;
    int64_t c;
};

using Instruction = int;
using Instructions = std::vector<Instruction>;

std::tuple<Registers, Instructions> parse_input(std::istream& s)
{
    Registers registers;
    Instructions instructions;
    int line_count = 0;

    for (std::string line; getline(s, line);)
    {
        if (line_count == 0)
            sscanf(line.data(), "Register A: %lld", &registers.a);
        else if (line_count == 1)
            sscanf(line.data(), "Register B: %lld", &registers.b);
        else if (line_count == 2)
            sscanf(line.data(), "Register C: %lld", &registers.c);
        else
            for (char c : line)
                if (c >= '0' && c <= '7')
                    instructions.push_back(c - '0');
        ++line_count;
    }

    return { registers, instructions };
}

void print(const Registers& registers)
{
    std::print("A: {} B: {} C: {}\n", registers.a, registers.b, registers.c);
}

void print(const Instructions& instructions)
{
    std::print("Instructions: ");
    for (auto i : instructions)
        std::print("{},", i);
    std::print("\n");
}

int run_instruction(Registers& registers, const Instructions& instructions, int& pc)
{
    if (pc < 0 || pc >= instructions.size())
    {
        return -1; // halt
    }
    int opcode              = instructions[pc++];
    int64_t operand_literal = instructions[pc++];
    int64_t& operand_combo =
        (operand_literal == 4) ? registers.a :
        (operand_literal == 5) ? registers.b :
        (operand_literal == 6) ? registers.c :
        operand_literal;

    //std::print("opcode: {} operand_literal: {} combo: {}\n",
    //opcode, operand_literal, operand_combo);
    switch (opcode)
    {
        case 0:
        // The adv instruction (opcode 0) performs division.
        // The numerator is the value in the A register.
        // The denominator is found by raising 2 to the power of the
        // instruction's combo operand. (So, an operand of 2 would divide A by 4 (2^2);
        // an operand of 5 would divide A by 2^B.) The result of the division operation
        // is truncated to an integer and then written to the A register.
			registers.a = registers.a >> operand_combo;
			return -2;
        
        case 1:
        // The bxl instruction (opcode 1) calculates the bitwise XOR of register B
        // and the instruction's literal operand, then stores the result in register B.
        registers.b = registers.b ^ operand_literal;
        return -2;
        
        case 2:
        // The bst instruction (opcode 2) calculates the value of its combo operand
        // modulo 8 (thereby keeping only its lowest 3 bits), then writes that value
        // to the B register.
        registers.b = operand_combo % 8;
        return -2;
        
        case 3:
        // The jnz instruction (opcode 3) does nothing if the A register is 0.
        // However, if the A register is not zero, it jumps by setting the instruction
        // pointer to the value of its literal operand; if this instruction jumps,
        // the instruction pointer is not increased by 2 after this instruction.
        if (registers.a)
        {
            pc = operand_literal;
        }
        return -2;
        
        case 4:
        // The bxc instruction (opcode 4) calculates the bitwise XOR of register B
        // and register C, then stores the result in register B.
        // (For legacy reasons, this instruction reads an operand but ignores it.)
        registers.b = registers.b ^ registers.c;
        return -2;
        
        case 5:
        // The out instruction (opcode 5) calculates the value of its combo operand
        // modulo 8, then outputs that value. (If a program outputs multiple values, they are separated by commas.)
        //std::print("{},", operand_combo % 8);
        return operand_combo % 8;
        
        case 6:
        // The bdv instruction (opcode 6) works exactly like the adv instruction except
        // that the result is stored in the B register. (The numerator is still read
        // from the A register.)
			registers.b = registers.a >> operand_combo;
        return -2;
        
        case 7:
        // The cdv instruction (opcode 7) works exactly like the adv instruction except
        // that the result is stored in the C register. (The numerator is still read
        // from the A register.)
			registers.c = registers.a >> operand_combo;
        return -2;
    }
    return -1;
}

std::vector<int> run(Registers registers, const Instructions& instructions)
{
	std::vector<int> output;
	int pc = 0;
	while (true)
	{
		int result = run_instruction(registers, instructions, pc);
		if (result == -1)
		{
			return output;
		}
		else if (result >= 0)
		{
		    output.push_back(result);
		}
	}
}

int main(int argc, char* argv[])
{
    std::print("Day 17 Solution\n");

    std::string   filename = argc < 2 ? "input.txt" : argv[1];
    std::ifstream inputFile(filename);

    if (!inputFile.is_open())
    {
        std::print("Couldn't open {}\n", filename);
        return -1;
    }

    auto [registers, instructions] = parse_input(inputFile);

    print(registers);
    print(instructions);

    print(run(registers, instructions));

    return 0;
}
