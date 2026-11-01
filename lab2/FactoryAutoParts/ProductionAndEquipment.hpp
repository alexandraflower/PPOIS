#pragma once
#include "Exceptions.hpp"
#include <string>

// Опережающие объявления из других модулей
class AutoPart;
class AssembledUnit;
class RawMaterial;
class PlantEngineer;
class ProductionShift;
class Employee;

// ==================== ЦЕХ И СТАНКИ ====================

class Machine {
protected:
    std::string machineId_;
    double nominalPowerKw_;
    double totalOperatingHours_;
    double currentTempCelcius_;
    double maxAllowedTemp_;
    bool isOperational_;

public:
    Machine(const std::string& machineId, double nominalPowerKw, double totalOperatingHours,
            double currentTempCelcius, double maxAllowedTemp, bool isOperational = true)
        : machineId_(machineId), nominalPowerKw_(nominalPowerKw), totalOperatingHours_(totalOperatingHours),
          currentTempCelcius_(currentTempCelcius), maxAllowedTemp_(maxAllowedTemp), isOperational_(isOperational) {
        if (nominalPowerKw <= 0.0 || totalOperatingHours < 0.0) {
            throw PlantException("Invalid machine parameters.");
        }
    }
    virtual ~Machine() = default;

    std::string getMachineId() const { return machineId_; }
    bool isOperational() const { return isOperational_; }

    void startEngine();
    void emergencyStop();
    void logOverheatingEvent() const;
    void serviceResetOperatingHours();
};

class CastingFurnace : public Machine {
private:
    double crucibleCapacityKg_;
    double chamberTemp_;
    double gasBurnerPower_;
    double meltViscosity_;

public:
    CastingFurnace(const std::string& machineId, double nominalPowerKw, double totalOperatingHours,
                   double currentTempCelcius, double maxAllowedTemp, double crucibleCapacityKg,
                   double chamberTemp, double gasBurnerPower, double meltViscosity, bool isOperational = true);
    ~CastingFurnace() override = default;

    void heatChamber(double targetTemp);
    void pourMoltenMetal(double pouredWeight);
    void skimSlagLayer();
};

class StampingPress : public Machine {
private:
    double clampingForceTons_;
    double strokeLengthMm_;
    double cycleTimeSeconds_;
    std::string installedDieId_;

public:
    StampingPress(const std::string& machineId, double nominalPowerKw, double totalOperatingHours,
                  double currentTempCelcius, double maxAllowedTemp, double clampingForceTons,
                  double strokeLengthMm, double cycleTimeSeconds, const std::string& installedDieId,
                  bool isOperational = true);
    ~StampingPress() override = default;

    void executeStampingStroke();
    bool checkGuardInterlock() const;
    void ejectFormedPart();
};

class CncMachine : public Machine {
private:
    double spindleSpeedRpm_;
    double feedRate_;
    double coolantPressureBar_;
    int activeToolNumber_;
    bool isCalibrated_;

public:
    CncMachine(const std::string& machineId, double nominalPowerKw, double totalOperatingHours,
               double currentTempCelcius, double maxAllowedTemp, double spindleSpeedRpm,
               double feedRate, double coolantPressureBar, int activeToolNumber,
               bool isCalibrated = true, bool isOperational = true);
    ~CncMachine() override = default;

    void loadGCodeProgram(const std::string& programName);
    void executeMillingPass(AutoPart& part);
    void supplyCoolantFluid();
};

class AssemblyRobot : public Machine {
private:
    int degreesOfFreedom_;
    double gripperPayloadKg_;
    double positioningAccuracyMm_;
    bool isEStopEngaged_;

public:
    AssemblyRobot(const std::string& machineId, double nominalPowerKw, double totalOperatingHours,
                  double currentTempCelcius, double maxAllowedTemp, int degreesOfFreedom,
                  double gripperPayloadKg, double positioningAccuracyMm,
                  bool isEStopEngaged = false, bool isOperational = true);
    ~AssemblyRobot() override = default;

    void pickAndPlaceComponent(AutoPart& part, AssembledUnit& unit);
    void calibrateKinematicAxes();
    void tightenFastenerJoint(double targetTorque);
};

// ==================== ОСНАСТКА И ИНФРАСТРУКТУРА ====================

class ToolFixture {
private:
    std::string fixtureSerial_;
    int maxWearCycles_;
    int currentCycleCount_;
    double cuttingEdgeSharpness_;

public:
    ToolFixture(const std::string& fixtureSerial, int maxWearCycles,
                int currentCycleCount, double cuttingEdgeSharpness);

    void incrementWear();
    void regrindCuttingEdges();
    void retireTool();
};

class MaintenanceRecord {
private:
    int recordId_;
    std::string serviceDate_;
    std::string partsReplacedList_;
    double downtimeMinutes_;
    double nextServiceHours_;

public:
    MaintenanceRecord(int recordId, const std::string& serviceDate,
                      const std::string& partsReplacedList, double downtimeMinutes,
                      double nextServiceHours);

    double calculateMaintenanceCost(double hourlyRate) const;
    bool verifyPostMaintenanceSafety() const;
};

class SensorData {
private:
    double vibrationLevel_;
    double oilPressureKpa_;
    std::string sensorTimestamp_;
    double samplingRateHz_;

public:
    SensorData(double vibrationLevel, double oilPressureKpa,
               const std::string& sensorTimestamp, double samplingRateHz);

    void filterNoiseSpikes();
    bool detectAbnormalVibration(double limit) const;
    void compressHistoricLogs();
};

class ProductionLine {
private:
    int lineId_;
    double conveyorSpeed_;
    int activeStationsCount_;
    int shiftTargetUnits_;
    int producedUnits_;
    bool isLineStalled_;

public:
    ProductionLine(int lineId, double conveyorSpeed, int activeStationsCount,
                   int shiftTargetUnits, int producedUnits = 0, bool isLineStalled = false);

    void startShiftWorkflow(ProductionShift& shift);
    void stepConveyorBelt();
    void resolveLineBottleneck();
    double evaluateShiftCompletionPercent() const;
};

class Workshop {
private:
    int workshopNumber_;
    double floorAreaSqMeters_;
    double currentNoiseLevelDb_;
    double airVentilationRate_;

public:
    Workshop(int workshopNumber, double floorAreaSqMeters,
             double currentNoiseLevelDb, double airVentilationRate);

    void evacuatePersonnel();
    void monitorVentilation(double currentCO2);
    double calculatePowerLoad() const;
};