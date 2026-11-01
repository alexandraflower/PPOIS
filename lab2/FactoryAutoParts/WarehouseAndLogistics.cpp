#include "WarehouseAndLogistics.hpp"
#include "MaterialAndDetails.hpp"


StockItem::StockItem(const std::string& skuIdentifier, int stockQuantity,
                     int reservedQuantity, int daysInStorage)
    : skuIdentifier_(skuIdentifier), stockQuantity_(stockQuantity),
    reservedQuantity_(reservedQuantity), daysInStorage_(daysInStorage) {
    if (skuIdentifier_.empty() || stockQuantity_ < 0 || reservedQuantity_ < 0 || daysInStorage_ < 0) {
        throw PlantException("Invalid stock item parameters.");
    }
}

void StockItem::reserveStock(int amount) {
    if (amount <= 0) {
        throw PlantException("Reserve amount must be positive.");
    }
    if (reservedQuantity_ + amount > stockQuantity_) {
        throw MaterialShortageException("Insufficient available stock for SKU: " + skuIdentifier_);
    }
    reservedQuantity_ += amount;
}

bool StockItem::flagSlowMovingStock(int thresholdDays) const {
    if (thresholdDays <= 0) {
        throw PlantException("Threshold days must be positive.");
    }
    return daysInStorage_ >= thresholdDays;
}


StorageCell::StorageCell(const std::string& shelfCoordinate, double cellWeightLimitKg,
                         bool isOccupied, const std::string& lastAccessedTimestamp)
    : shelfCoordinate_(shelfCoordinate), cellWeightLimitKg_(cellWeightLimitKg),
    isOccupied_(isOccupied), lastAccessedTimestamp_(lastAccessedTimestamp) {
    if (shelfCoordinate_.empty() || cellWeightLimitKg_ <= 0.0) {
        throw PlantException("Invalid storage cell parameters.");
    }
}

void StorageCell::reserveCellForPlacement() {
    if (isOccupied_) {
        throw StorageCellFullException("Storage cell " + shelfCoordinate_ + " is already occupied.");
    }
    isOccupied_ = true;
    lastAccessedTimestamp_ = "RESERVED";
}

bool StorageCell::verifyWeightPermissibility(double incomingWeight) const {
    if (incomingWeight <= 0.0) {
        throw PlantException("Incoming weight must be positive.");
    }
    if (incomingWeight > cellWeightLimitKg_) {
        throw StorageCellFullException("Incoming weight exceeds limit of cell " + shelfCoordinate_);
    }
    return !isOccupied_;
}

void StorageCell::clearCell() {
    isOccupied_ = false;
    lastAccessedTimestamp_ = "CLEARED";
}


StorageRack::StorageRack(int rackIdentifier, int tierCount,
                         double maxStructuralLoadKg, double currentStructuralLoadKg)
    : rackIdentifier_(rackIdentifier), tierCount_(tierCount),
    maxStructuralLoadKg_(maxStructuralLoadKg), currentStructuralLoadKg_(currentStructuralLoadKg) {
    if (rackIdentifier_ <= 0 || tierCount_ <= 0 || maxStructuralLoadKg_ <= 0.0 || currentStructuralLoadKg_ < 0.0) {
        throw PlantException("Invalid storage rack parameters.");
    }
}

bool StorageRack::checkStructuralIntegrity() const {
    return currentStructuralLoadKg_ <= maxStructuralLoadKg_ && tierCount_ > 0;
}

void StorageRack::distributeWeightEvenly() {
    if (!checkStructuralIntegrity()) {
        throw StorageCellFullException("Rack #" + std::to_string(rackIdentifier_) + " is overloaded.");
    }
}

void StorageRack::decommissionDamagedTier(int tierNumber) {
    if (tierNumber <= 0 || tierNumber > tierCount_) {
        throw PlantException("Invalid tier number for decommissioning.");
    }
    tierCount_ -= 1;
    maxStructuralLoadKg_ *= 0.8;
}


Warehouse::Warehouse(const std::string& warehouseCode, double ambientTemperature,
                     double humidityLevel, int totalRacksCapacity, bool isHazardousMaterialsAllowed)
    : warehouseCode_(warehouseCode), ambientTemperature_(ambientTemperature),
      humidityLevel_(humidityLevel), totalRacksCapacity_(totalRacksCapacity),
      isHazardousMaterialsAllowed_(isHazardousMaterialsAllowed) {
    if (warehouseCode_.empty() || humidityLevel_ < 0.0 || humidityLevel_ > 100.0 || totalRacksCapacity_ <= 0) {
        throw PlantException("Invalid warehouse parameters.");
    }
}

bool Warehouse::auditClimateConditions() const {
    bool tempOk = (ambientTemperature_ >= 5.0 && ambientTemperature_ <= 35.0);
    bool humidityOk = (humidityLevel_ >= 20.0 && humidityLevel_ <= 75.0);
    return tempOk && humidityOk;
}

int Warehouse::locateFreeStorageCell() const {
    if (totalRacksCapacity_ <= 0) {
        throw StorageCellFullException("Warehouse " + warehouseCode_ + " has no available racks.");
    }
    return 1;
}

void Warehouse::executeQuarterlyInventory() {
    if (!auditClimateConditions() && isHazardousMaterialsAllowed_) {
        throw PlantException("Unsafe climate conditions in hazardous warehouse " + warehouseCode_);
    }
}

void Warehouse::lockWarehouseZone(int rackId) {
    if (rackId <= 0 || rackId > totalRacksCapacity_) {
        throw PlantException("Invalid rack ID to lock.");
    }
    isHazardousMaterialsAllowed_ = false;
}


TransportPallet::TransportPallet(const std::string& palletBarcode, double tareWeightKg,
                                 double cargoPayloadKg, bool isStrappedAndSecured)
    : palletBarcode_(palletBarcode), tareWeightKg_(tareWeightKg),
    cargoPayloadKg_(cargoPayloadKg), isStrappedAndSecured_(isStrappedAndSecured) {
    if (palletBarcode_.empty() || tareWeightKg_ <= 0.0 || cargoPayloadKg_ < 0.0) {
        throw PlantException("Invalid transport pallet parameters.");
    }
}

void TransportPallet::loadCartonBox(double boxWeight) {
    if (boxWeight <= 0.0) {
        throw PlantException("Box weight must be positive.");
    }
    if (cargoPayloadKg_ + boxWeight > 1500.0) {
        throw StorageCellFullException("Pallet " + palletBarcode_ + " maximum payload exceeded.");
    }
    cargoPayloadKg_ += boxWeight;
    isStrappedAndSecured_ = false;
}

void TransportPallet::wrapWithStretchFilm() {
    if (cargoPayloadKg_ <= 0.0) {
        throw PlantException("Cannot wrap an empty pallet.");
    }
    isStrappedAndSecured_ = true;
}

double TransportPallet::verifyTotalGrossWeight() const {
    return tareWeightKg_ + cargoPayloadKg_;
}


Forklift::Forklift(double batteryLevelPercent, double maxLiftHeightMeters,
                   double liftingCapacityKg, bool isMastMoving)
    : batteryLevelPercent_(batteryLevelPercent), maxLiftHeightMeters_(maxLiftHeightMeters),
    liftingCapacityKg_(liftingCapacityKg), isMastMoving_(isMastMoving) {
    if (batteryLevelPercent_ < 0.0 || batteryLevelPercent_ > 100.0 ||
        maxLiftHeightMeters_ <= 0.0 || liftingCapacityKg_ <= 0.0) {
        throw PlantException("Invalid forklift parameters.");
    }
}

void Forklift::elevateForks(double targetHeight) {
    if (batteryLevelPercent_ < 15.0) {
        throw PlantException("Forklift battery too low to elevate forks.");
    }
    if (targetHeight <= 0.0 || targetHeight > maxLiftHeightMeters_) {
        throw PlantException("Target lift height exceeds forklift capabilities.");
    }
    isMastMoving_ = true;
    batteryLevelPercent_ -= 2.0;
    isMastMoving_ = false;
}

void Forklift::transportPalletToDispatch(TransportPallet& pallet) {
    if (batteryLevelPercent_ < 15.0) {
        throw PlantException("Forklift requires charging before transporting pallet.");
    }
    if (pallet.verifyTotalGrossWeight() > liftingCapacityKg_) {
        throw PlantException("Pallet weight exceeds forklift lifting capacity.");
    }
    pallet.wrapWithStretchFilm();
    batteryLevelPercent_ -= 5.0;
}

void Forklift::dockToChargingStation() {
    isMastMoving_ = false;
    batteryLevelPercent_ = 100.0;
}


Supplier::Supplier(const std::string& companyTaxId, double reliabilityRating,
                   int averageLeadTimeDays, bool contractStatusActive)
    : companyTaxId_(companyTaxId), reliabilityRating_(reliabilityRating),
    averageLeadTimeDays_(averageLeadTimeDays), contractStatusActive_(contractStatusActive) {
    if (companyTaxId_.empty() || reliabilityRating_ < 0.0 || reliabilityRating_ > 100.0 || averageLeadTimeDays_ <= 0) {
        throw PlantException("Invalid supplier parameters.");
    }
}

void Supplier::penalizeLateDelivery(int daysDelayed) {
    if (daysDelayed < 0) {
        throw PlantException("Delayed days cannot be negative.");
    }
    double penalty = daysDelayed * 2.5;
    reliabilityRating_ = (reliabilityRating_ > penalty) ? (reliabilityRating_ - penalty) : 0.0;
    averageLeadTimeDays_ += daysDelayed;
}

double Supplier::evaluateSupplierScorecard() const {
    if (!contractStatusActive_ || companyTaxId_.empty()) {
        return 0.0;
    }
    return reliabilityRating_ - (averageLeadTimeDays_ * 0.5);
}

void Supplier::renewSupplyAgreement(int extensionYears) {
    if (extensionYears <= 0) {
        throw PlantException("Extension years must be positive.");
    }
    if (reliabilityRating_ < 30.0) {
        throw PlantException("Cannot renew agreement with low-reliability supplier.");
    }
    contractStatusActive_ = true;
}


SupplyOrder::SupplyOrder(int poNumber, double totalPurchasedVolumeKg,
                         const std::string& agreedDeliveryDeadline, double totalPaymentAmount,
                         bool isCustomsCleared, bool isDelivered)
    : poNumber_(poNumber), totalPurchasedVolumeKg_(totalPurchasedVolumeKg),
    agreedDeliveryDeadline_(agreedDeliveryDeadline), totalPaymentAmount_(totalPaymentAmount),
    isCustomsCleared_(isCustomsCleared), isDelivered_(isDelivered) {
    if (poNumber_ <= 0 || totalPurchasedVolumeKg_ <= 0.0 || agreedDeliveryDeadline_.empty() || totalPaymentAmount_ <= 0.0) {
        throw PlantException("Invalid supply order parameters.");
    }
}

void SupplyOrder::authorizePurchaseBudget() {
    if (totalPaymentAmount_ <= 0.0 || totalPurchasedVolumeKg_ <= 0.0) {
        throw PlantException("Invalid purchase budget parameters.");
    }
    isCustomsCleared_ = true;
}

std::string SupplyOrder::trackInTransitLocation() const {
    if (isDelivered_) {
        return "Delivered to plant gate (PO #" + std::to_string(poNumber_) + ")";
    }
    if (!isCustomsCleared_) {
        return "Held at customs (Deadline: " + agreedDeliveryDeadline_ + ")";
    }
    return "In transit";
}

void SupplyOrder::confirmReceiptAtGate() {
    if (!isCustomsCleared_) {
        throw PlantException("Cannot confirm receipt: supply order not cleared by customs.");
    }
    isDelivered_ = true;
}

double SupplyOrder::calculateLatePenalties(int daysLate) const {
    if (daysLate < 0) {
        throw PlantException("Days late cannot be negative.");
    }
    return daysLate * (totalPaymentAmount_ * 0.01);
}


ShipmentOrder::ShipmentOrder(int shipmentId, int palletCountScheduled,
                             const std::string& targetDestinationCity, const std::string& truckPlateNumber,
                             double shippingInvoiceValue, bool isDispatchCleared)
    : shipmentId_(shipmentId), palletCountScheduled_(palletCountScheduled),
    targetDestinationCity_(targetDestinationCity), truckPlateNumber_(truckPlateNumber),
    isDispatchCleared_(isDispatchCleared), shippingInvoiceValue_(shippingInvoiceValue) {
    if (shipmentId_ <= 0 || palletCountScheduled_ <= 0 || targetDestinationCity_.empty() ||
        truckPlateNumber_.empty() || shippingInvoiceValue_ < 0.0) {
        throw PlantException("Invalid shipment order parameters.");
    }
}

bool ShipmentOrder::validateLotBarcodes() {
    if (palletCountScheduled_ <= 0 || shippingInvoiceValue_ <= 0.0) {
        return false;
    }
    isDispatchCleared_ = true;
    return isDispatchCleared_;
}

void ShipmentOrder::sealTruckTrailer(const std::string& sealCode) {
    if (sealCode.empty()) {
        throw PlantException("Seal code cannot be empty.");
    }
    if (!isDispatchCleared_) {
        throw PlantException("Cannot seal trailer before dispatch clearance.");
    }
}

std::string ShipmentOrder::generateBillOfLading() const {
    return "BOL #" + std::to_string(shipmentId_) + " | To: " + targetDestinationCity_ +
           " | Truck: " + truckPlateNumber_ + " | Pallets: " + std::to_string(palletCountScheduled_);
}

void ShipmentOrder::closeFulfilledShipment() {
    if (!isDispatchCleared_) {
        throw PlantException("Cannot close un-cleared shipment.");
    }
    palletCountScheduled_ = 0;
}


DeliveryRoute::DeliveryRoute(double routeDistanceKm, double estimatedFuelLiters,
                             double tollRoadFees, double transitTimeHours)
    : routeDistanceKm_(routeDistanceKm), estimatedFuelLiters_(estimatedFuelLiters),
    tollRoadFees_(tollRoadFees), transitTimeHours_(transitTimeHours) {
    if (routeDistanceKm_ <= 0.0 || estimatedFuelLiters_ <= 0.0 || tollRoadFees_ < 0.0 || transitTimeHours_ <= 0.0) {
        throw PlantException("Invalid delivery route parameters.");
    }
}

void DeliveryRoute::optimizeWaypoints() {
    routeDistanceKm_ *= 0.92;
    estimatedFuelLiters_ = routeDistanceKm_ * 0.3;
    transitTimeHours_ *= 0.92;
}

double DeliveryRoute::recalculateEstimatedArrival(double currentSpeed) const {
    if (currentSpeed <= 0.0) {
        throw PlantException("Current speed must be positive.");
    }
    return routeDistanceKm_ / currentSpeed;
}

void DeliveryRoute::recordTollExpense(double fee) {
    if (fee < 0.0) {
        throw PlantException("Toll fee cannot be negative.");
    }
    tollRoadFees_ += fee;
}