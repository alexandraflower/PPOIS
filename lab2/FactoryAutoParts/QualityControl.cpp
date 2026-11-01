#include "QualityControl.hpp"
#include "MaterialAndDetails.hpp"
#include <cmath>


ToleranceLimit::ToleranceLimit(double nominalSizeMm, double upperDeviationMm,
                               double lowerDeviationMm, double surfaceRoughnessRa)
    : nominalSizeMm_(nominalSizeMm), upperDeviationMm_(upperDeviationMm),
    lowerDeviationMm_(lowerDeviationMm), surfaceRoughnessRa_(surfaceRoughnessRa) {
    if (nominalSizeMm_ <= 0.0 || upperDeviationMm_ < 0.0 || lowerDeviationMm_ < 0.0 || surfaceRoughnessRa_ <= 0.0) {
        throw PlantException("Invalid tolerance limit parameters.");
    }
}

bool ToleranceLimit::isWithinTolerance(double value) const {
    double minAllowed = nominalSizeMm_ - lowerDeviationMm_;
    double maxAllowed = nominalSizeMm_ + upperDeviationMm_;
    return value >= minAllowed && value <= maxAllowed;
}

void ToleranceLimit::tightenToleranceBand(double safetyFactor) {
    if (safetyFactor <= 0.0 || safetyFactor >= 1.0) {
        throw PlantException("Safety factor must be between 0 and 1.");
    }
    upperDeviationMm_ *= safetyFactor;
    lowerDeviationMm_ *= safetyFactor;
    surfaceRoughnessRa_ *= safetyFactor;
}


QualityStandard::QualityStandard(const std::string& standardCode, int issueYear,
                                 int samplingFrequencyPcs, int auditFrequencyMonths)
    : standardCode_(standardCode), issueYear_(issueYear),
    samplingFrequencyPcs_(samplingFrequencyPcs), auditFrequencyMonths_(auditFrequencyMonths) {
    if (standardCode_.empty() || issueYear_ < 1900 || samplingFrequencyPcs_ <= 0 || auditFrequencyMonths_ <= 0) {
        throw PlantException("Invalid quality standard parameters.");
    }
}

bool QualityStandard::auditProductionProcess() const {
    return !standardCode_.empty() && samplingFrequencyPcs_ > 0 && auditFrequencyMonths_ <= 12;
}

bool QualityStandard::isStandardActive(int currentYear) const {
    if (currentYear < issueYear_) {
        throw PlantException("Current year cannot be earlier than standard issue year.");
    }
    return (currentYear - issueYear_) <= 10;
}


DefectReport::DefectReport(int reportId, const std::string& defectTypeCategory,
                           double financialLoss, const std::string& rootCauseAnalysis,
                           bool isCriticalDefect)
    : reportId_(reportId), defectTypeCategory_(defectTypeCategory),
    financialLoss_(financialLoss), rootCauseAnalysis_(rootCauseAnalysis),
    isCriticalDefect_(isCriticalDefect) {
    if (reportId_ <= 0 || defectTypeCategory_.empty() || financialLoss_ < 0.0) {
        throw PlantException("Invalid defect report parameters.");
    }
}

double DefectReport::calculateScrapLoss(const AutoPart& part) {
    financialLoss_ += part.getManufacturingCost();
    return financialLoss_;
}

void DefectReport::routeToEngineeringReview() {
    if (rootCauseAnalysis_.empty()) {
        rootCauseAnalysis_ = "Pending engineering investigation for report #" + std::to_string(reportId_);
    }
    isCriticalDefect_ = true;
}


ComplianceCertificate::ComplianceCertificate(const std::string& certificateNumber, const std::string& issueDate,
                                             const std::string& leadAuditorSignature, int validityDurationDays)
    : certificateNumber_(certificateNumber), issueDate_(issueDate),
    leadAuditorSignature_(leadAuditorSignature), validityDurationDays_(validityDurationDays) {
    if (certificateNumber_.empty() || issueDate_.empty() || leadAuditorSignature_.empty() || validityDurationDays_ <= 0) {
        throw PlantException("Invalid compliance certificate parameters.");
    }
}

void ComplianceCertificate::bindToShipmentLot(const std::string& batchCode) {
    if (batchCode.empty()) {
        throw PlantException("Batch code cannot be empty when binding certificate.");
    }
    if (validityDurationDays_ <= 0) {
        throw PlantException("Cannot bind expired certificate " + certificateNumber_);
    }
    certificateNumber_ += "-" + batchCode;
}

void ComplianceCertificate::revokeCertificate(const std::string& fraudReason) {
    if (fraudReason.empty()) {
        throw PlantException("Revocation reason must be specified.");
    }
    validityDurationDays_ = 0;
}


DimensionScanner::DimensionScanner(double laserAccuracyMicrons, double actualLengthMm,
                                   double actualWidthMm, double actualHeightMm)
    : laserAccuracyMicrons_(laserAccuracyMicrons), actualLengthMm_(actualLengthMm),
    actualWidthMm_(actualWidthMm), actualHeightMm_(actualHeightMm) {
    if (laserAccuracyMicrons_ <= 0.0 || actualLengthMm_ <= 0.0 || actualWidthMm_ <= 0.0 || actualHeightMm_ <= 0.0) {
        throw PlantException("Invalid dimension scanner parameters.");
    }
}

void DimensionScanner::scanGeometryPointcloud(const AutoPart& part) {
    if (part.getPartId().empty()) {
        throw PlantException("Cannot scan part with empty ID.");
    }
    if (laserAccuracyMicrons_ > 50.0) {
        throw ToleranceViolationException("Scanner accuracy is too low for pointcloud measurement.");
    }
    actualHeightMm_ += 0.001;
}

double DimensionScanner::calculateDeviationFromCad(const ToleranceLimit& limit) const {
    double avgDimension = (actualLengthMm_ + actualWidthMm_) * 0.5;
    if (!limit.isWithinTolerance(actualLengthMm_)) {
        return std::abs(actualLengthMm_ - avgDimension) + (laserAccuracyMicrons_ * 0.001);
    }
    return 0.0;
}

void DimensionScanner::recalibrateLaserOpticalHead() {
    laserAccuracyMicrons_ = 1.0;
}


LabStressTest::LabStressTest(double appliedPressureMpa, int cycleLimitTarget,
                             int reachedCyclesCount, bool ruptureDetected)
    : appliedPressureMpa_(appliedPressureMpa), cycleLimitTarget_(cycleLimitTarget),
    reachedCyclesCount_(reachedCyclesCount), ruptureDetected_(ruptureDetected) {
    if (appliedPressureMpa_ < 0.0 || cycleLimitTarget_ <= 0 || reachedCyclesCount_ < 0) {
        throw PlantException("Invalid lab stress test parameters.");
    }
}

void LabStressTest::applyFatiguePressureCycle() {
    if (ruptureDetected_) {
        throw DefectiveBatchException("Sample already ruptured during stress test.");
    }
    reachedCyclesCount_ += 100;
    if (appliedPressureMpa_ > 500.0 && reachedCyclesCount_ >= cycleLimitTarget_) {
        ruptureDetected_ = true;
    }
}

bool LabStressTest::detectMicroCracksAcoustically() const {
    return ruptureDetected_ || (appliedPressureMpa_ > 350.0 && reachedCyclesCount_ > cycleLimitTarget_ / 2);
}

void LabStressTest::recordDestructionThreshold(double pressure) {
    if (pressure <= 0.0) {
        throw PlantException("Destruction pressure must be positive.");
    }
    appliedPressureMpa_ = pressure;
    ruptureDetected_ = true;
}


ScrapBin::ScrapBin(double binCapacityKg, double currentScrapWeightKg,
                   const std::string& metalTypeSorted, bool isFullAlert)
    : binCapacityKg_(binCapacityKg), currentScrapWeightKg_(currentScrapWeightKg),
    metalTypeSorted_(metalTypeSorted), isFullAlert_(isFullAlert) {
    if (binCapacityKg_ <= 0.0 || currentScrapWeightKg_ < 0.0 || metalTypeSorted_.empty()) {
        throw PlantException("Invalid scrap bin parameters.");
    }
}

void ScrapBin::depositScrappedPart(const AutoPart& part) {
    if (isFullAlert_ || currentScrapWeightKg_ >= binCapacityKg_) {
        isFullAlert_ = true;
        throw StorageCellFullException("Scrap bin for " + metalTypeSorted_ + " is full.");
    }
    double estimatedWeight = part.getManufacturingCost() * 0.05;
    currentScrapWeightKg_ += (estimatedWeight > 0.5 ? estimatedWeight : 1.0);
    if (currentScrapWeightKg_ >= binCapacityKg_) {
        isFullAlert_ = true;
    }
}

void ScrapBin::compactScrapWithPress() {
    currentScrapWeightKg_ *= 0.85;
    if (currentScrapWeightKg_ < binCapacityKg_) {
        isFullAlert_ = false;
    }
}

void ScrapBin::emptyBinForRecycling() {
    currentScrapWeightKg_ = 0.0;
    isFullAlert_ = false;
}


QualityInspection::QualityInspection(int inspectionNumber, const std::string& inspectionTimestamp,
                                     double measuredDefectSeverity, bool isApproved,
                                     bool recheckRequired)
    : inspectionNumber_(inspectionNumber), inspectionTimestamp_(inspectionTimestamp),
    measuredDefectSeverity_(measuredDefectSeverity), isApproved_(isApproved),
    recheckRequired_(recheckRequired) {
    if (inspectionNumber_ <= 0 || inspectionTimestamp_.empty() || measuredDefectSeverity_ < 0.0) {
        throw PlantException("Invalid quality inspection parameters.");
    }
}

void QualityInspection::conductVisualInspection(const AutoPart& part) {
    if (!part.isFinished()) {
        recheckRequired_ = true;
        isApproved_ = false;
    } else if (measuredDefectSeverity_ == 0.0) {
        isApproved_ = true;
        recheckRequired_ = false;
    }
}

bool QualityInspection::validateAgainstSpec(const AutoPart& part, const ToleranceLimit& limit) {
    if (!part.isFinished() || !limit.isWithinTolerance(measuredDefectSeverity_)) {
        flagForScrap("Tolerance or readiness violation during inspection #" + std::to_string(inspectionNumber_));
        throw ToleranceViolationException("Part " + part.getPartId() + " failed specification check.");
    }
    stampApprovalMark();
    return isApproved_;
}

void QualityInspection::stampApprovalMark() {
    isApproved_ = true;
    recheckRequired_ = false;
}

void QualityInspection::flagForScrap(const std::string& cause) {
    if (cause.empty()) {
        throw PlantException("Scrap cause cannot be empty.");
    }
    isApproved_ = false;
    recheckRequired_ = false;
    measuredDefectSeverity_ += 10.0;
}