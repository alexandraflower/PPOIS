#include "Tape.hpp"
#include "Program.hpp"
#include "TuringMachine.hpp"
#include <fstream>
#include <iostream>
using namespace std;

int main(int argc, char* argv[]){
    if(argc<2){
        std::cerr<<"Invalid input.\n";
        return 1;
    }
    std::string filename=argv[1];
    bool needLog=false;
    for(int i=2;i<argc;i++){
        if(std::string(argv[i])=="-log") needLog=true;
    }

    std::ifstream workFile(filename);
    if(!workFile.is_open()){
        std::cerr<<"File opening error.\n";
        return 1;
    }
    Tape tape;
    Program program;
    workFile>>tape;
    workFile>>program;
    workFile.close();
    TuringMachine myMachine(tape, program, "q0");
    cout<<program;
    cout<<"Initial state: ";
    tape.getFullTape();
    myMachine.run(needLog);
    cout<<"Result: ";
    tape.getFullTape();
    return 0;
}