#include "parser.h"

Parser::Parser(std::string filename, std::string outputFileName)
{
    this->filename = filename;
    state = State::Start;
    this->outputFileName = outputFileName;
}

void Parser::parseCommand(std::string str)
{
    if (str == "movrr") {
        state = State::MovrrCommand;
    } else if (str == "movmr") {
        state = State::MovmrCommand;
    } else if (str == "movrm") {
        state = State::MovrmCommand;
    } else if (str == "add") {
        state = State::AddCommand;
    } else if (str == "sub") {
        state = State::SubCommand;
    } else if (str == "mul") {
        state = State::MulCommand;
    } else if (str == "div") {
        state = State::DivCommand;
    } else if (str == "fadd") {
        state = State::FaddCommand;
    } else if (str == "fsub") {
        state = State::FsubCommand;
    } else if (str == "fmul") {
        state = State::FmulCommand;
    } else if (str == "fdiv") {
        state = State::FdivCommand;
    } else if (str == "fin") {
        state = State::FinCommand;
    } else if (str == "fout") {
        state = State::FoutCommand;
    } else if (str == "jmp") {
        state = State::JmpCommand;
    } else if (str == "je") {
        state = State::JeCommand;
    } else if (str == "jg") {
        state = State::JgCommand;
    } else if (str == "jl") {
        state = State::JlCommand;
    } else if (str == "lea") {
        state = State::LeaCommand;
    } else if (str == "call") {
        state = State::CallCommand;
    } else if (str == "ret") {
        state = State::RetCommand;
    } else if (str == "in") {
        state = State::InCommand;
    } else if (str == "out") {
        state = State::OutCommand;
    } else if (str == "halt") {
        state = State::HaltCommand;
    } else if (str == "cmp") {
        state = State::CmpCommand;
    } else {
        state = State::Error;
    }
}

int Parser::parseRegisterArg(std::string str, State desiredState)
{
    if (str.length() <= 1 || str.length() > 3) {
        state = State::Error;
    }
    else if (str[0] == 'r') {
        if (str.length() == 2) {
            std::string strNums;
            strNums += str[1];
            int number = std::stoi(strNums);
            if (number >= 0) {
                state = desiredState;
                return number;
            } else {
                state = State::Error;
            }
        } else if (str.length() == 3) {
            std::string strNums;
            strNums += str[1];
            strNums += str[2];
            int nums = std::stoi(strNums);
            if (nums >= 0 && nums <= 15) {
                state = desiredState;
                return nums;
            } else {
                state = State::Error;
            }
        } else {
            state = State::Error;
        }
    }
    return -1;
}

void Parser::parseAddressArg(std::string str)
{
    if (str.length() != 4) {
        state = State::Error;
        return;
    }
    std::string strNum1, strNum2;
    strNum1 += str[0];
    strNum1 += str[1];
    strNum2 += str[2];
    strNum2 += str[3];
    firstHalfAddress = std::stoi(strNum1, 0, 16);
    secondHalfAddress = std::stoi(strNum2, 0, 16);
}

void Parser::setAddressCommands(std::string str, uint8_t opcode)
{
    output.push_back(opcode);
    parseAddressArg(str);
    output.push_back(firstHalfAddress);
    output.push_back(secondHalfAddress);
    state = State::AddressArg;
}

void Parser::setRegistersCommand(std::string str, uint8_t opcode)
{
    output.push_back(opcode);
    regNumber1 = parseRegisterArg(str, State::RegisterArg1);
}

void Parser::setInOrOutCommand(std::string str, uint8_t opcode)
{
    output.push_back(opcode);
    regNumber1 = parseRegisterArg(str, State::RegisterArg);
    output.push_back(regNumber1);
    output.push_back(0x0);
}

bool Parser::parse()
{
    file.open(filename, std::ios::in);
    if (!file.is_open()) {
        std::cerr << "cannot open file!" << std::endl;
    }

    std::string str;
    while (file >> str) {
        std::cout << str << "\n";

        switch (state) {
        case State::Start:
            parseCommand(str);
            break;
        case State::MovrrCommand:
            setRegistersCommand(str, 0x01);
            break;
        case State::RegisterArg1:
            regNumber2 = parseRegisterArg(str, State::RegisterArg2);
            output.push_back((regNumber2 << 4) + regNumber1);
            output.push_back(0x0);
            break;
        case State::RegisterArg:
        case State::RegisterArg2:
        case State::AddressArg:
        case State::Argless:
            parseCommand(str);
            break;
        case State::MovmrCommand:
            setRegistersCommand(str, 0x02);
            break;
        case State::MovrmCommand:
            setRegistersCommand(str, 0x03);
            break;
        case State::AddCommand:
            setRegistersCommand(str, 0x10);
            break;
        case State::SubCommand:
            setRegistersCommand(str, 0x11);
            break;
        case State::MulCommand:
            setRegistersCommand(str, 0x12);
            break;
        case State::DivCommand:
            setRegistersCommand(str, 0x13);
            break;
        case State::FaddCommand:
            setRegistersCommand(str, 0x20);
            break;
        case State::FsubCommand:
            setRegistersCommand(str, 0x21);
            break;
        case State::FmulCommand:
            setRegistersCommand(str, 0x22);
            break;
        case State::FdivCommand:
            setRegistersCommand(str, 0x23);
            break;
        case State::FinCommand:
            setInOrOutCommand(str, 0x24);
            break;
        case State::FoutCommand:
            setInOrOutCommand(str, 0x25);
            break;
        case State::JmpCommand:
            setAddressCommands(str, 0x30);
            break;
        case State::JeCommand:
            setAddressCommands(str, 0x33);
            break;
        case State::JgCommand:
            setAddressCommands(str, 0x35);
            break;
        case State::JlCommand:
            setAddressCommands(str, 0x37);
            break;
        case State::LeaCommand:
            setAddressCommands(str, 0x40);
            break;
        case State::CallCommand:
            setAddressCommands(str, 0x50);
            break;
        case State::RetCommand:
            parseCommand(str);
            output.push_back(0x51);
            output.push_back(0x0);
            output.push_back(0x0);
            break;
        case State::InCommand:
            setInOrOutCommand(str, 0x52);
            break;
        case State::OutCommand:
            setInOrOutCommand(str, 0x53);
            break;
        case State::HaltCommand:
            parseCommand(str);
            output.push_back(0x54);
            output.push_back(0x0);
            output.push_back(0x0);
            break;
        case State::CmpCommand:
            setRegistersCommand(str, 0x60);
            break;
        default:
            break;
        }
    }
    if (state == State::HaltCommand) {
        output.push_back(0x54);
        output.push_back(0x0);
        output.push_back(0x0);
    } if (state == State::RetCommand) {
        output.push_back(0x51);
        output.push_back(0x0);
        output.push_back(0x0);
    }
    outputFile.open(outputFileName, std::ios::out | std::ios::binary);
    for (size_t var = 0; var < output.size(); ++var) {
        outputFile.write((char*)&output[var], sizeof(uint8_t));
    }
    outputFile.close();
    return true;
}
