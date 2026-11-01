#include <UnitTest++/UnitTest++.h>
#include <string>
#include "Exceptions.hpp"
#include "MaterialAndDetails.hpp"
#include "ProductionAndEquipment.hpp"
#include "QualityControl.hpp"
#include "WarehouseAndLogistics.hpp"
#include "OrdersAndManagement.hpp"

class IncidentReport {};

SUITE(ExceptionsSuite) {
    TEST(AllPlantExceptionsHierarchyAndMessages) {
        PlantException baseEx("Base plant error");
        CHECK_EQUAL("Base plant error", std::string(baseEx.what()));

        MaterialShortageException matEx;
        CHECK_EQUAL("Material shortage.", std::string(matEx.what()));

        DefectiveBatchException defEx;
        CHECK_EQUAL("Defective batch.", std::string(defEx.what()));

        MachineOverheatException heatEx;
        CHECK_EQUAL("Critical overheat.", std::string(heatEx.what()));

        ToolWearLimitException wearEx;
        CHECK_EQUAL("Critical tool wear limit.", std::string(wearEx.what()));

        ToleranceViolationException tolEx;
        CHECK_EQUAL("Tolerance violation.", std::string(tolEx.what()));

        StorageCellFullException cellEx;
        CHECK_EQUAL("Storage cell is full.", std::string(cellEx.what()));

        ShiftNotStaffedException shiftEx;
        CHECK_EQUAL("Shift not staffed.", std::string(shiftEx.what()));

        UnauthorizedOperationException authEx;
        CHECK_EQUAL("Unauthorized operator.", std::string(authEx.what()));

        OrderDeadlineExceededException deadEx;
        CHECK_EQUAL("Order deadline exceeded.", std::string(deadEx.what()));

        AssemblyMismatchException asmEx;
        CHECK_EQUAL("Assembly mismatch.", std::string(asmEx.what()));

        SafetyShieldOpenException shieldEx;
        CHECK_EQUAL("Safety shield open.", std::string(shieldEx.what()));

        CHECK_THROW(throw MaterialShortageException("Custom shortage"), PlantException);
        CHECK_THROW(throw SafetyShieldOpenException("Custom shield"), std::runtime_error);
    }
}

SUITE(MaterialAndDetailsSuite) {
    TEST(RawMaterial_Test) {
        RawMaterial rm("RM-01", 7800.0, 1400.0, 100.0);
        CHECK_EQUAL("RM-01", rm.getMaterialId());
        CHECK_CLOSE(7800.0, rm.getDensity(), 1e-5);
        CHECK_CLOSE(100.0, rm.getCostPerKg(), 1e-5);

        CHECK_CLOSE(500.0, rm.calculateBatchCost(5.0), 1e-5);
        CHECK(rm.verifyMeltingSuitability(1500.0));
        CHECK(!rm.verifyMeltingSuitability(1200.0));
        CHECK_CLOSE(110.0, rm.applyLossCoefficient(10.0), 1e-5);

        CHECK_THROW(RawMaterial("BAD", 0.0, 1000.0, 50.0), PlantException);
        CHECK_THROW(rm.calculateBatchCost(-1.0), PlantException);
        CHECK_THROW(rm.verifyMeltingSuitability(0.0), PlantException);
        CHECK_THROW(rm.applyLossCoefficient(105.0), PlantException);
    }

    TEST(MetalAlloy_Test) {
        MetalAlloy alloy("STL-20", 7850.0, 1500.0, 150.0, 0.20, 500.0, false);
        CHECK(alloy.checkWeldability());

        alloy.conductHardeningProcess(850);
        CHECK(!alloy.checkWeldability()); 

        CHECK_THROW(MetalAlloy("BAD", 7850.0, 1500.0, 150.0, -0.1, 500.0), PlantException);
        CHECK_THROW(alloy.conductHardeningProcess(100), PlantException);
        CHECK_THROW(alloy.conductHardeningProcess(1600), PlantException);
    }

    TEST(PlasticComposite_Test) {
        PlasticComposite poly("PC-GF30", 1400.0, 260.0, 80.0, "GlassFiber", 120.0, 5.0);
        CHECK_CLOSE(0.0, poly.evaluateThermalDeformation(100), 1e-5);
        CHECK_CLOSE(1.0, poly.evaluateThermalDeformation(140), 1e-5);

        poly.degradePolymer(500);
        CHECK_THROW(poly.degradePolymer(-10), PlantException);

        PlasticComposite emptyFiber("PC-0", 1200.0, 200.0, 50.0, "", 100.0, 2.0);
        CHECK_THROW(emptyFiber.evaluateThermalDeformation(150), PlantException);
    }

    TEST(MaterialBatch_Test) {
        Supplier sup("TAX-01", 90.0, 4, true);
        MaterialBatch batch("B-100", 200.0, "2026-10-01", &sup, false);

        batch.deductWeight(50.0);
        CHECK_THROW(batch.deductWeight(300.0), MaterialShortageException);
        CHECK_THROW(batch.deductWeight(-5.0), PlantException);

        batch.quarantineBatch("Surface rust");
        CHECK_THROW(batch.deductWeight(10.0), DefectiveBatchException);
        CHECK_THROW(batch.quarantineBatch(""), PlantException);

        batch.releaseToProduction();
        batch.deductWeight(150.0); 
        CHECK_THROW(batch.releaseToProduction(), MaterialShortageException);
    }

    TEST(AutoPart_Test) {
        AutoPart part("P-01", 2.5, 100.0, 200.0, false);
        CHECK_EQUAL("P-01", part.getPartId());
        CHECK(!part.isFinished());
        CHECK_CLOSE(200.0, part.getManufacturingCost(), 1e-5);

        CHECK(part.applyProtectingCoating("Zinc"));
        CHECK_CLOSE(215.0, part.getManufacturingCost(), 1e-5);
        CHECK_CLOSE(258.0, part.calculateFinalPrice(20.0), 1e-5);

        CHECK(part.markFinished());
        CHECK(part.isFinished());

        CHECK_THROW(AutoPart("BAD", -1.0, 100.0, 200.0), PlantException);
        CHECK_THROW(part.applyProtectingCoating(""), PlantException);
        CHECK_THROW(part.calculateFinalPrice(-5.0), PlantException);
    }

    TEST(EnginePiston_Test) {
        EnginePiston piston("PST-1", 1.2, 85.0, 1200.0, 84.5, 8.0, 3, 30.0, false);
        CHECK(piston.verifyThermalExpansion(100.0));
        CHECK_THROW(piston.verifyThermalExpansion(2500.0), ToleranceViolationException);

        piston.balanceWeight(1.5);
        CHECK_THROW(piston.balanceWeight(10.0), ToleranceViolationException);
        CHECK_THROW(piston.balanceWeight(0.0), PlantException);
        CHECK_THROW(EnginePiston("BAD", 1.0, 80.0, 1000.0, -5.0, 8.0, 3, 30.0), PlantException);
    }

    TEST(BrakeDisc_Test) {
        BrakeDisc disc("DSC-1", 6.0, 300.0, 1800.0, 300.0, 32, 24.0, 20.0, true);
        CHECK(disc.checkDynamicBalance());

        disc.measureBrakingWear(2.0);
        CHECK_THROW(disc.measureBrakingWear(5.0), ToleranceViolationException);
        CHECK_THROW(disc.measureBrakingWear(-1.0), PlantException);

        BrakeDisc warpedDisc("DSC-2", 6.0, 300.0, 1800.0, 300.0, 32, 24.0, 75.0, true);
        CHECK_THROW(warpedDisc.checkDynamicBalance(), ToleranceViolationException);
    }

    TEST(TransmissionGear_Test) {
        TransmissionGear gear("GR-1", 1.8, 120.0, 1400.0, 20, 2.5, 15.0, 58.0, true);
        CHECK_CLOSE(2.0, gear.calculateGearRatio(40), 1e-5);
        CHECK(gear.verifyToothMeshing(0.2));

        CHECK_THROW(gear.calculateGearRatio(0), PlantException);
        CHECK_THROW(gear.verifyToothMeshing(0.01), ToleranceViolationException);
        CHECK_THROW(gear.verifyToothMeshing(5.0), ToleranceViolationException);
        CHECK_THROW(TransmissionGear("BAD", 1.0, 100.0, 500.0, 0, 2.0, 10.0, 55.0), PlantException);
    }

    TEST(CarHeadlight_Test) {
        CarHeadlight lamp("HL-1", 1.5, 250.0, 3500.0, 3200.0, 12, 67, 12.0, true);
        CHECK(lamp.testWaterIngressProtection(4));
        lamp.adjustBeamAngle(5.0, -3.0);

        CHECK_THROW(lamp.testWaterIngressProtection(-1), PlantException);
        CHECK_THROW(lamp.adjustBeamAngle(20.0, 0.0), ToleranceViolationException);

        CarHeadlight weakLamp("HL-2", 1.5, 250.0, 3500.0, 3200.0, 12, 54, 12.0, true);
        CHECK_THROW(weakLamp.testWaterIngressProtection(5), ToleranceViolationException);
    }

    TEST(BillOfMaterials_Test) {
        BillOfMaterials bom("BOM-V8", 2, 10.0);
        bom.addRequiredComponent("Piston", 4);
        CHECK_CLOSE(16.0, bom.calculateTotalMaterialWeight(), 1e-5);
        CHECK(bom.matchInventoryAvailability());
        bom.cloneForVariant("SPORT");

        CHECK_THROW(bom.addRequiredComponent("", 2), PlantException);
        CHECK_THROW(bom.cloneForVariant(""), PlantException);

        BillOfMaterials emptyBom("BOM-0", 0, 0.0);
        CHECK_THROW(emptyBom.matchInventoryAvailability(), MaterialShortageException);
    }

    TEST(AssembledUnit_Test) {
        BillOfMaterials bom("BOM-1", 1, 4.0);
        AssembledUnit unit("UNIT-01", 0, 10.0, 2.0, false);
        AutoPart rawPart("P-RAW", 1.0, 50.0, 100.0, false);
        AutoPart finPart("P-FIN", 1.0, 50.0, 100.0, true);

        CHECK_THROW(unit.attachComponent(rawPart), AssemblyMismatchException);
        unit.attachComponent(finPart);

        CHECK(unit.validateCompleteness(bom));
        CHECK(unit.performTorqueCheck(100.0));
        CHECK_THROW(unit.performTorqueCheck(300.0), ToleranceViolationException);

        unit.disassembleDefectiveUnit();
        CHECK_THROW(unit.disassembleDefectiveUnit(), PlantException);
    }
}

SUITE(ProductionAndEquipmentSuite) {
    TEST(Machine_Test) {
        Machine m("M-01", 50.0, 10.0, 25.0, 80.0, true);
        CHECK_EQUAL("M-01", m.getMachineId());
        CHECK(m.isOperational());

        m.startEngine();
        m.logOverheatingEvent();
        m.emergencyStop();
        CHECK(!m.isOperational());
        CHECK_THROW(m.startEngine(), PlantException);

        m.serviceResetOperatingHours();
        CHECK(m.isOperational());

        Machine overheated("M-HOT", 50.0, 10.0, 95.0, 80.0, true);
        CHECK_THROW(overheated.startEngine(), MachineOverheatException);
        CHECK_THROW(overheated.logOverheatingEvent(), MachineOverheatException);
        CHECK_THROW(Machine("BAD", -10.0, 0.0, 20.0, 80.0), PlantException);
    }

    TEST(CastingFurnace_Test) {
        CastingFurnace furnace("FURN-1", 200.0, 50.0, 300.0, 1600.0, 100.0, 400.0, 50.0, 120.0, true);
        CHECK_THROW(furnace.pourMoltenMetal(20.0), PlantException); 

        furnace.heatChamber(1200.0);
        furnace.pourMoltenMetal(40.0);
        furnace.skimSlagLayer();

        CHECK_THROW(furnace.pourMoltenMetal(80.0), MaterialShortageException);
        CHECK_THROW(furnace.heatChamber(2000.0), MachineOverheatException);
    }

    TEST(StampingPress_Test) {
        StampingPress press("PR-1", 80.0, 20.0, 30.0, 90.0, 150.0, 200.0, 5.0, "DIE-01", true);
        CHECK(press.checkGuardInterlock());
        press.executeStampingStroke();
        press.ejectFormedPart();

        StampingPress noDiePress("PR-2", 80.0, 20.0, 30.0, 90.0, 150.0, 200.0, 5.0, "", true);
        CHECK(!noDiePress.checkGuardInterlock());
        CHECK_THROW(noDiePress.executeStampingStroke(), SafetyShieldOpenException);
    }

    TEST(CncMachine_Test) {
        CncMachine cnc("CNC-1", 40.0, 10.0, 40.0, 85.0, 3000.0, 150.0, 0.5, 1, true, true);
        AutoPart part("P-CNC", 2.0, 80.0, 300.0, false);

        cnc.loadGCodeProgram("PROG_01.NC");
        cnc.supplyCoolantFluid();
        cnc.executeMillingPass(part);
        CHECK(part.isFinished());

        CHECK_THROW(cnc.loadGCodeProgram(""), PlantException);

        CncMachine uncalibrated("CNC-2", 40.0, 10.0, 40.0, 85.0, 3000.0, 150.0, 5.0, 1, false, true);
        CHECK_THROW(uncalibrated.executeMillingPass(part), ToleranceViolationException);
    }

    TEST(AssemblyRobot_Test) {
        AssemblyRobot robot("ROB-1", 15.0, 30.0, 25.0, 70.0, 6, 20.0, 0.8, true, true);
        AutoPart part("P-ROB", 1.5, 50.0, 200.0, true);
        AssembledUnit unit("U-ROB", 0, 5.0, 1.0, false);

        CHECK_THROW(robot.pickAndPlaceComponent(part, unit), SafetyShieldOpenException);
        CHECK_THROW(robot.tightenFastenerJoint(50.0), SafetyShieldOpenException);

        robot.calibrateKinematicAxes();
        robot.pickAndPlaceComponent(part, unit);
        robot.tightenFastenerJoint(100.0);
        CHECK_THROW(robot.tightenFastenerJoint(500.0), ToleranceViolationException);
    }

    TEST(ToolFixture_Test) {
        ToolFixture tool("TF-01", 2, 1, 80.0);
        tool.regrindCuttingEdges();
        tool.incrementWear();
        CHECK_THROW(tool.regrindCuttingEdges(), ToolWearLimitException);
        CHECK_THROW(tool.incrementWear(), ToolWearLimitException);
        tool.retireTool();
    }

    TEST(MaintenanceRecord_Test) {
        MaintenanceRecord rec(10, "2026-10-15", "Bearing", 120.0, 500.0);
        CHECK_CLOSE(1250.0, rec.calculateMaintenanceCost(500.0), 1e-5);
        CHECK(rec.verifyPostMaintenanceSafety());
        CHECK_THROW(rec.calculateMaintenanceCost(0.0), PlantException);
    }

    TEST(SensorData_Test) {
        SensorData sensor(120.0, 0.0, "2026-10-15 12:00", 50.0);
        CHECK(sensor.detectAbnormalVibration(100.0));
        sensor.filterNoiseSpikes();
        CHECK(!sensor.detectAbnormalVibration(100.0));
        sensor.compressHistoricLogs();
        CHECK_THROW(sensor.detectAbnormalVibration(-5.0), PlantException);
    }

    TEST(ProductionLine_Test) {
        ProductionShift shift(1, "08:00", 8, 0.0, false);
        ProductionLine line(1, 0.5, 10, 50, 10, true);

        line.startShiftWorkflow(shift);
        line.resolveLineBottleneck();
        line.stepConveyorBelt();
        CHECK_CLOSE(22.0, line.evaluateShiftCompletionPercent(), 1e-5);

        ProductionLine emptyLine(2, 1.0, 0, 50, 0, false);
        CHECK_THROW(emptyLine.startShiftWorkflow(shift), ShiftNotStaffedException);
    }

    TEST(Workshop_Test) {
        Workshop ws(1, 1000.0, 85.0, 200.0);
        ws.monitorVentilation(1200.0);
        ws.evacuatePersonnel();
        CHECK(ws.calculatePowerLoad() > 150.0);
        CHECK_THROW(ws.monitorVentilation(-10.0), PlantException);
    }
}

SUITE(QualityControlSuite) {
    TEST(ToleranceLimit_Test) {
        ToleranceLimit lim(100.0, 0.5, 0.5, 1.6);
        CHECK(lim.isWithinTolerance(100.2));
        CHECK(!lim.isWithinTolerance(101.0));

        lim.tightenToleranceBand(0.5);
        CHECK(!lim.isWithinTolerance(100.4));
        CHECK_THROW(lim.tightenToleranceBand(1.5), PlantException);
    }

    TEST(QualityStandard_Test) {
        QualityStandard std("ISO-9001", 2020, 10, 6);
        CHECK(std.auditProductionProcess());
        CHECK(std.isStandardActive(2026));
        CHECK(!std.isStandardActive(2035));
        CHECK_THROW(std.isStandardActive(2015), PlantException);
    }

    TEST(DefectReport_Test) {
        AutoPart part("P-DEF", 1.0, 50.0, 400.0, false);
        DefectReport rep(1, "Porosity", 100.0, "", false);
        CHECK_CLOSE(500.0, rep.calculateScrapLoss(part), 1e-5);
        rep.routeToEngineeringReview();
    }

    TEST(ComplianceCertificate_Test) {
        ComplianceCertificate cert("CERT-01", "2026-01-01", "Auditor A", 365);
        cert.bindToShipmentLot("LOT-77");
        CHECK_THROW(cert.bindToShipmentLot(""), PlantException);

        cert.revokeCertificate("Expired test lab license");
        CHECK_THROW(cert.bindToShipmentLot("LOT-78"), PlantException);
    }

    TEST(DimensionScanner_Test) {
        DimensionScanner scanner(10.0, 102.0, 100.0, 50.0);
        AutoPart part("P-SCAN", 1.0, 100.0, 200.0, true);
        ToleranceLimit lim(100.0, 0.5, 0.5, 1.6);

        scanner.scanGeometryPointcloud(part);
        CHECK(scanner.calculateDeviationFromCad(lim) > 0.0);
        scanner.recalibrateLaserOpticalHead();

        DimensionScanner badScanner(80.0, 100.0, 100.0, 50.0);
        CHECK_THROW(badScanner.scanGeometryPointcloud(part), ToleranceViolationException);
    }

    TEST(LabStressTest_Test) {
        LabStressTest test(600.0, 200, 100, false);
        test.applyFatiguePressureCycle(); 
        CHECK(test.detectMicroCracksAcoustically());
        CHECK_THROW(test.applyFatiguePressureCycle(), DefectiveBatchException);

        LabStressTest test2(300.0, 500, 0, false);
        test2.recordDestructionThreshold(450.0);
        CHECK(test2.detectMicroCracksAcoustically());
    }

    TEST(ScrapBin_Test) {
        ScrapBin bin(10.0, 9.0, "Steel", false);
        AutoPart part("P-SCRAP", 2.0, 50.0, 100.0, false);

        bin.depositScrappedPart(part);
        CHECK_THROW(bin.depositScrappedPart(part), StorageCellFullException);

        bin.compactScrapWithPress();
        bin.emptyBinForRecycling();
        bin.depositScrappedPart(part);
    }

    TEST(QualityInspection_Test) {
        AutoPart part("P-INSP", 1.0, 100.0, 200.0, true);
        ToleranceLimit lim(10.0, 1.0, 1.0, 1.6);
        QualityInspection insp(1, "2026-10-28", 10.2, false, false);

        insp.conductVisualInspection(part);
        CHECK(insp.validateAgainstSpec(part, lim));

        QualityInspection badInsp(2, "2026-10-28", 25.0, false, false);
        CHECK_THROW(badInsp.validateAgainstSpec(part, lim), ToleranceViolationException);
    }
}

SUITE(WarehouseAndLogisticsSuite) {
    TEST(StockItem_Test) {
        StockItem item("SKU-01", 100, 20, 45);
        item.reserveStock(30);
        CHECK(item.flagSlowMovingStock(30));
        CHECK(!item.flagSlowMovingStock(60));
        CHECK_THROW(item.reserveStock(60), MaterialShortageException);
    }

    TEST(StorageCell_Test) {
        StorageCell cell("B-02", 500.0, false, "");
        CHECK(cell.verifyWeightPermissibility(300.0));
        cell.reserveCellForPlacement();
        CHECK(!cell.verifyWeightPermissibility(300.0));
        CHECK_THROW(cell.reserveCellForPlacement(), StorageCellFullException);

        cell.clearCell();
        CHECK(cell.verifyWeightPermissibility(300.0));
    }

    TEST(StorageRack_Test) {
        StorageRack rack(1, 5, 2000.0, 1500.0);
        CHECK(rack.checkStructuralIntegrity());
        rack.distributeWeightEvenly();
        rack.decommissionDamagedTier(2);
        CHECK_THROW(rack.decommissionDamagedTier(10), PlantException);

        StorageRack overloaded(2, 4, 1000.0, 1200.0);
        CHECK_THROW(overloaded.distributeWeightEvenly(), StorageCellFullException);
    }

    TEST(Warehouse_Test) {
        Warehouse wh("WH-01", 22.0, 50.0, 20, true);
        CHECK(wh.auditClimateConditions());
        CHECK_EQUAL(1, wh.locateFreeStorageCell());
        wh.executeQuarterlyInventory();
        wh.lockWarehouseZone(5);
        CHECK_EQUAL(1, wh.locateFreeStorageCell());
        CHECK_THROW(wh.lockWarehouseZone(99), PlantException);

        Warehouse badWh("WH-HOT", 45.0, 50.0, 10, true);
        CHECK_THROW(badWh.executeQuarterlyInventory(), PlantException);
    }

    TEST(TransportPallet_Test) {
        TransportPallet pallet("PAL-01", 20.0, 100.0, false);
        pallet.loadCartonBox(50.0);
        pallet.wrapWithStretchFilm();
        CHECK_CLOSE(170.0, pallet.verifyTotalGrossWeight(), 1e-5);
        CHECK_THROW(pallet.loadCartonBox(2000.0), StorageCellFullException);
    }

    TEST(Forklift_Test) {
        Forklift fl(80.0, 5.0, 1000.0, false);
        TransportPallet pallet("PAL-02", 20.0, 200.0, false);

        fl.elevateForks(3.0);
        fl.transportPalletToDispatch(pallet);
        fl.dockToChargingStation();

        CHECK_THROW(fl.elevateForks(10.0), PlantException);
        TransportPallet heavy("PAL-H", 50.0, 1200.0, false);
        CHECK_THROW(fl.transportPalletToDispatch(heavy), PlantException);
    }

    TEST(Supplier_Test) {
        Supplier sup("TAX-99", 80.0, 6, true);
        CHECK_CLOSE(77.0, sup.evaluateSupplierScorecard(), 1e-5);
        sup.penalizeLateDelivery(2);
        sup.renewSupplyAgreement(2);

        CHECK_THROW(sup.penalizeLateDelivery(-1), PlantException);
        sup.penalizeLateDelivery(30); // Рейтинг падает ниже 30.0
        CHECK_THROW(sup.renewSupplyAgreement(1), PlantException);
    }

    TEST(SupplyOrder_Test) {
        SupplyOrder po(101, 500.0, "2026-11-15", 10000.0, false, false);
        CHECK_THROW(po.confirmReceiptAtGate(), PlantException);

        po.authorizePurchaseBudget();
        po.confirmReceiptAtGate();
        CHECK(po.trackInTransitLocation().find("Delivered") != std::string::npos);
        CHECK_CLOSE(200.0, po.calculateLatePenalties(2), 1e-5);
    }

    TEST(ShipmentOrder_Test) {
        ShipmentOrder sh(501, 3, "Казань", "В456ОР16", 150000.0, false);
        CHECK_THROW(sh.sealTruckTrailer("SEAL-1"), PlantException);

        CHECK(sh.validateLotBarcodes());
        sh.sealTruckTrailer("SEAL-1");
        CHECK(sh.generateBillOfLading().find("BOL #501") != std::string::npos);
        sh.closeFulfilledShipment();
    }

    TEST(DeliveryRoute_Test) {
        DeliveryRoute route(1000.0, 300.0, 1500.0, 14.0);
        route.optimizeWaypoints();
        CHECK_CLOSE(9.2, route.recalculateEstimatedArrival(100.0), 1e-5);
        route.recordTollExpense(500.0);
        CHECK_THROW(route.recalculateEstimatedArrival(0.0), PlantException);
    }
}

SUITE(OrdersAndManagementSuite) {
    TEST(Employee_Test) {
        Employee emp("E-01", "Иванов И.И.", 500.0, 160.0, false);
        CHECK_THROW(emp.registerTurnstileEntry(), UnauthorizedOperationException);

        CHECK(emp.passMandatorySafetyTest());
        CHECK(emp.registerTurnstileEntry().find("E-01") != std::string::npos);
        CHECK_CLOSE(80000.0, emp.calculateMonthlyGrossSalary(), 1e-5);
        CHECK_THROW(Employee("BAD", "Имя", -100.0, 160.0), PlantException);
    }

    TEST(MachineOperator_Test) {
        Machine m("M-OP", 30.0, 10.0, 25.0, 80.0, true);
        MachineOperator op("E-02", "Петров П.П.", 400.0, 160.0, 4, "CNC", 0, false, true);

        op.executeSafetySelfCheck(); 
        op.setupMachineTooling(m);
        op.logProducedQuantity(15);

        MachineOperator lowGrade("E-03", "Сидоров", 350.0, 160.0, 2, "CNC", 0, true, true);
        CHECK_THROW(lowGrade.setupMachineTooling(m), UnauthorizedOperationException);
    }

    TEST(QualityInspector_Test) {
        QualityInspector qi("E-04", "Смирнова", 450.0, 160.0, 2, 777, 0, 0, true);
        AutoPart finPart("P-OK", 1.0, 50.0, 100.0, true);
        AutoPart rawPart("P-RAW", 1.0, 50.0, 100.0, false);

        CHECK_CLOSE(777.0, qi.performMicrometerVerification(finPart), 1e-5);
        CHECK_THROW(qi.performMicrometerVerification(rawPart), ToleranceViolationException);

        qi.issueRejectionTag(rawPart, "Scratch");
        CHECK(qi.auditShiftDefectRate() > 0.0);
    }

    TEST(PlantEngineer_Test) {
        PlantEngineer eng("E-05", "Козлов", 600.0, 160.0, "Robotics", 0, true, 3, true);
        CncMachine cnc("CNC-E", 40.0, 20.0, 30.0, 85.0, 3000.0, 100.0, 5.0, 1, true, true);
        IncidentReport inc;

        eng.optimizeCuttingParameters(cnc);
        CHECK(eng.schedulePreventiveOverhaul(cnc));
        eng.investigateProductionIncident(inc);

        PlantEngineer jrEng("E-06", "Стажер", 300.0, 160.0, "CAD", 0, false, 1, false);
        CHECK_THROW(jrEng.optimizeCuttingParameters(cnc), UnauthorizedOperationException);
        CHECK_THROW(jrEng.investigateProductionIncident(inc), UnauthorizedOperationException);
    }

    TEST(ShiftSupervisor_Test) {
        ShiftSupervisor sup("E-07", "Морозов", 650.0, 160.0, 1, 10, 0, 95.0, false, true);
        Employee worker("E-08", "Рабочий", 350.0, 160.0, false);

        CHECK(sup.conductPreShiftSafetyMeeting());
        sup.reassignWorkerStation(worker, 3);
        CHECK(sup.signShiftHandoverSheet());
        sup.haltSectorDueToHazard();
    }

    TEST(WarehouseWorker_Test) {
        WarehouseWorker ww("E-09", "Кладовщик", 380.0, 160.0, 101, true, 10.0, "Z-1", true);
        StockItem item("SKU-WW", 50, 5, 20);

        ww.scanItemBarcode(item);
        ww.packCrateForAssembly();
        CHECK(ww.reportDamagedPackaging(item));
    }

    TEST(ProductionShift_Test) {
        ProductionShift shift(1, "20:00", 10, 1.2, false);
        shift.validateMinimumStaffing(20);
        CHECK(shift.computeShiftOvertimeHours());
        shift.logShiftDowntime(30);
        shift.closeShiftAccounting();

        CHECK_THROW(shift.validateMinimumStaffing(100), ShiftNotStaffedException);
        CHECK_THROW(shift.logShiftDowntime(700), PlantException);
    }

    TEST(ManufacturingOrder_Test) {
        ManufacturingOrder order(10, "PST-1", 100, 5, "2026-11-01", 50);
        ProductionLine line(1, 1.5, 5, 100, 0, false);

        CHECK_CLOSE(27.5, order.calculateRequiredWorkHours(0.5), 1e-5);
        order.allocateLineResources(line);
        order.incrementCompletedBatch(20);
        CHECK_THROW(order.checkDeadlineRisk(0), OrderDeadlineExceededException);
    }

    TEST(Client_Test) {
        Client client(1, "АвтоВАЗ", 1000000.0, 30);
        CHECK(client.verifyCreditCapacity(500000.0));
        CHECK_CLOSE(15.0, client.applyVolumeDiscount(1200), 1e-5);
        client.updateContractTier("VIP");
        CHECK(client.verifyCreditCapacity(1400000.0));
    }

    TEST(ClientContract_Test) {
        ClientContract contract(77, "2026-01-01", 500000.0, 0.5, false);
        CHECK_CLOSE(0.0, contract.computeDelayDamages(5), 1e-5);

        contract.activateContract();
        CHECK_CLOSE(12500.0, contract.computeDelayDamages(5), 1e-5);
        contract.terminateContractDueToDefault();
        CHECK_THROW(contract.terminateContractDueToDefault(), PlantException);
    }

    TEST(PlantFinance_Test) {
        PlantFinance fin(500000.0, 300000.0, 50000.0, 100000.0);
        SupplyOrder po(1, 100.0, "2026-11-10", 5000.0, true, false);
        ShipmentOrder sh(1, 2, "Минск", "1234AB-7", 80000.0, true);

        CHECK_CLOSE(200000.0, fin.disbursePayrollFunds(100000.0), 1e-5);
        CHECK_CLOSE(499000.0, fin.paySupplyInvoice(po), 1e-5);
        CHECK_CLOSE(504000.0, fin.receiveClientPayment(sh), 1e-5);
        CHECK_CLOSE(50.0, fin.calculateProfitMargin(200000.0, 50000.0), 1e-5);
    }
}

int main() {
    return UnitTest::RunAllTests();
}