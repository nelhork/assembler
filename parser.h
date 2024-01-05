#ifndef PARSER_H
#define PARSER_H

#endif // PARSER_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

enum State {
    Start, Arg1, Arg2, Offset, RegisterArg1, RegisterArg2,
    RegisterArg, AddressArg, Argless, MovrrCommand, MovmrCommand,
    MovrmCommand, AddCommand, SubCommand, MulCommand, DivCommand,
    FaddCommand, FsubCommand, FmulCommand, FdivCommand,
    FinCommand, FoutCommand, JmpCommand, JeCommand, JgCommand,
    JlCommand, LeaCommand, CallCommand, RetCommand, InCommand,
    OutCommand, HaltCommand, CmpCommand, Error, Finish
};

class Parser {
public:
    Parser(std::string filename, std::string outputFileName);
    bool parse();
private:
    State state;
    std::string filename;
    std::fstream file;
    std::ofstream outputFile;
    std::vector<uint8_t> output;
    int regNumber1;
    int regNumber2;
    int firstHalfAddress;
    int secondHalfAddress;
    std::string outputFileName;

    void parseCommand(std::string str);
    int parseRegisterArg(std::string str, State desiredState);
    void parseAddressArg(std::string str);
    void setAddressCommands(std::string str, uint8_t opcode);
    void setRegistersCommand(std::string str, uint8_t opcode);
    void setInOrOutCommand(std::string str, uint8_t opcode);
};
