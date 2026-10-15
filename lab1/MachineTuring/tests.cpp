#include <UnitTest++/UnitTest++.h>
#include <sstream>
#include "Tape.hpp"
#include "Program.hpp"
#include "TuringMachine.hpp"

SUITE(TapeTests) {

    TEST(DefaultConstructorBlankSymbol) {
        Tape tape;
        CHECK_EQUAL('_', tape.readCurrentSymbol());
        CHECK_EQUAL('_', tape.getSymbolAt(0));
        CHECK_EQUAL('_', tape.getSymbolAt(-5));
        CHECK_EQUAL('_', tape.getSymbolAt(5));
    }

    TEST(CustomBlankSymbol) {
        Tape tape('#');
        CHECK_EQUAL('#', tape.readCurrentSymbol());
        CHECK_EQUAL('#', tape.getSymbolAt(10));
    }

    TEST(WriteAndReadCurrentSymbol) {
        Tape tape;
        tape.writeNewSymbol('1');
        CHECK_EQUAL('1', tape.readCurrentSymbol());
        CHECK_EQUAL('1', tape.getSymbolAt(0));
    }

    TEST(MoveRightAndWrite) {
        Tape tape;
        tape.writeNewSymbol('A');
        tape.moveToDirection(RIGHT);
        CHECK_EQUAL('_', tape.readCurrentSymbol());
        tape.writeNewSymbol('B');
        CHECK_EQUAL('B', tape.readCurrentSymbol());
        CHECK_EQUAL('A', tape.getSymbolAt(0));
        CHECK_EQUAL('B', tape.getSymbolAt(1));
    }

    TEST(MoveLeftAndWriteNegativeIndex) {
        Tape tape;
        tape.moveToDirection(LEFT);
        tape.writeNewSymbol('Z');
        CHECK_EQUAL('Z', tape.getSymbolAt(-1));
        CHECK_EQUAL('Z', tape.readCurrentSymbol());
    }

    TEST(MoveStayDoesNotChangePosition) {
        Tape tape;
        tape.writeNewSymbol('X');
        tape.moveToDirection(STAY);
        CHECK_EQUAL('X', tape.readCurrentSymbol());
        CHECK_EQUAL('X', tape.getSymbolAt(0));
    }

    TEST(OverwriteSymbolAtSamePosition) {
        Tape tape;
        tape.writeNewSymbol('1');
        CHECK_EQUAL('1', tape.readCurrentSymbol());
        tape.writeNewSymbol('0');
        CHECK_EQUAL('0', tape.readCurrentSymbol());
    }

    TEST(MultipleSequentialMoves) {
        Tape tape;
        tape.moveToDirection(RIGHT);
        tape.moveToDirection(RIGHT);
        tape.moveToDirection(LEFT);
        tape.writeNewSymbol('M');
        CHECK_EQUAL('M', tape.getSymbolAt(1));
    }

    TEST(StreamInputOperator) {
        Tape tape;
        std::istringstream iss("1101");
        iss >> tape;
        CHECK_EQUAL('1', tape.getSymbolAt(0));
        CHECK_EQUAL('1', tape.getSymbolAt(1));
        CHECK_EQUAL('0', tape.getSymbolAt(2));
        CHECK_EQUAL('1', tape.getSymbolAt(3));
        CHECK_EQUAL('_', tape.getSymbolAt(4));
        CHECK_EQUAL('1', tape.readCurrentSymbol());
    }

    TEST(StreamInputClearsPreviousContent) {
        Tape tape;
        tape.writeNewSymbol('A');
        tape.moveToDirection(RIGHT);
        tape.writeNewSymbol('B');

        std::istringstream iss("XYZ");
        iss >> tape;
        CHECK_EQUAL('X', tape.getSymbolAt(0));
        CHECK_EQUAL('Y', tape.getSymbolAt(1));
        CHECK_EQUAL('Z', tape.getSymbolAt(2));
        CHECK_EQUAL('_', tape.getSymbolAt(3));
    }

    TEST(StreamOutputOperator) {
        Tape tape;
        std::istringstream iss("ABCDE");
        iss >> tape;
        std::ostringstream oss;
        oss << tape;
        CHECK_EQUAL("_ _ [A] B C\n", oss.str());
    }

    TEST(EqualityOperatorSameTapes) {
        Tape t1, t2;
        t1.writeNewSymbol('a');
        t2.writeNewSymbol('a');
        CHECK(t1 == t2);
        CHECK(!(t1 != t2));
    }

    TEST(EqualityOperatorDifferentSymbols) {
        Tape t1, t2;
        t1.writeNewSymbol('a');
        t2.writeNewSymbol('b');
        CHECK(t1 != t2);
        CHECK(!(t1 == t2));
    }

    TEST(EqualityOperatorDifferentPositions) {
        Tape t1, t2;
        t1.writeNewSymbol('x');
        t2.writeNewSymbol('x');
        t2.moveToDirection(RIGHT);
        CHECK(t1 != t2);
    }

    TEST(EqualityOperatorDifferentBlankSymbols) {
        Tape t1('_');
        Tape t2('#');
        CHECK(t1 != t2);
    }
}

SUITE(ProgramTests) {

    TEST(AddAndSearchRuleSuccess) {
        Program prog;
        Action act{"q1", '1', RIGHT};
        prog.addRule("q0", '0', act);

        const Action* found = prog.searchForRule("q0", '0');
        CHECK(found != nullptr);
        if (found) {
            CHECK_EQUAL("q1", found->newState);
            CHECK_EQUAL('1', found->newSymbol);
            CHECK_EQUAL(RIGHT, found->direction);
        }
    }

    TEST(SearchRuleNotFoundWrongSymbol) {
        Program prog;
        prog.addRule("q0", '0', {"q1", '1', RIGHT});
        CHECK(prog.searchForRule("q0", '1') == nullptr);
    }

    TEST(SearchRuleNotFoundWrongState) {
        Program prog;
        prog.addRule("q0", '0', {"q1", '1', RIGHT});
        CHECK(prog.searchForRule("q_unknown", '0') == nullptr);
    }

    TEST(SearchInEmptyProgram) {
        Program prog;
        CHECK(prog.searchForRule("q0", '_') == nullptr);
    }

    TEST(OverwriteRule) {
        Program prog;
        prog.addRule("q0", '0', {"q1", '1', RIGHT});
        prog.addRule("q0", '0', {"q2", '_', LEFT});

        const Action* found = prog.searchForRule("q0", '0');
        CHECK(found != nullptr);
        if (found) {
            CHECK_EQUAL("q2", found->newState);
            CHECK_EQUAL('_', found->newSymbol);
            CHECK_EQUAL(LEFT, found->direction);
        }
    }

    TEST(DeleteExistingRule) {
        Program prog;
        prog.addRule("q0", 'a', {"q1", 'b', STAY});
        CHECK(prog.searchForRule("q0", 'a') != nullptr);

        prog.deleteRule("q0", 'a');
        CHECK(prog.searchForRule("q0", 'a') == nullptr);
    }

    TEST(DeleteNonExistingRuleDoesNotThrow) {
        Program prog;
        prog.deleteRule("q0", 'x');
        CHECK(prog.searchForRule("q0", 'x') == nullptr);
    }

    TEST(StreamInputMultipleRules) {
        Program prog;
        std::istringstream iss(
            "q0 0 -> q0 1 R\n"
            "q0 1 -> q1 0 L\n"
            "q1 _ -> halt _ S\n"
        );
        iss >> prog;

        const Action* a1 = prog.searchForRule("q0", '0');
        CHECK(a1 != nullptr);
        if (a1) {
            CHECK_EQUAL("q0", a1->newState);
            CHECK_EQUAL('1', a1->newSymbol);
            CHECK_EQUAL(RIGHT, a1->direction);
        }

        const Action* a2 = prog.searchForRule("q0", '1');
        CHECK(a2 != nullptr);
        if (a2) {
            CHECK_EQUAL("q1", a2->newState);
            CHECK_EQUAL('0', a2->newSymbol);
            CHECK_EQUAL(LEFT, a2->direction);
        }

        const Action* a3 = prog.searchForRule("q1", '_');
        CHECK(a3 != nullptr);
        if (a3) {
            CHECK_EQUAL("halt", a3->newState);
            CHECK_EQUAL('_', a3->newSymbol);
            CHECK_EQUAL(STAY, a3->direction);
        }
    }

    TEST(StreamOutputFormat) {
        Program prog;
        prog.addRule("q0", '0', {"q1", '1', RIGHT});
        std::ostringstream oss;
        oss << prog;
        CHECK_EQUAL("q0 0->q1 1 R\n", oss.str());
    }
}

SUITE(TuringMachineUnitTests) {

    TEST(InitialStateAndNotHalted) {
        Tape tape;
        Program prog;
        TuringMachine tm(tape, prog, "q0");
        CHECK_EQUAL("q0", tm.getCurrentState());
        CHECK(!tm.isHalted());
    }

    TEST(StepWithoutMatchingRuleHalts) {
        Tape tape;
        Program prog; 
        TuringMachine tm(tape, prog, "q0");

        ++tm;
        CHECK(tm.isHalted());
    }

    TEST(StepAppliesSymbolAndStateChange) {
        Tape tape;
        tape.writeNewSymbol('0');
        Program prog;
        prog.addRule("q0", '0', {"q1", '1', STAY});

        TuringMachine tm(tape, prog, "q0");
        ++tm;

        CHECK_EQUAL("q1", tm.getCurrentState());
        CHECK_EQUAL('1', tape.readCurrentSymbol());
        CHECK(!tm.isHalted());
    }

    TEST(StepMovesHeadRight) {
        Tape tape;
        tape.writeNewSymbol('A');
        Program prog;
        prog.addRule("q0", 'A', {"q1", 'A', RIGHT});

        TuringMachine tm(tape, prog, "q0");
        ++tm;

        CHECK_EQUAL(1, tape.getSymbolAt(1) == '_' ? 1 : 1);
        CHECK_EQUAL('_', tape.readCurrentSymbol());
        CHECK_EQUAL('A', tape.getSymbolAt(0));
    }

    TEST(StepMovesHeadLeft) {
        Tape tape;
        tape.writeNewSymbol('B');
        Program prog;
        prog.addRule("q0", 'B', {"q1", 'B', LEFT});

        TuringMachine tm(tape, prog, "q0");
        ++tm;

        CHECK_EQUAL('_', tape.readCurrentSymbol());
        CHECK_EQUAL('B', tape.getSymbolAt(0));
    }

    TEST(StepHaltsExplicitlyOnHaltState) {
        Tape tape;
        tape.writeNewSymbol('x');
        Program prog;
        prog.addRule("q0", 'x', {"halt", 'y', STAY});

        TuringMachine tm(tape, prog, "q0");
        ++tm;

        CHECK_EQUAL("halt", tm.getCurrentState());
        CHECK(tm.isHalted());
        CHECK_EQUAL('y', tape.readCurrentSymbol());
    }

    TEST(StepDoesNothingIfAlreadyHalted) {
        Tape tape;
        Program prog;
        TuringMachine tm(tape, prog, "q0");

        ++tm; 
        CHECK(tm.isHalted());

        ++tm; 
        CHECK(tm.isHalted());
    }

    TEST(RunHaltsByMaxStepsLimit) {
        Tape tape;
        tape.writeNewSymbol('0');
        Program prog;
        
        prog.addRule("q0", '0', {"q0", '0', STAY});

        TuringMachine tm(tape, prog, "q0");
        tm.run(false, 15);

        CHECK(tm.isHalted());
        CHECK_EQUAL("q0", tm.getCurrentState());
    }
}

SUITE(TuringMachineAlgorithmTests) {

    TEST(InvertBitsNotAlgorithm) {
        Tape tape;
        std::istringstream iss("101");
        iss >> tape;

        Program prog;
        
        prog.addRule("q0", '1', {"q0", '0', RIGHT});
        prog.addRule("q0", '0', {"q0", '1', RIGHT});
        prog.addRule("q0", '_', {"halt", '_', STAY});

        TuringMachine tm(tape, prog, "q0");
        tm.run(false);

        CHECK(tm.isHalted());
        CHECK_EQUAL('0', tape.getSymbolAt(0));
        CHECK_EQUAL('1', tape.getSymbolAt(1));
        CHECK_EQUAL('0', tape.getSymbolAt(2));
    }

    TEST(EraseAllSymbolsAlgorithm) {
        Tape tape;
        std::istringstream iss("AAA");
        iss >> tape;

        Program prog;
        prog.addRule("q0", 'A', {"q0", '_', RIGHT});
        prog.addRule("q0", '_', {"halt", '_', STAY});

        TuringMachine tm(tape, prog, "q0");
        tm.run(false);

        CHECK_EQUAL('_', tape.getSymbolAt(0));
        CHECK_EQUAL('_', tape.getSymbolAt(1));
        CHECK_EQUAL('_', tape.getSymbolAt(2));
        CHECK(tm.isHalted());
    }

    TEST(BinaryIncrementAlgorithm) {
        Tape tape;
        std::istringstream iss("101_");
        iss >> tape;

        Program prog;
        prog.addRule("q0", '0', {"q0", '0', RIGHT});
        prog.addRule("q0", '1', {"q0", '1', RIGHT});
        prog.addRule("q0", '_', {"q_carry", '_', LEFT}); 

        prog.addRule("q_carry", '0', {"halt", '1', STAY});
        prog.addRule("q_carry", '1', {"q_carry", '0', LEFT});
        prog.addRule("q_carry", '_', {"halt", '1', STAY});

        TuringMachine tm(tape, prog, "q0");
        tm.run(false);

        CHECK(tm.isHalted());
        CHECK_EQUAL('1', tape.getSymbolAt(0));
        CHECK_EQUAL('1', tape.getSymbolAt(1));
        CHECK_EQUAL('0', tape.getSymbolAt(2));
    }

    TEST(BinaryIncrementWithOverflow) {
        Tape tape;
        std::istringstream iss("11_");
        iss >> tape;

        Program prog;
        prog.addRule("q0", '1', {"q0", '1', RIGHT});
        prog.addRule("q0", '_', {"q_carry", '_', LEFT});
        prog.addRule("q_carry", '1', {"q_carry", '0', LEFT});
        prog.addRule("q_carry", '_', {"halt", '1', STAY});

        TuringMachine tm(tape, prog, "q0");
        tm.run(false);

        CHECK_EQUAL('1', tape.getSymbolAt(-1));
        CHECK_EQUAL('0', tape.getSymbolAt(0));
        CHECK_EQUAL('0', tape.getSymbolAt(1));
    }
}

int main() {
    return UnitTest::RunAllTests();
}