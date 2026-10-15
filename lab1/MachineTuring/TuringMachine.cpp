#include "TuringMachine.hpp"
#include "Tape.hpp"
#include "Program.hpp"

TuringMachine::TuringMachine(Tape& tape, Program& program, std::string currentState):
    tape_(tape), program_(program), currentState_(currentState), isHalted_(false){}

bool TuringMachine::isHalted() const{
    return isHalted_;
}

TuringMachine& TuringMachine::operator++(){
    if(!isHalted_){
        char currentSymbol=tape_.readCurrentSymbol();
        const Action* action=program_.searchForRule(currentState_, currentSymbol);
        if(action){
            tape_.writeNewSymbol(action->newSymbol);
            tape_.moveToDirection(action->direction);
            currentState_=action->newState;
            if(currentState_=="halt") isHalted_=true;
        }
        else isHalted_=true;
    }
    return *this;
}

const std::string TuringMachine::getCurrentState() const {
    return currentState_;
}

void TuringMachine::run(bool needLog, size_t maxSteps){
    size_t steps=0;
    if(needLog) std::cout<<"Start\n"<<tape_;
    while(!isHalted_&&steps<maxSteps){
        ++(*this);
        steps++;
        if (needLog) std::cout<<tape_;
    }
    if(!isHalted_) isHalted_=true;
}