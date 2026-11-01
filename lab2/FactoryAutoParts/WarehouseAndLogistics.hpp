#pragma once
#include "Exceptions.hpp"
#include <string>
#include <vector>

class AutoPart;
class MaterialBatch;
class WarehouseWorker;
class Client;
class AssembledUnit;

class StockItem {
private:
    std::string skuIdentifier_;
    int stockQuantity_;
    int reservedQuantity_;
    int daysInStorage_;

public:
    StockItem(const std::string& skuIdentifier, int stockQuantity, 
              int reservedQuantity, int daysInStorage);

    void reserveStock(int amount);
    bool flagSlowMovingStock(int thresholdDays) const;
};

class StorageCell {
private:
    std::string shelfCoordinate_;
    double cellWeightLimitKg_;
    bool isOccupied_;
    std::string lastAccessedTimestamp_;

public:
    StorageCell(const std::string& shelfCoordinate, double cellWeightLimitKg, 
                bool isOccupied = false, const std::string& lastAccessedTimestamp = "");

    void reserveCellForPlacement();
    bool verifyWeightPermissibility(double incomingWeight) const;
    void clearCell();
};

class StorageRack {
private:
    int rackIdentifier_;
    int tierCount_;
    double maxStructuralLoadKg_;
    double currentStructuralLoadKg_;

public:
    StorageRack(int rackIdentifier, int tierCount, 
                double maxStructuralLoadKg, double currentStructuralLoadKg);

    bool checkStructuralIntegrity() const;
    void distributeWeightEvenly();
    void decommissionDamagedTier(int tierNumber);
};

class Warehouse {
private:
    std::string warehouseCode_;
    double ambientTemperature_;
    double humidityLevel_;
    int totalRacksCapacity_;
    bool isHazardousMaterialsAllowed_;

public:
    Warehouse(const std::string& warehouseCode, double ambientTemperature, 
              double humidityLevel, int totalRacksCapacity, bool isHazardousMaterialsAllowed);

    bool auditClimateConditions() const;
    int locateFreeStorageCell() const;
    void executeQuarterlyInventory();
    void lockWarehouseZone(int rackId);
};

class TransportPallet {
private:
    std::string palletBarcode_;
    double tareWeightKg_;
    double cargoPayloadKg_;
    bool isStrappedAndSecured_;

public:
    TransportPallet(const std::string& palletBarcode, double tareWeightKg, 
                    double cargoPayloadKg, bool isStrappedAndSecured = false);

    void loadCartonBox(double boxWeight);
    void wrapWithStretchFilm();
    double verifyTotalGrossWeight() const;
};

class Forklift {
private:
    double batteryLevelPercent_;
    double maxLiftHeightMeters_;
    double liftingCapacityKg_;
    bool isMastMoving_;

public:
    Forklift(double batteryLevelPercent, double maxLiftHeightMeters, 
             double liftingCapacityKg, bool isMastMoving = false);

    void elevateForks(double targetHeight);
    void transportPalletToDispatch(TransportPallet& pallet);
    void dockToChargingStation();
};

class Supplier {
private:
    std::string companyTaxId_;
    double reliabilityRating_;
    int averageLeadTimeDays_;
    bool contractStatusActive_;

public:
    Supplier(const std::string& companyTaxId, double reliabilityRating, 
             int averageLeadTimeDays, bool contractStatusActive = true);

    void penalizeLateDelivery(int daysDelayed);
    double evaluateSupplierScorecard() const;
    void renewSupplyAgreement(int extensionYears);
};

class SupplyOrder {
private:
    int poNumber_;
    double totalPurchasedVolumeKg_;
    std::string agreedDeliveryDeadline_;
    double totalPaymentAmount_;
    bool isCustomsCleared_;
    bool isDelivered_;

public:
    SupplyOrder(int poNumber, double totalPurchasedVolumeKg, 
                const std::string& agreedDeliveryDeadline, double totalPaymentAmount, 
                bool isCustomsCleared = true, bool isDelivered = false);

    void authorizePurchaseBudget();
    std::string trackInTransitLocation() const;
    void confirmReceiptAtGate();
    double calculateLatePenalties(int daysLate) const;
};

class ShipmentOrder {
private:
    int shipmentId_;
    int palletCountScheduled_;
    std::string targetDestinationCity_;
    std::string truckPlateNumber_;
    bool isDispatchCleared_;
    double shippingInvoiceValue_;

public:
    ShipmentOrder(int shipmentId, int palletCountScheduled, 
                  const std::string& targetDestinationCity, const std::string& truckPlateNumber, 
                  double shippingInvoiceValue, bool isDispatchCleared = false);

    bool validateLotBarcodes();
    void sealTruckTrailer(const std::string& sealCode);
    std::string generateBillOfLading() const;
    void closeFulfilledShipment();
};

class DeliveryRoute {
private:
    double routeDistanceKm_;
    double estimatedFuelLiters_;
    double tollRoadFees_;
    double transitTimeHours_;

public:
    DeliveryRoute(double routeDistanceKm, double estimatedFuelLiters, 
                  double tollRoadFees, double transitTimeHours);

    void optimizeWaypoints();
    double recalculateEstimatedArrival(double currentSpeed) const;
    void recordTollExpense(double fee);
};