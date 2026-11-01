#include "MaterialAndDetails.hpp"
#include "WarehouseAndLogistics.hpp"
#include <cmath>

double RawMaterial::calculateBatchCost(double weight) const {
    if (weight <= 0.0) {
        throw PlantException("Weight must be positive for cost calculation.");
    }
    return weight * costPerKg_;
}

bool RawMaterial::verifyMeltingSuitability(double furnaceTemp) const {
    if (furnaceTemp <= 0.0) {
        throw PlantException("Invalid furnace temperature.");
    }
    return furnaceTemp >= meltingPoint_;
}

double RawMaterial::applyLossCoefficient(double lossPercent) const {
    if (lossPercent < 0.0 || lossPercent >= 100.0) {
        throw PlantException("Invalid loss percentage.");
    }
    return costPerKg_ * (1.0 + lossPercent / 100.0);
}


void MetalAlloy::conductHardeningProcess(int quenchTemp) {
    if (quenchTemp < 200 || quenchTemp > meltingPoint_) {
        throw PlantException("Invalid quenching temperature for hardening.");
    }
    isHeatTreated_ = true;
    tensileStrength_ *= 1.25; 
}

bool MetalAlloy::checkWeldability() const {
    return carbonContent_ <= 0.25 && !isHeatTreated_;
}


double PlasticComposite::evaluateThermalDeformation(int operatingTemp) const {
    if (reinforcementFiber_.empty()) {
        throw PlantException("Reinforcement fiber type is not specified.");
    }
    if (operatingTemp <= glassTransitionTemp_) {
        return 0.0;
    }
    return (operatingTemp - glassTransitionTemp_) * 0.05;
}

void PlasticComposite::degradePolymer(int exposureHours) {
    if (exposureHours < 0) {
        throw PlantException("Exposure hours cannot be negative.");
    }
    double degradation = (exposureHours * 0.001) / (uvResistanceIndex_ > 0.0 ? uvResistanceIndex_ : 1.0);
    uvResistanceIndex_ = (uvResistanceIndex_ > degradation) ? (uvResistanceIndex_ - degradation) : 0.0;
}


MaterialBatch::MaterialBatch(const std::string& batchNumber, double weightKg, const std::string& arrivalDate,
                            Supplier* supplier, bool isQuarantined) : 
    batchNumber_(batchNumber), weightKg_(weightKg), arrivalDate_(arrivalDate),
    isQuarantined_(isQuarantined), supplierRef_(supplier) {
    if (weightKg_ < 0.0) {
        throw PlantException("Invalid material batch parameters.");
    }
}

void MaterialBatch::deductWeight(double requiredAmount) {
    if (isQuarantined_) {
        throw DefectiveBatchException("Cannot deduct weight from a quarantined batch: " + batchNumber_);
    }
    if (requiredAmount <= 0.0) {
        throw PlantException("Deduction amount must be positive.");
    }
    if (requiredAmount > weightKg_) {
        throw MaterialShortageException("Not enough material in batch " + batchNumber_);
    }
    weightKg_ -= requiredAmount;
}

void MaterialBatch::quarantineBatch(const std::string& reason) {
    if (reason.empty()) {
        throw PlantException("Quarantine reason must be provided.");
    }
    isQuarantined_ = true;
    if (supplierRef_ != nullptr) {
        supplierRef_->penalizeLateDelivery(1);
    }
}

void MaterialBatch::releaseToProduction() {
    if (weightKg_ <= 0.0) {
        throw MaterialShortageException("Cannot release empty batch " + batchNumber_);
    }
    isQuarantined_ = false;
}


bool AutoPart::applyProtectingCoating(const std::string& coatingType) {
    if (coatingType.empty()) {
        throw PlantException("Coating type cannot be empty.");
    }
    manufacturingCost_ += 15.0; 
    return true;
}

double AutoPart::calculateFinalPrice(double markupPercent) const {
    if (markupPercent < 0.0) {
        throw PlantException("Markup percent cannot be negative.");
    }
    return manufacturingCost_ * (1.0 + markupPercent / 100.0);
}


EnginePiston::EnginePiston(const std::string& partId, double nominalWeight, double standardDimension,
                           double manufacturingCost, double skirtDiameter, double crownThickness,
                           int ringGrooveCount, double compressionHeight, bool isFinished)
    : AutoPart(partId, nominalWeight, standardDimension, manufacturingCost, isFinished),
    skirtDiameter_(skirtDiameter), crownThickness_(crownThickness),
    ringGrooveCount_(ringGrooveCount), compressionHeight_(compressionHeight) {
    if (skirtDiameter_ <= 0.0 || crownThickness_ <= 0.0 || ringGrooveCount_ <= 0 || compressionHeight_ <= 0.0) {
        throw PlantException("Invalid engine piston parameters.");
    }
}

bool EnginePiston::verifyThermalExpansion(double operatingTemp) const {
    double expansion = skirtDiameter_ * 0.000012 * (operatingTemp - 20.0) + (crownThickness_ * 0.0001);
    if (expansion > 0.15) {
        throw ToleranceViolationException("Piston thermal expansion exceeds limit.");
    }
    return compressionHeight_ > 0.0 && ringGrooveCount_ > 0;
}

void EnginePiston::balanceWeight(double targetWeight) {
    if (targetWeight <= 0.0) {
        throw PlantException("Target weight must be positive.");
    }
    if (std::abs(nominalWeight_ - targetWeight) > 5.0) {
        throw ToleranceViolationException("Piston weight difference is too large to balance.");
    }
    nominalWeight_ = targetWeight;
}


BrakeDisc::BrakeDisc(const std::string& partId, double nominalWeight, double standardDimension,
                     double manufacturingCost, double rotorDiameter, int vaneCount,
                     double thickness, double runout, bool isFinished)
    : AutoPart(partId, nominalWeight, standardDimension, manufacturingCost, isFinished),
    rotorDiameter_(rotorDiameter), vaneCount_(vaneCount),
    frictionSurfaceThickness_(thickness), runoutMicrons_(runout) {
    if (rotorDiameter_ <= 0.0 || vaneCount_ <= 0 || frictionSurfaceThickness_ <= 0.0 || runoutMicrons_ < 0.0) {
        throw PlantException("Invalid brake disc parameters.");
    }
}

void BrakeDisc::measureBrakingWear(double wearAmount) {
    if (wearAmount < 0.0) {
        throw PlantException("Wear amount cannot be negative.");
    }
    frictionSurfaceThickness_ -= wearAmount;
    if (frictionSurfaceThickness_ < 20.0) {
        throw ToleranceViolationException("Brake disc thickness below minimum safety threshold.");
    }
}

bool BrakeDisc::checkDynamicBalance() const {
    if (runoutMicrons_ > 50.0) {
        throw ToleranceViolationException("Brake disc runout exceeds allowable tolerance.");
    }
    return rotorDiameter_ > 0.0 && vaneCount_ > 0;
}


TransmissionGear::TransmissionGear(const std::string& partId, double nominalWeight, double standardDimension,
                                   double manufacturingCost, int toothCount, double moduleSize,
                                   double helixAngle, double hardness, bool isFinished)
    : AutoPart(partId, nominalWeight, standardDimension, manufacturingCost, isFinished),
    toothCount_(toothCount), moduleSize_(moduleSize),
    helixAngle_(helixAngle), surfaceHardnessHRC_(hardness) {
    if (toothCount_ <= 0 || moduleSize_ <= 0.0 || surfaceHardnessHRC_ <= 0.0) {
        throw PlantException("Invalid transmission gear parameters.");
    }
}

double TransmissionGear::calculateGearRatio(int matingTeeth) const {
    if (matingTeeth <= 0) {
        throw PlantException("Mating teeth count must be positive.");
    }
    return static_cast<double>(matingTeeth) / static_cast<double>(toothCount_);
}

bool TransmissionGear::verifyToothMeshing(double backlash) const {
    if (backlash < 0.05 || backlash > 0.5 * moduleSize_) {
        throw ToleranceViolationException("Gear tooth backlash is out of tolerance.");
    }
    return surfaceHardnessHRC_ >= 55.0 && std::abs(helixAngle_) <= 45.0;
}


CarHeadlight::CarHeadlight(const std::string& partId, double nominalWeight, double standardDimension,
                           double manufacturingCost, double luminousFlux, int ledCount,
                           int ipGrade, double voltage, bool isFinished)
    : AutoPart(partId, nominalWeight, standardDimension, manufacturingCost, isFinished),
    luminousFluxLumens_(luminousFlux), ledModuleCount_(ledCount),
    housingSealingGradeIP_(ipGrade), operatingVoltage_(voltage) {
    if (luminousFluxLumens_ <= 0.0 || ledModuleCount_ <= 0 || housingSealingGradeIP_ < 0 || operatingVoltage_ <= 0.0) {
        throw PlantException("Invalid car headlight parameters.");
    }
}

bool CarHeadlight::testWaterIngressProtection(int pressureBar) const {
    if (pressureBar < 0) {
        throw PlantException("Pressure cannot be negative.");
    }
    if (housingSealingGradeIP_ < 65 && pressureBar > 2) {
        throw ToleranceViolationException("Headlight failed water ingress test.");
    }
    return operatingVoltage_ >= 12.0 && ledModuleCount_ > 0;
}

void CarHeadlight::adjustBeamAngle(double horizontalAngle, double verticalAngle) {
    if (std::abs(horizontalAngle) > 15.0 || std::abs(verticalAngle) > 10.0) {
        throw ToleranceViolationException("Beam angle adjustment exceeds optical limits.");
    }
    luminousFluxLumens_ *= 0.99; // Небольшая калибровка светового потока
}


BillOfMaterials::BillOfMaterials(const std::string& specCode, int partTypesCount, double totalMaterialNeed)
    : specificationCode_(specCode), requiredPartTypesCount_(partTypesCount),
    totalRawMaterialNeedKg_(totalMaterialNeed) {
    if (specificationCode_.empty() || requiredPartTypesCount_ < 0 || totalRawMaterialNeedKg_ < 0.0) {
        throw PlantException("Invalid Bill of Materials parameters.");
    }
}

void BillOfMaterials::addRequiredComponent(const std::string& partType, int qty) {
    if (partType.empty() || qty <= 0) {
        throw PlantException("Invalid component specification.");
    }
    requiredPartTypesCount_ += 1;
    totalRawMaterialNeedKg_ += qty * 1.5;
}

double BillOfMaterials::calculateTotalMaterialWeight() const {
    return totalRawMaterialNeedKg_;
}

bool BillOfMaterials::matchInventoryAvailability() const {
    if (totalRawMaterialNeedKg_ <= 0.0 || requiredPartTypesCount_ <= 0) {
        throw MaterialShortageException("BOM has no valid material requirements.");
    }
    return true;
}

void BillOfMaterials::cloneForVariant(const std::string& variantCode) {
    if (variantCode.empty()) {
        throw PlantException("Variant code cannot be empty.");
    }
    specificationCode_ += "-" + variantCode;
}


AssembledUnit::AssembledUnit(const std::string& unitId, int partsCount, double duration,
                             double totalWeight, bool passed)
    : unitId_(unitId), partsCount_(partsCount), assemblyDurationMinutes_(duration),
    totalUnitWeight_(totalWeight), assemblyPassed_(passed) {
    if (unitId_.empty() || partsCount_ < 0 || assemblyDurationMinutes_ < 0.0 || totalUnitWeight_ < 0.0) {
        throw PlantException("Invalid assembled unit parameters.");
    }
}

void AssembledUnit::attachComponent(AutoPart& part) {
    if (!part.isFinished()) {
        throw AssemblyMismatchException("Cannot attach unfinished or defective part: " + part.getPartId());
    }
    partsCount_ += 1;
    totalUnitWeight_ += 1.0;
    assemblyDurationMinutes_ += 5.0;
}

bool AssembledUnit::validateCompleteness(const BillOfMaterials& bom) {
    if (partsCount_ <= 0 || totalUnitWeight_ < bom.calculateTotalMaterialWeight() * 0.5) {
        throw AssemblyMismatchException("Unit " + unitId_ + " does not match BOM requirements.");
    }
    assemblyPassed_ = true;
    return assemblyPassed_;
}

bool AssembledUnit::performTorqueCheck(double torqueNm) {
    if (torqueNm < 20.0 || torqueNm > 250.0) {
        assemblyPassed_ = false;
        throw ToleranceViolationException("Fastener torque out of specification.");
    }
    assemblyPassed_ = true;
    return assemblyPassed_;
}

void AssembledUnit::disassembleDefectiveUnit() {
    if (partsCount_ == 0) {
        throw PlantException("Unit is already disassembled.");
    }
    partsCount_ = 0;
    totalUnitWeight_ = 0.0;
    assemblyPassed_ = false;
}