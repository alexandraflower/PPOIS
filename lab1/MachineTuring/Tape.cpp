#include "Tape.hpp"
#include <map>
#include <iostream>
#include <algorithm>

Tape::Tape(char blankSymb): cellNumber_(0), blankSymbol_(blankSymb){}

char Tape::getSymbolAt(int position) const{
    auto symb=cells_.find(position);
    if (symb!=cells_.end()){
        return symb->second;
    }
    else{
        return blankSymbol_;
    }
}

char Tape::readCurrentSymbol() const{
    return getSymbolAt(cellNumber_);
}

void Tape::writeNewSymbol(char newSymbol){
    cells_[cellNumber_]=newSymbol;
}

void Tape::moveToDirection(Direction direction){
    if(direction==LEFT){
        cellNumber_--;
    } else if(direction==RIGHT){
        cellNumber_++;
    }
}

void Tape::getFullTape() const{
    if(cells_.empty()){
        std::cout<<"["<<blankSymbol_<<"]\n";
        return;
    }
    int minIndex=std::min(cells_.begin()->first, cellNumber_);
    int maxIndex=std::max(cells_.rbegin()->first, cellNumber_);

    for(int i=minIndex;i<=maxIndex;i++){
        char symbol=getSymbolAt(i);

        if(i==cellNumber_){
            std::cout<<"["<<symbol<<"] ";
        } else std::cout<<symbol<<" ";
    }
    std::cout<<"\n";
}

std::ostream& operator<<(std::ostream& output, const Tape& tape){
    output<<tape.getSymbolAt(tape.cellNumber_-2)<<" "<<tape.getSymbolAt(tape.cellNumber_-1)<<" ["<<tape.getSymbolAt(tape.cellNumber_)<<"] "<<
    tape.getSymbolAt(tape.cellNumber_+1)<<" "<<tape.getSymbolAt(tape.cellNumber_+2)<<"\n";
    return output;
}

std::istream& operator>>(std::istream& input, Tape& tape){
    std::string expression;
    input>>expression;
    tape.cells_.clear();
    tape.cellNumber_=0;
    for (std::size_t i=0; i<expression.length();i++){
        tape.cells_[i]=expression[i];
    }
    return input;
}

bool Tape::operator==(const Tape& tape) const{
    return (cellNumber_==tape.cellNumber_)&&(cells_==tape.cells_)&&(blankSymbol_==tape.blankSymbol_);
}

bool Tape::operator!=(const Tape& tape) const{
    return !(*this==tape);
}