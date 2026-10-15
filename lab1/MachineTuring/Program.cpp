#include "Program.hpp"

void Program::addRule(const std::string& currentState, char currentSymbol, const Action& action){
    rules_[{currentState, currentSymbol}]=action;
}

const Action* Program::searchForRule(const std::string& currentState, char currentSymbol) const{
    auto act=rules_.find({currentState, currentSymbol});
    if(act!=rules_.end()){
        return &(act->second);
    }
    else return nullptr;
}

void Program::deleteRule(const std::string& currentState, char currentSymbol){
    rules_.erase({currentState, currentSymbol});
}

char directionToChar(Direction dir){
    if (dir==LEFT) return 'L';
    else if(dir==RIGHT) return 'R';
    return 'S';
}

std::ostream& operator<<(std::ostream& output, const Program& prog){
    for(const auto& item:prog.rules_){
        output<<item.first.first<<" "<<item.first.second<<"->"
        <<item.second.newState<<" "<<item.second.newSymbol<<" "
        <<directionToChar(item.second.direction)<<"\n";
    }
    return output;
}

std::istream& operator>>(std::istream& input, Program& prog){
    std::string currentState;
    char currentSymbol;
    std::string arrow;
    std::string newState;
    char newSymbol;
    char dir;
    Direction direction;
    while(input>>currentState>>currentSymbol>>arrow>>newState>>newSymbol>>dir){
        if (dir=='R') direction=RIGHT;
        else if(dir=='L') direction=LEFT;
        else direction=STAY;
        Action action{newState, newSymbol, direction};
        prog.addRule(currentState, currentSymbol, action);
    }
    return input;
}


