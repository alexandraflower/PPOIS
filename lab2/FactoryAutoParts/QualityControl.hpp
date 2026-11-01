#pragma once
#include "Exceptions.hpp"
#include <string>

class AutoPart;
class QualityInspector;
class RawMaterial;

class ToleranceLimit {
private:
    double nominalSizeMm_;
    double upperDeviationMm_;
    double lowerDeviationMm_;
    double surfaceRoughnessRa_;

public:
    ToleranceLimit(double nominalSizeMm, double upperDeviationMm, 
                   double lowerDeviationMm, double surfaceRoughnessRa);

    bool isWithinTolerance(double value) const;
    void tightenToleranceBand(double safetyFactor);
};

class QualityStandard {
private:
    std::string standardCode_;
    int issueYear_;
    int samplingFrequencyPcs_;
    int auditFrequencyMonths_;

public:
    QualityStandard(const std::string& standardCode, int issueYear, 
                    int samplingFrequencyPcs, int auditFrequencyMonths);

    bool auditProductionProcess() const;
    bool isStandardActive(int currentYear) const;
};

class DefectReport {
private:
    int reportId_;
    std::string defectTypeCategory_;
    double financialLoss_;
    std::string rootCauseAnalysis_;
    bool isCriticalDefect_;

public:
    DefectReport(int reportId, const std::string& defectTypeCategory, 
                 double financialLoss, const std::string& rootCauseAnalysis, 
                 bool isCriticalDefect = false);

    double calculateScrapLoss(const AutoPart& part);
    void routeToEngineeringReview();
};

class ComplianceCertificate {
private:
    std::string certificateNumber_;
    std::string issueDate_;
    std::string leadAuditorSignature_;
    int validityDurationDays_;

public:
    ComplianceCertificate(const std::string& certificateNumber, const std::string& issueDate, 
                          const std::string& leadAuditorSignature, int validityDurationDays);

    void bindToShipmentLot(const std::string& batchCode);
    void revokeCertificate(const std::string& fraudReason);
};

class DimensionScanner {
private:
    double laserAccuracyMicrons_;
    double actualLengthMm_;
    double actualWidthMm_;
    double actualHeightMm_;

public:
    DimensionScanner(double laserAccuracyMicrons, double actualLengthMm, 
                     double actualWidthMm, double actualHeightMm);

    void scanGeometryPointcloud(const AutoPart& part);
    double calculateDeviationFromCad(const ToleranceLimit& limit) const;
    void recalibrateLaserOpticalHead();
};

class LabStressTest {
private:
    double appliedPressureMpa_;
    int cycleLimitTarget_;
    int reachedCyclesCount_;
    bool ruptureDetected_;

public:
    LabStressTest(double appliedPressureMpa, int cycleLimitTarget, 
                  int reachedCyclesCount, bool ruptureDetected = false);

    void applyFatiguePressureCycle();
    bool detectMicroCracksAcoustically() const;
    void recordDestructionThreshold(double pressure);
};

class ScrapBin {
private:
    double binCapacityKg_;
    double currentScrapWeightKg_;
    std::string metalTypeSorted_;
    bool isFullAlert_;

public:
    ScrapBin(double binCapacityKg, double currentScrapWeightKg, 
             const std::string& metalTypeSorted, bool isFullAlert = false);

    void depositScrappedPart(const AutoPart& part);
    void compactScrapWithPress();
    void emptyBinForRecycling();
};

class QualityInspection {
private:
    int inspectionNumber_;
    std::string inspectionTimestamp_;
    double measuredDefectSeverity_;
    bool isApproved_;
    bool recheckRequired_;

public:
    QualityInspection(int inspectionNumber, const std::string& inspectionTimestamp, 
                     double measuredDefectSeverity, bool isApproved = false, 
                     bool recheckRequired = false);

    void conductVisualInspection(const AutoPart& part);
    bool validateAgainstSpec(const AutoPart& part, const ToleranceLimit& limit);
    void stampApprovalMark();
    void flagForScrap(const std::string& cause);
};