#pragma once
#include "Exceptions.hpp"
#include <string>

class Supplier;

class RawMaterial {
protected:
    std::string materialId_;
    double density_;
    double meltingPoint_;
    double costPerKg_;

public:
    RawMaterial(const std::string& materialId, double density, double meltingPoint, double costPerKg)
        : materialId_(materialId), density_(density), meltingPoint_(meltingPoint), costPerKg_(costPerKg) {
        if (density_ <= 0.0 || costPerKg_ < 0.0) {
            throw PlantException("Invalid material parameters.");
        }
    }
    virtual ~RawMaterial() = default;

    std::string getMaterialId() const { return materialId_; }
    double getDensity() const { return density_; }
    double getCostPerKg() const { return costPerKg_; }

    double calculateBatchCost(double weight) const;
    bool verifyMeltingSuitability(double furnaceTemp) const;
    double applyLossCoefficient(double lossPercent) const;
};

class MetalAlloy : public RawMaterial {
private:
    double carbonContent_;
    double tensileStrength_;
    bool isHeatTreated_;

public:
    MetalAlloy(const std::string& materialId, double density, double meltingPoint, double costPerKg,
               double carbonContent, double tensileStrength, bool isHeatTreated = false)
        : RawMaterial(materialId, density, meltingPoint, costPerKg),
          carbonContent_(carbonContent), tensileStrength_(tensileStrength), isHeatTreated_(isHeatTreated) {
        if (carbonContent <= 0.0 || tensileStrength <= 0.0) {
            throw PlantException("Invalid metal alloy parameters.");
        }
    }
    ~MetalAlloy() override = default;

    void conductHardeningProcess(int quenchTemp);
    bool checkWeldability() const;
};

class PlasticComposite : public RawMaterial {
private:
    std::string reinforcementFiber_;
    double glassTransitionTemp_;
    double uvResistanceIndex_;

public:
    PlasticComposite(const std::string& materialId, double density, double meltingPoint, double costPerKg,
                     const std::string& reinforcementFiber, double glassTransitionTemp, double uvResistanceIndex)
        : RawMaterial(materialId, density, meltingPoint, costPerKg),
          reinforcementFiber_(reinforcementFiber), glassTransitionTemp_(glassTransitionTemp),
          uvResistanceIndex_(uvResistanceIndex) {}
    ~PlasticComposite() override = default;

    double evaluateThermalDeformation(int operatingTemp) const;
    void degradePolymer(int exposureHours);
};

class MaterialBatch {
private:
    std::string batchNumber_;
    double weightKg_;
    std::string arrivalDate_;
    bool isQuarantined_;
    Supplier* supplierRef_;

public:
    MaterialBatch(const std::string& batchNumber, double weightKg, const std::string& arrivalDate,
                  Supplier* supplier = nullptr, bool isQuarantined = false);

    void deductWeight(double requiredAmount);
    void quarantineBatch(const std::string& reason);
    void releaseToProduction();
};


class AutoPart {
protected:
    std::string partId_;
    double nominalWeight_;
    double standardDimension_;
    double manufacturingCost_;
    bool isFinished_;

public:
    AutoPart(const std::string& partId, double nominalWeight, double standardDimension,
             double manufacturingCost, bool isFinished = false)
        : partId_(partId), nominalWeight_(nominalWeight), standardDimension_(standardDimension),
          manufacturingCost_(manufacturingCost), isFinished_(isFinished) {
        if (nominalWeight_ <= 0.0 || standardDimension_ <= 0.0) {
            throw PlantException("Invalid auto part parameters.");
        }
    }
    virtual ~AutoPart() = default;

    std::string getPartId() const { return partId_; }
    double getManufacturingCost() const { return manufacturingCost_; }
    bool isFinished() const { return isFinished_; }

    bool applyProtectingCoating(const std::string& coatingType);
    bool markFinished() { isFinished_ = true; return true; }
    double calculateFinalPrice(double markupPercent) const;
};

class EnginePiston : public AutoPart {
private:
    double skirtDiameter_;
    double crownThickness_;
    int ringGrooveCount_;
    double compressionHeight_;

public:
    EnginePiston(const std::string& partId, double nominalWeight, double standardDimension,
                 double manufacturingCost, double skirtDiameter, double crownThickness,
                 int ringGrooveCount, double compressionHeight, bool isFinished = false);

    bool verifyThermalExpansion(double operatingTemp) const;
    void balanceWeight(double targetWeight);
};

class BrakeDisc : public AutoPart {
private:
    double rotorDiameter_;
    int vaneCount_;
    double frictionSurfaceThickness_;
    double runoutMicrons_;

public:
    BrakeDisc(const std::string& partId, double nominalWeight, double standardDimension,
              double manufacturingCost, double rotorDiameter, int vaneCount,
              double thickness, double runout, bool isFinished = false);

    void measureBrakingWear(double wearAmount);
    bool checkDynamicBalance() const;
};

class TransmissionGear : public AutoPart {
private:
    int toothCount_;
    double moduleSize_;
    double helixAngle_;
    double surfaceHardnessHRC_;

public:
    TransmissionGear(const std::string& partId, double nominalWeight, double standardDimension,
                     double manufacturingCost, int toothCount, double moduleSize,
                     double helixAngle, double hardness, bool isFinished = false);

    double calculateGearRatio(int matingTeeth) const;
    bool verifyToothMeshing(double backlash) const;
};

class CarHeadlight : public AutoPart {
private:
    double luminousFluxLumens_;
    int ledModuleCount_;
    int housingSealingGradeIP_;
    double operatingVoltage_;

public:
    CarHeadlight(const std::string& partId, double nominalWeight, double standardDimension,
                 double manufacturingCost, double luminousFlux, int ledCount,
                 int ipGrade, double voltage, bool isFinished = false);

    bool testWaterIngressProtection(int pressureBar) const;
    void adjustBeamAngle(double horizontalAngle, double verticalAngle);
};



class BillOfMaterials {
private:
    std::string specificationCode_;
    int requiredPartTypesCount_;
    double totalRawMaterialNeedKg_;

public:
    BillOfMaterials(const std::string& specCode, int partTypesCount, double totalMaterialNeed);

    void addRequiredComponent(const std::string& partType, int qty);
    double calculateTotalMaterialWeight() const;
    bool matchInventoryAvailability() const;
    void cloneForVariant(const std::string& variantCode);
};

class AssembledUnit {
private:
    std::string unitId_;
    int partsCount_;
    double assemblyDurationMinutes_;
    double totalUnitWeight_;
    bool assemblyPassed_;

public:
    AssembledUnit(const std::string& unitId, int partsCount, double duration,
                  double totalWeight, bool passed = false);

    void attachComponent(AutoPart& part);
    bool validateCompleteness(const BillOfMaterials& bom);
    bool performTorqueCheck(double torqueNm);
    void disassembleDefectiveUnit();
};