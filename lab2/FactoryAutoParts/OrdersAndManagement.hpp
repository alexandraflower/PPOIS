#pragma once
#include "Exceptions.hpp"
#include <string>

class Machine;
class AutoPart;
class CncMachine;
class IncidentReport;
class StockItem;
class ProductionLine;
class SupplyOrder;
class ShipmentOrder;

class Employee{
protected:
    std::string badgeId_;
    std::string fullName_;
    double baseHourlyWage_;
    double hoursWorkedThisMonth_;
    bool safetyBriefingPassed_;
public:
    Employee(const std::string& badgeId, const std::string& fullName, double baseHourlyWage,
    double hoursWorkedThisMonth, bool safetyBriefingPassed = false ) : badgeId_(badgeId),
    fullName_(fullName), baseHourlyWage_(baseHourlyWage), hoursWorkedThisMonth_(hoursWorkedThisMonth),
    safetyBriefingPassed_(safetyBriefingPassed){
        if (baseHourlyWage<=0.0 || hoursWorkedThisMonth < 0.0){
            throw PlantException("Invalid employee parameters.");
        }
    }
    virtual ~Employee() = default;

    std::string registerTurnstileEntry() const;
    bool passMandatorySafetyTest();
    double calculateMonthlyGrossSalary() const;
};

class MachineOperator : public Employee {
private:
    int operatorSkillGrade_;
    std::string assignedMachineType_;
    int shiftPartsProduced_;
    bool safetyGogglesWorn_;
public:
    MachineOperator(const std::string& badgeId, const std::string& fullName, double baseHourlyWage,
    double hoursWorkedThisMonth, int operatorSkillGrade,  const std::string& assignedMachineType, 
    int shiftPartsProduced, bool safetyGogglesWorn, bool safetyBriefingPassed = false) : 
    Employee(badgeId, fullName, baseHourlyWage, hoursWorkedThisMonth, safetyBriefingPassed),
    operatorSkillGrade_(operatorSkillGrade), assignedMachineType_(assignedMachineType),
    shiftPartsProduced_(shiftPartsProduced), safetyGogglesWorn_(safetyGogglesWorn){}
    void setupMachineTooling(Machine &machine);
    void executeSafetySelfCheck();
    void logProducedQuantity(int count);
};

class QualityInspector : public Employee {
private:
    int certLevel_;
    int stampSerialCode_;
    int partsInspectedCount_;
    int rejectedPartsCount_;
public:
    QualityInspector(const std::string& badgeId, const std::string& fullName, double baseHourlyWage,
    double hoursWorkedThisMonth, int certLevel, int stampSerialCode, int partsInspectedCount, 
    int rejectedPartsCount, bool safetyBriefingPassed = false) : 
    Employee(badgeId, fullName, baseHourlyWage, hoursWorkedThisMonth, safetyBriefingPassed), 
    certLevel_(certLevel), stampSerialCode_(stampSerialCode),
    partsInspectedCount_(partsInspectedCount), rejectedPartsCount_(rejectedPartsCount){}
    double performMicrometerVerification(AutoPart& part);
    void issueRejectionTag(AutoPart& part, const std::string& cause);
    double auditShiftDefectRate();
};

class PlantEngineer : public Employee{
private:
    std::string engineeringSpecialty_;
    int activeModificationsCount_;
    bool certCardCamValid_;
    int plantAccessLevel_;
public:
    PlantEngineer(const std::string& badgeId, const std::string& fullName, double baseHourlyWage,
    double hoursWorkedThisMonth, const std::string& engineeringSpecialty,
    int activeModificationsCount, bool certCardCamValid, int plantAccessLevel, bool safetyBriefingPassed = false) :
    Employee(badgeId, fullName, baseHourlyWage, hoursWorkedThisMonth, safetyBriefingPassed),
    engineeringSpecialty_(engineeringSpecialty), activeModificationsCount_(activeModificationsCount),
    certCardCamValid_(certCardCamValid), plantAccessLevel_(plantAccessLevel){}
    void optimizeCuttingParameters(CncMachine& machine);
    bool schedulePreventiveOverhaul(Machine& machine);
    void investigateProductionIncident(IncidentReport& incident);
};

class ShiftSupervisor : public Employee {
private:
    int supervisionSector_;
    int currentTeamHeadCount_;
    int safetyViolationsLogged_;
    double shiftPlanPercentage_;
    bool isShiftDebriefDone_;
public:
    ShiftSupervisor(const std::string& badgeId, const std::string& fullName, double baseHourlyWage,
    double hoursWorkedThisMonth, int supervisionSector, 
    int currentTeamHeadCount, int safetyViolationsLogged, double shiftPlanPercentage,
    bool isShiftDebriefDone, bool safetyBriefingPassed = false) : Employee(badgeId, fullName, baseHourlyWage, hoursWorkedThisMonth, 
    safetyBriefingPassed), supervisionSector_(supervisionSector), currentTeamHeadCount_(currentTeamHeadCount),
    safetyViolationsLogged_(safetyViolationsLogged), shiftPlanPercentage_(shiftPlanPercentage),
    isShiftDebriefDone_(isShiftDebriefDone){}
    bool conductPreShiftSafetyMeeting();
    void reassignWorkerStation(Employee& employee, int station);
    void haltSectorDueToHazard();
    bool signShiftHandoverSheet();
};

class WarehouseWorker : public Employee {
private:
    int scangunId_;
    bool forkliftLicenseValid_;
    double itemPickedPerHour_;
    std::string shiftZoneAssigned_;
public:
    WarehouseWorker(const std::string& badgeId, const std::string& fullName, double baseHourlyWage,
    double hoursWorkedThisMonth, int scangunId, bool forkliftLicenseValid,
    double itemPickedPerHour, const std::string& shiftZoneAssigned, bool safetyBriefingPassed = false) : Employee(badgeId, fullName, baseHourlyWage, 
    hoursWorkedThisMonth, safetyBriefingPassed), scangunId_(scangunId), forkliftLicenseValid_(forkliftLicenseValid),
    itemPickedPerHour_(itemPickedPerHour), shiftZoneAssigned_(shiftZoneAssigned){}
    void scanItemBarcode(StockItem& item);
    void packCrateForAssembly();
    bool reportDamagedPackaging(StockItem& item);
};

class ProductionShift{
private:
    int shiftIdentifier_;
    std::string shiftStartTime_;
    int durationHours_;
    double nightShiftBonusApplicable_;
    bool isHandoverComplete_;
public:
    ProductionShift(int shiftIdentifier, const std::string& shiftStartTime, int durationHours, 
    double nightShiftBonusApplicable, bool isHandoverComplete) : shiftIdentifier_(shiftIdentifier),
    shiftStartTime_(shiftStartTime), durationHours_(durationHours), 
    nightShiftBonusApplicable_(nightShiftBonusApplicable), isHandoverComplete_(isHandoverComplete){}
    void validateMinimumStaffing(int requiredCrew);
    bool computeShiftOvertimeHours();
    void logShiftDowntime(int minutes);
    void closeShiftAccounting();
};

class ManufacturingOrder{
private:
    int workOrderNumber_;
    std::string partTypeRequested_;
    int targetQuantityPcs_;
    int scrapSurplusMargin_;
    std::string deadlineDate_;
    int currentCompletedCount_;
public:
    ManufacturingOrder(int workOrderNumber, const std::string& partTypeRequested, int targetQuantityPcs,
    int scrapSurplusMargin, const std::string& deadlineDate, int currentCompletedCount) : 
    workOrderNumber_(workOrderNumber), partTypeRequested_(partTypeRequested), 
    targetQuantityPcs_(targetQuantityPcs), scrapSurplusMargin_(scrapSurplusMargin),
    deadlineDate_(deadlineDate), currentCompletedCount_(currentCompletedCount){}
    double calculateRequiredWorkHours(double initCycleTime);
    void allocateLineResources(ProductionLine& line);
    void incrementCompletedBatch(int batchSize);
    void checkDeadlineRisk(int remainingDays);
};

class Client{
private:
    int clientEnterpriseCode_;
    std::string corporateName_;
    double creditLimitRub_;
    int paymentTermsDays_;
public:
    Client(int clientEnterpriseCode, const std::string& corporateName, double creditLimitRub, 
    int paymentTermsDays) : clientEnterpriseCode_(clientEnterpriseCode), corporateName_(corporateName),
    creditLimitRub_(creditLimitRub), paymentTermsDays_(paymentTermsDays){}
    bool verifyCreditCapacity(double orderSum);
    double applyVolumeDiscount(int partsQuantity);
    void updateContractTier(const std::string& tier);
};

class ClientContract{
private:
    int contractDocNumber_;
    std::string effectiveDate_;
    double totalContractValue_;
    double penaltyRatePerDay_;
    bool isSignedByBothParties_;
public:
    ClientContract(int contractDocNumber, const std::string& effectiveDate, double totalContractValue,
    double penaltyRatePerDay, bool isSignedByBothParties) : contractDocNumber_(contractDocNumber),
    effectiveDate_(effectiveDate), totalContractValue_(totalContractValue), 
    penaltyRatePerDay_(penaltyRatePerDay), isSignedByBothParties_(isSignedByBothParties){}
    void activateContract();
    double computeDelayDamages(int daysLate);
    void terminateContractDueToDefault();
};

class PlantFinance{
private:
    double operationalReserveRub_;
    double payrollAccountBalance_;
    double electricityMonthlyBudget_;
    double quarterlyEbitda_;
public:
    PlantFinance(double operationalReserveRub, double payrollAccountBalance, double electricityMonthlyBudget,
    double quarterlyEbitda) : operationalReserveRub_(operationalReserveRub), 
    payrollAccountBalance_(payrollAccountBalance), electricityMonthlyBudget_(electricityMonthlyBudget),
    quarterlyEbitda_(quarterlyEbitda){}
    double disbursePayrollFunds(double totalSalaries);
    double paySupplyInvoice(SupplyOrder& order);
    double receiveClientPayment(ShipmentOrder& order);
    double calculateProfitMargin(double revenue, double directCosts);
};