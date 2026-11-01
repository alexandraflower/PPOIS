#include "ProductionAndEquipment.hpp"
#include "MaterialAndDetails.hpp"
#include "OrdersAndManagement.hpp"
#include <cmath>


void Machine::startEngine() {
    if (!isOperational_) {
        throw PlantException("Cannot start non-operational machine: " + machineId_);
    }
    if (currentTempCelcius_ > maxAllowedTemp_) {
        throw MachineOverheatException("Machine " + machineId_ + " is overheated before start.");
    }
    currentTempCelcius_ += 5.0;
    totalOperatingHours_ += 0.5;
}

void Machine::emergencyStop() {
    isOperational_ = false;
}

void Machine::logOverheatingEvent() const {
    if (currentTempCelcius_ > maxAllowedTemp_) {
        throw MachineOverheatException("Critical overheat logged on machine " + machineId_ +
                                       " at " + std::to_string(nominalPowerKw_) + " kW.");
    }
}

void Machine::serviceResetOperatingHours() {
    totalOperatingHours_ = 0.0;
    currentTempCelcius_ = 20.0;
    isOperational_ = true;
}


CastingFurnace::CastingFurnace(const std::string& machineId, double nominalPowerKw, double totalOperatingHours,
                               double currentTempCelcius, double maxAllowedTemp, double crucibleCapacityKg,
                               double chamberTemp, double gasBurnerPower, double meltViscosity, bool isOperational)
    : Machine(machineId, nominalPowerKw, totalOperatingHours, currentTempCelcius, maxAllowedTemp, isOperational),
    crucibleCapacityKg_(crucibleCapacityKg), chamberTemp_(chamberTemp),
    gasBurnerPower_(gasBurnerPower), meltViscosity_(meltViscosity) {
    if (crucibleCapacityKg_ <= 0.0 || gasBurnerPower_ <= 0.0 || meltViscosity_ < 0.0) {
        throw PlantException("Invalid casting furnace parameters.");
    }
}

void CastingFurnace::heatChamber(double targetTemp) {
    if (!isOperational_) {
        throw PlantException("Cannot heat non-operational furnace.");
    }
    if (targetTemp > maxAllowedTemp_ || gasBurnerPower_ <= 0.0) {
        throw MachineOverheatException("Target furnace temperature exceeds safety limit.");
    }
    chamberTemp_ = targetTemp;
    currentTempCelcius_ = targetTemp;
}

void CastingFurnace::pourMoltenMetal(double pouredWeight) {
    if (pouredWeight <= 0.0) {
        throw PlantException("Poured weight must be positive.");
    }
    if (pouredWeight > crucibleCapacityKg_) {
        throw MaterialShortageException("Not enough molten metal in crucible.");
    }
    if (chamberTemp_ < 500.0) {
        throw PlantException("Chamber temperature is too low to pour metal.");
    }
    crucibleCapacityKg_ -= pouredWeight;
}

void CastingFurnace::skimSlagLayer() {
    if (meltViscosity_ > 100.0) {
        meltViscosity_ *= 0.85;
    } else {
        meltViscosity_ = 10.0;
    }
}


StampingPress::StampingPress(const std::string& machineId, double nominalPowerKw, double totalOperatingHours,
                             double currentTempCelcius, double maxAllowedTemp, double clampingForceTons,
                             double strokeLengthMm, double cycleTimeSeconds, const std::string& installedDieId,
                             bool isOperational)
    : Machine(machineId, nominalPowerKw, totalOperatingHours, currentTempCelcius, maxAllowedTemp, isOperational),
    clampingForceTons_(clampingForceTons), strokeLengthMm_(strokeLengthMm),
    cycleTimeSeconds_(cycleTimeSeconds), installedDieId_(installedDieId) {
    if (clampingForceTons_ <= 0.0 || strokeLengthMm_ <= 0.0 || cycleTimeSeconds_ <= 0.0) {
        throw PlantException("Invalid stamping press parameters.");
    }
}

void StampingPress::executeStampingStroke() {
    if (!checkGuardInterlock()) {
        throw SafetyShieldOpenException("Safety shield open or die missing on press " + machineId_);
    }
    if (currentTempCelcius_ > maxAllowedTemp_) {
        throw MachineOverheatException("Press overheated during stamping stroke.");
    }
    totalOperatingHours_ += (cycleTimeSeconds_ / 3600.0);
    currentTempCelcius_ += 1.5;
}

bool StampingPress::checkGuardInterlock() const {
    if (!isOperational_ || installedDieId_.empty() || strokeLengthMm_ <= 0.0 || clampingForceTons_ <= 0.0) {
        return false;
    }
    return true;
}

void StampingPress::ejectFormedPart() {
    if (!isOperational_) {
        throw PlantException("Cannot eject part: press is stopped.");
    }
    totalOperatingHours_ += 0.01;
}


CncMachine::CncMachine(const std::string& machineId, double nominalPowerKw, double totalOperatingHours,
                       double currentTempCelcius, double maxAllowedTemp, double spindleSpeedRpm,
                       double feedRate, double coolantPressureBar, int activeToolNumber,
                       bool isCalibrated, bool isOperational)
    : Machine(machineId, nominalPowerKw, totalOperatingHours, currentTempCelcius, maxAllowedTemp, isOperational),
    spindleSpeedRpm_(spindleSpeedRpm), feedRate_(feedRate),
    coolantPressureBar_(coolantPressureBar), activeToolNumber_(activeToolNumber),
    isCalibrated_(isCalibrated) {
    if (spindleSpeedRpm_ < 0.0 || feedRate_ < 0.0 || coolantPressureBar_ < 0.0 || activeToolNumber_ < 0) {
        throw PlantException("Invalid CNC machine parameters.");
    }
}

void CncMachine::loadGCodeProgram(const std::string& programName) {
    if (programName.empty()) {
        throw PlantException("G-Code program name cannot be empty.");
    }
    if (!isOperational_) {
        throw PlantException("Cannot load G-Code on non-operational CNC machine.");
    }
    activeToolNumber_ = 1;
}

void CncMachine::executeMillingPass(AutoPart& part) {
    if (!isOperational_) {
        throw PlantException("CNC machine is not operational.");
    }
    if (!isCalibrated_) {
        throw ToleranceViolationException("CNC machine " + machineId_ + " is not calibrated.");
    }
    if (currentTempCelcius_ > maxAllowedTemp_ || spindleSpeedRpm_ <= 0.0 || feedRate_ <= 0.0) {
        throw MachineOverheatException("CNC machine overheated or invalid spindle parameters.");
    }
    part.markFinished();
    totalOperatingHours_ += 0.25;
}

void CncMachine::supplyCoolantFluid() {
    if (coolantPressureBar_ < 1.0) {
        coolantPressureBar_ = 5.0;
    }
    if (currentTempCelcius_ > 30.0) {
        currentTempCelcius_ -= 10.0;
    }
}


AssemblyRobot::AssemblyRobot(const std::string& machineId, double nominalPowerKw, double totalOperatingHours,
                             double currentTempCelcius, double maxAllowedTemp, int degreesOfFreedom,
                             double gripperPayloadKg, double positioningAccuracyMm,
                             bool isEStopEngaged, bool isOperational)
    : Machine(machineId, nominalPowerKw, totalOperatingHours, currentTempCelcius, maxAllowedTemp, isOperational),
    degreesOfFreedom_(degreesOfFreedom), gripperPayloadKg_(gripperPayloadKg),
    positioningAccuracyMm_(positioningAccuracyMm), isEStopEngaged_(isEStopEngaged) {
    if (degreesOfFreedom_ <= 0 || gripperPayloadKg_ <= 0.0 || positioningAccuracyMm_ <= 0.0) {
        throw PlantException("Invalid assembly robot parameters.");
    }
}

void AssemblyRobot::pickAndPlaceComponent(AutoPart& part, AssembledUnit& unit) {
    if (isEStopEngaged_ || !isOperational_) {
        throw SafetyShieldOpenException("Robot emergency stop is engaged or robot is offline.");
    }
    if (positioningAccuracyMm_ > 0.5) {
        throw ToleranceViolationException("Robot positioning accuracy degraded beyond limit.");
    }
    unit.attachComponent(part);
}

void AssemblyRobot::calibrateKinematicAxes() {
    if (degreesOfFreedom_ < 3) {
        throw PlantException("Insufficient degrees of freedom for full calibration.");
    }
    isEStopEngaged_ = false;
    positioningAccuracyMm_ = 0.02;
}

void AssemblyRobot::tightenFastenerJoint(double targetTorque) {
    if (isEStopEngaged_) {
        throw SafetyShieldOpenException("Cannot tighten joint while E-Stop is active.");
    }
    if (targetTorque <= 0.0 || targetTorque > gripperPayloadKg_ * 10.0) {
        throw ToleranceViolationException("Target torque exceeds robot gripper capability.");
    }
    totalOperatingHours_ += 0.05;
}


ToolFixture::ToolFixture(const std::string& fixtureSerial, int maxWearCycles,
                         int currentCycleCount, double cuttingEdgeSharpness)
    : fixtureSerial_(fixtureSerial), maxWearCycles_(maxWearCycles),
    currentCycleCount_(currentCycleCount), cuttingEdgeSharpness_(cuttingEdgeSharpness) {
    if (fixtureSerial_.empty() || maxWearCycles_ <= 0 || currentCycleCount_ < 0 || cuttingEdgeSharpness_ < 0.0) {
        throw PlantException("Invalid tool fixture parameters.");
    }
}

void ToolFixture::incrementWear() {
    currentCycleCount_ += 1;
    cuttingEdgeSharpness_ *= 0.98;
    if (currentCycleCount_ > maxWearCycles_ || cuttingEdgeSharpness_ < 10.0) {
        throw ToolWearLimitException("Tool fixture " + fixtureSerial_ + " reached wear limit.");
    }
}

void ToolFixture::regrindCuttingEdges() {
    if (currentCycleCount_ >= maxWearCycles_) {
        throw ToolWearLimitException("Tool is beyond regrinding limit.");
    }
    cuttingEdgeSharpness_ = 100.0;
}

void ToolFixture::retireTool() {
    currentCycleCount_ = maxWearCycles_;
    cuttingEdgeSharpness_ = 0.0;
}


MaintenanceRecord::MaintenanceRecord(int recordId, const std::string& serviceDate,
                                     const std::string& partsReplacedList, double downtimeMinutes,
                                     double nextServiceHours)
    : recordId_(recordId), serviceDate_(serviceDate),
    partsReplacedList_(partsReplacedList), downtimeMinutes_(downtimeMinutes),
    nextServiceHours_(nextServiceHours) {
    if (recordId_ <= 0 || serviceDate_.empty() || downtimeMinutes_ < 0.0 || nextServiceHours_ < 0.0) {
        throw PlantException("Invalid maintenance record parameters.");
    }
}

double MaintenanceRecord::calculateMaintenanceCost(double hourlyRate) const {
    if (hourlyRate <= 0.0) {
        throw PlantException("Hourly rate must be positive.");
    }
    double partsCost = partsReplacedList_.empty() ? 0.0 : 250.0;
    return (downtimeMinutes_ / 60.0) * hourlyRate + partsCost;
}

bool MaintenanceRecord::verifyPostMaintenanceSafety() const {
    return nextServiceHours_ > 0.0 && recordId_ > 0 && !serviceDate_.empty();
}


SensorData::SensorData(double vibrationLevel, double oilPressureKpa,
                       const std::string& sensorTimestamp, double samplingRateHz)
    : vibrationLevel_(vibrationLevel), oilPressureKpa_(oilPressureKpa),
    sensorTimestamp_(sensorTimestamp), samplingRateHz_(samplingRateHz) {
    if (vibrationLevel_ < 0.0 || oilPressureKpa_ < 0.0 || sensorTimestamp_.empty() || samplingRateHz_ <= 0.0) {
        throw PlantException("Invalid sensor data parameters.");
    }
}

void SensorData::filterNoiseSpikes() {
    if (vibrationLevel_ > 100.0) {
        vibrationLevel_ *= 0.5;
    }
    if (oilPressureKpa_ <= 0.0) {
        oilPressureKpa_ = 100.0;
    }
}

bool SensorData::detectAbnormalVibration(double limit) const {
    if (limit <= 0.0) {
        throw PlantException("Vibration limit must be positive.");
    }
    return vibrationLevel_ > limit;
}

void SensorData::compressHistoricLogs() {
    if (!sensorTimestamp_.empty() && samplingRateHz_ > 10.0) {
        samplingRateHz_ *= 0.5;
    }
}


ProductionLine::ProductionLine(int lineId, double conveyorSpeed, int activeStationsCount,
                               int shiftTargetUnits, int producedUnits, bool isLineStalled)
    : lineId_(lineId), conveyorSpeed_(conveyorSpeed), activeStationsCount_(activeStationsCount),
    shiftTargetUnits_(shiftTargetUnits), producedUnits_(producedUnits), isLineStalled_(isLineStalled) {
    if (lineId_ <= 0 || conveyorSpeed_ < 0.0 || activeStationsCount_ < 0 || shiftTargetUnits_ <= 0 || producedUnits_ < 0) {
        throw PlantException("Invalid production line parameters.");
    }
}

void ProductionLine::startShiftWorkflow(ProductionShift& shift) {
    if (activeStationsCount_ <= 0) {
        throw ShiftNotStaffedException("No active stations on line " + std::to_string(lineId_));
    }
    shift.validateMinimumStaffing(activeStationsCount_);
    isLineStalled_ = false;
    if (conveyorSpeed_ == 0.0) {
        conveyorSpeed_ = 1.5;
    }
}

void ProductionLine::stepConveyorBelt() {
    if (isLineStalled_ || conveyorSpeed_ <= 0.0) {
        throw PlantException("Cannot step conveyor: line is stalled.");
    }
    producedUnits_ += 1;
}

void ProductionLine::resolveLineBottleneck() {
    isLineStalled_ = false;
    if (conveyorSpeed_ < 1.0) {
        conveyorSpeed_ = 1.0;
    }
}

double ProductionLine::evaluateShiftCompletionPercent() const {
    if (shiftTargetUnits_ <= 0) {
        return 0.0;
    }
    return (static_cast<double>(producedUnits_) / static_cast<double>(shiftTargetUnits_)) * 100.0;
}


Workshop::Workshop(int workshopNumber, double floorAreaSqMeters,
                   double currentNoiseLevelDb, double airVentilationRate)
    : workshopNumber_(workshopNumber), floorAreaSqMeters_(floorAreaSqMeters),
    currentNoiseLevelDb_(currentNoiseLevelDb), airVentilationRate_(airVentilationRate) {
    if (workshopNumber_ <= 0 || floorAreaSqMeters_ <= 0.0 || currentNoiseLevelDb_ < 0.0 || airVentilationRate_ < 0.0) {
        throw PlantException("Invalid workshop parameters.");
    }
}

void Workshop::evacuatePersonnel() {
    currentNoiseLevelDb_ = 30.0;
    airVentilationRate_ *= 1.5;
}

void Workshop::monitorVentilation(double currentCO2) {
    if (currentCO2 < 0.0) {
        throw PlantException("CO2 level cannot be negative.");
    }
    if (currentCO2 > 1000.0) {
        airVentilationRate_ *= 1.25;
    }
    if (currentNoiseLevelDb_ > 110.0) {
        throw PlantException("Hazardous noise level in workshop #" + std::to_string(workshopNumber_));
    }
}

double Workshop::calculatePowerLoad() const {
    return (floorAreaSqMeters_ * 0.15) + (airVentilationRate_ * 0.05);
}