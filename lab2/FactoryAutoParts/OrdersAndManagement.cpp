#include "OrdersAndManagement.hpp"
#include "MaterialAndDetails.hpp"
#include "ProductionAndEquipment.hpp"
#include "QualityControl.hpp"
#include "WarehouseAndLogistics.hpp"

std::string Employee::registerTurnstileEntry() const {
    if (!safetyBriefingPassed_) {
        throw UnauthorizedOperationException("Employee " + badgeId_ + " has not passed safety briefing.");
    }
    return "Entry registered: " + badgeId_ + " (" + fullName_ + ")";
}

bool Employee::passMandatorySafetyTest() {
    safetyBriefingPassed_ = true;
    return safetyBriefingPassed_;
}

double Employee::calculateMonthlyGrossSalary() const {
    return baseHourlyWage_ * hoursWorkedThisMonth_;
}


void MachineOperator::setupMachineTooling(Machine& machine) {
    if (!safetyBriefingPassed_ || !safetyGogglesWorn_) {
        throw UnauthorizedOperationException("Operator must pass safety briefing and wear safety goggles.");
    }
    if (operatorSkillGrade_ < 3) {
        throw UnauthorizedOperationException("Operator skill grade is too low for tooling setup.");
    }
    if (!machine.isOperational() || assignedMachineType_.empty()) {
        throw PlantException("Machine is not operational or machine type is unassigned.");
    }
}

void MachineOperator::executeSafetySelfCheck() {
    if (!safetyGogglesWorn_) {
        safetyGogglesWorn_ = true;
    }
    if (!safetyBriefingPassed_) {
        throw UnauthorizedOperationException("Mandatory safety briefing not passed.");
    }
}

void MachineOperator::logProducedQuantity(int count) {
    if (count < 0) {
        throw PlantException("Produced count cannot be negative.");
    }
    shiftPartsProduced_ += count;
}


double QualityInspector::performMicrometerVerification(AutoPart& part) {
    if (certLevel_ < 1) {
        throw UnauthorizedOperationException("Inspector lacks certification for micrometer verification.");
    }
    partsInspectedCount_ += 1;
    if (!part.isFinished()) {
        rejectedPartsCount_ += 1;
        throw ToleranceViolationException("Part " + part.getPartId() + " is not finished.");
    }
    return static_cast<double>(stampSerialCode_);
}

void QualityInspector::issueRejectionTag(AutoPart& part, const std::string& cause) {
    if (cause.empty()) {
        throw PlantException("Rejection cause cannot be empty.");
    }
    partsInspectedCount_ += 1;
    rejectedPartsCount_ += 1;
    if (part.getPartId().empty()) {
        throw PlantException("Invalid part ID for rejection tag.");
    }
}

double QualityInspector::auditShiftDefectRate() {
    if (partsInspectedCount_ <= 0) {
        return 0.0;
    }
    return (static_cast<double>(rejectedPartsCount_) / static_cast<double>(partsInspectedCount_)) * 100.0;
}


void PlantEngineer::optimizeCuttingParameters(CncMachine& machine) {
    if (!certCardCamValid_ || plantAccessLevel_ < 2) {
        throw UnauthorizedOperationException("Engineer is not authorized for CAD/CAM optimization.");
    }
    if (!machine.isOperational() || engineeringSpecialty_.empty()) {
        throw PlantException("Cannot optimize non-operational CNC machine.");
    }
    activeModificationsCount_ += 1;
}

bool PlantEngineer::schedulePreventiveOverhaul(Machine& machine) {
    if (plantAccessLevel_ < 1) {
        throw UnauthorizedOperationException("Insufficient access level to schedule overhaul.");
    }
    machine.serviceResetOperatingHours();
    return true;
}

void PlantEngineer::investigateProductionIncident(IncidentReport& incident) {
    (void)incident; 
    if (!safetyBriefingPassed_ || plantAccessLevel_ < 2) {
        throw UnauthorizedOperationException("Engineer lacks clearance to investigate incidents.");
    }
    activeModificationsCount_ += 1;
}


bool ShiftSupervisor::conductPreShiftSafetyMeeting() {
    if (currentTeamHeadCount_ <= 0) {
        throw ShiftNotStaffedException("No workers present in sector " + std::to_string(supervisionSector_));
    }
    safetyBriefingPassed_ = true;
    return true;
}

void ShiftSupervisor::reassignWorkerStation(Employee& employee, int station) {
    if (station <= 0) {
        throw PlantException("Invalid station number.");
    }
    employee.passMandatorySafetyTest();
}

void ShiftSupervisor::haltSectorDueToHazard() {
    safetyViolationsLogged_ += 1;
    shiftPlanPercentage_ = 0.0;
    isShiftDebriefDone_ = false;
}

bool ShiftSupervisor::signShiftHandoverSheet() {
    if (shiftPlanPercentage_ < 50.0 && safetyViolationsLogged_ > 5) {
        throw PlantException("Cannot sign handover due to critical unresolved shift issues.");
    }
    isShiftDebriefDone_ = true;
    return isShiftDebriefDone_;
}


void WarehouseWorker::scanItemBarcode(StockItem& item) {
    if (scangunId_ <= 0 || shiftZoneAssigned_.empty()) {
        throw UnauthorizedOperationException("Warehouse worker has no valid scangun or assigned zone.");
    }
    item.reserveStock(1);
    itemPickedPerHour_ += 1.0;
}

void WarehouseWorker::packCrateForAssembly() {
    if (!safetyBriefingPassed_) {
        throw UnauthorizedOperationException("Safety briefing required before packing crates.");
    }
    itemPickedPerHour_ += 5.0;
}

bool WarehouseWorker::reportDamagedPackaging(StockItem& item) {
    if (!forkliftLicenseValid_) {
        return item.flagSlowMovingStock(30);
    }
    return item.flagSlowMovingStock(15);
}


void ProductionShift::validateMinimumStaffing(int requiredCrew) {
    if (requiredCrew <= 0) {
        throw PlantException("Required crew count must be positive.");
    }
    if (requiredCrew > 50) {
        throw ShiftNotStaffedException("Shift " + std::to_string(shiftIdentifier_) + " is understaffed.");
    }
}

bool ProductionShift::computeShiftOvertimeHours() {
    if (shiftStartTime_.empty()) {
        throw PlantException("Shift start time is not set.");
    }
    return durationHours_ > 8 && nightShiftBonusApplicable_ >= 0.0;
}

void ProductionShift::logShiftDowntime(int minutes) {
    if (minutes < 0) {
        throw PlantException("Downtime minutes cannot be negative.");
    }
    if (minutes >= durationHours_ * 60) {
        throw PlantException("Downtime exceeds total shift duration.");
    }
}

void ProductionShift::closeShiftAccounting() {
    isHandoverComplete_ = true;
}


double ManufacturingOrder::calculateRequiredWorkHours(double initCycleTime) {
    if (initCycleTime <= 0.0) {
        throw PlantException("Cycle time must be positive.");
    }
    int remainingParts = (targetQuantityPcs_ + scrapSurplusMargin_) - currentCompletedCount_;
    if (remainingParts <= 0) {
        return 0.0;
    }
    return remainingParts * initCycleTime;
}

void ManufacturingOrder::allocateLineResources(ProductionLine& line) {
    if (partTypeRequested_.empty() || workOrderNumber_ <= 0) {
        throw PlantException("Invalid manufacturing order configuration.");
    }
    line.resolveLineBottleneck();
}

void ManufacturingOrder::incrementCompletedBatch(int batchSize) {
    if (batchSize <= 0) {
        throw PlantException("Batch size must be positive.");
    }
    currentCompletedCount_ += batchSize;
}

void ManufacturingOrder::checkDeadlineRisk(int remainingDays) {
    if (deadlineDate_.empty()) {
        throw PlantException("Deadline date is not specified.");
    }
    int remainingParts = targetQuantityPcs_ - currentCompletedCount_;
    if (remainingDays <= 0 && remainingParts > 0) {
        throw OrderDeadlineExceededException("Order #" + std::to_string(workOrderNumber_) + " missed deadline " + deadlineDate_);
    }
}


bool Client::verifyCreditCapacity(double orderSum) {
    if (orderSum < 0.0) {
        throw PlantException("Order sum cannot be negative.");
    }
    return orderSum <= creditLimitRub_ && paymentTermsDays_ > 0;
}

double Client::applyVolumeDiscount(int partsQuantity) {
    if (partsQuantity < 0) {
        throw PlantException("Parts quantity cannot be negative.");
    }
    if (partsQuantity >= 1000) {
        return 15.0;
    } else if (partsQuantity >= 500) {
        return 10.0;
    } else if (partsQuantity >= 100) {
        return 5.0;
    }
    return 0.0;
}

void Client::updateContractTier(const std::string& tier) {
    if (tier.empty() || clientEnterpriseCode_ <= 0 || corporateName_.empty()) {
        throw PlantException("Invalid client data or contract tier.");
    }
    if (tier == "VIP" || tier == "Gold") {
        creditLimitRub_ *= 1.5;
        paymentTermsDays_ += 15;
    }
}


void ClientContract::activateContract() {
    if (contractDocNumber_ <= 0 || effectiveDate_.empty() || totalContractValue_ <= 0.0) {
        throw PlantException("Cannot activate contract with invalid parameters.");
    }
    isSignedByBothParties_ = true;
}

double ClientContract::computeDelayDamages(int daysLate) {
    if (daysLate < 0) {
        throw PlantException("Days late cannot be negative.");
    }
    if (!isSignedByBothParties_) {
        return 0.0;
    }
    return totalContractValue_ * (penaltyRatePerDay_ / 100.0) * daysLate;
}

void ClientContract::terminateContractDueToDefault() {
    if (!isSignedByBothParties_) {
        throw PlantException("Contract is not active.");
    }
    isSignedByBothParties_ = false;
    totalContractValue_ = 0.0;
}


double PlantFinance::disbursePayrollFunds(double totalSalaries) {
    if (totalSalaries < 0.0) {
        throw PlantException("Total salaries cannot be negative.");
    }
    if (totalSalaries > payrollAccountBalance_) {
        throw PlantException("Insufficient funds in payroll account.");
    }
    payrollAccountBalance_ -= totalSalaries;
    return payrollAccountBalance_;
}

double PlantFinance::paySupplyInvoice(SupplyOrder& order) {
    order.authorizePurchaseBudget();
    double penalty = order.calculateLatePenalties(0);
    double invoiceCost = 1000.0 - penalty;
    if (operationalReserveRub_ < invoiceCost) {
        throw PlantException("Insufficient operational reserve to pay supply invoice.");
    }
    operationalReserveRub_ -= invoiceCost;
    return operationalReserveRub_;
}

double PlantFinance::receiveClientPayment(ShipmentOrder& order) {
    if (!order.validateLotBarcodes()) {
        throw PlantException("Cannot receive payment for unvalidated shipment.");
    }
    double payment = 5000.0;
    operationalReserveRub_ += payment;
    quarterlyEbitda_ += payment * 0.2;
    return operationalReserveRub_;
}

double PlantFinance::calculateProfitMargin(double revenue, double directCosts) {
    if (revenue <= 0.0) {
        throw PlantException("Revenue must be positive to calculate profit margin.");
    }
    double totalCosts = directCosts + electricityMonthlyBudget_;
    return ((revenue - totalCosts) / revenue) * 100.0;
}