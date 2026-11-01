#include <iostream>
#include <iomanip>
#include "Exceptions.hpp"
#include "MaterialAndDetails.hpp"
#include "ProductionAndEquipment.hpp"
#include "QualityControl.hpp"
#include "WarehouseAndLogistics.hpp"
#include "OrdersAndManagement.hpp"

void printHeader(const std::string& title) {
    std::cout << "\n========================================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "========================================================\n";
}

int main() {
    std::cout << std::fixed << std::setprecision(2);

    try {
        printHeader("1. ПОСТАВКА СЫРЬЯ И СКЛАДСКОЙ УЧЕТ");

        Supplier steelSupplier("TAX-77012345", 95.0, 5, true);
        MetalAlloy steelAlloy("STL-40X", 7850.0, 1450.0, 120.0, 0.20, 650.0, false);

        std::cout << "[Сырье] Марка стали: " << steelAlloy.getMaterialId()
                  << " | Свариваемость: " << (steelAlloy.checkWeldability() ? "Да" : "Нет") << "\n";

        steelAlloy.conductHardeningProcess(850);
        std::cout << "[Термообработка] Закалка при 850 C выполнена успешно.\n";

        MaterialBatch batch("BATCH-2026-01", 500.0, "2026-10-28", &steelSupplier, false);
        batch.deductWeight(120.0);
        std::cout << "[Партия] Списано 120 кг стали в цех. Стоимость списанного сырья: "
                  << steelAlloy.calculateBatchCost(120.0) << " руб.\n";

        Warehouse mainWarehouse("WH-NORTH", 20.0, 45.0, 50, true);
        StorageCell cellA1("A-01-01", 1000.0, false, "2026-10-27");
        if (mainWarehouse.auditClimateConditions() && cellA1.verifyWeightPermissibility(380.0)) {
            cellA1.reserveCellForPlacement();
            std::cout << "[Склад] Климат в норме. Ячейка A-01-01 зарезервирована под остаток партии.\n";
        }

        
        printHeader("2. ЗАПУСК СМЕНЫ И МЕХАНООБРАБОТКА В ЦЕХУ");

        ShiftSupervisor supervisor("EMP-001", "Иванов И.И.", 600.0, 160.0, 1, 12, 0, 100.0, false, true);
        MachineOperator cncOperator("EMP-015", "Петров П.П.", 450.0, 168.0, 4, "CNC-Milling", 0, true, false);
        PlantEngineer engineer("EMP-005", "Сидоров С.С.", 550.0, 160.0, "CAD/CAM", 2, true, 3, true);

        supervisor.conductPreShiftSafetyMeeting();
        cncOperator.passMandatorySafetyTest();
        cncOperator.executeSafetySelfCheck();
        std::cout << "[Турникет] " << cncOperator.registerTurnstileEntry() << "\n";

        ProductionShift morningShift(101, "08:00", 8, 0.0, false);
        ProductionLine line1(1, 2.5, 6, 100, 0, false);
        line1.startShiftWorkflow(morningShift);
        std::cout << "[Конвейер] Линия #1 запущена в работу.\n";

        CastingFurnace furnace("FURN-01", 150.0, 120.0, 400.0, 1800.0, 300.0, 600.0, 80.0, 45.0, true);
        furnace.startEngine();
        furnace.heatChamber(1500.0);
        furnace.pourMoltenMetal(50.0);
        std::cout << "[Печь] Выплавка и заливка 50 кг металла завершена.\n";

        CncMachine cnc("CNC-01", 45.0, 50.0, 35.0, 90.0, 4500.0, 250.0, 6.0, 1, true, true);
        cnc.startEngine();
        cncOperator.setupMachineTooling(cnc);
        engineer.optimizeCuttingParameters(cnc);
        cnc.loadGCodeProgram("PISTON_V8_OP10.NC");

        EnginePiston piston("PART-PST-01", 1.2, 85.0, 1500.0, 84.9, 8.5, 3, 32.0, false);
        BrakeDisc disc("PART-DSC-01", 6.5, 320.0, 2200.0, 320.0, 36, 28.0, 15.0, false);

        cnc.executeMillingPass(piston);
        cnc.executeMillingPass(disc);
        piston.applyProtectingCoating("Molybdenum");
        cncOperator.logProducedQuantity(2);
        line1.stepConveyorBelt();

        std::cout << "[ЧПУ] Детали " << piston.getPartId() << " и " << disc.getPartId()
                  << " успешно обработаны. Статус готовности поршня: "
                  << (piston.isFinished() ? "Готов" : "В работе") << "\n";

        
        
        printHeader("3. КОНТРОЛЬ КАЧЕСТВА (ОТК) И РОБОТИЗИРОВАННАЯ СБОРКА");

        QualityInspector inspector("EMP-022", "Смирнова А.В.", 480.0, 160.0, 3, 9901, 10, 0, true);
        ToleranceLimit pistonTolerance(85.0, 0.1, 0.1, 1.6);
        QualityInspection inspection(501, "2026-10-28 11:30", 85.02, false, false);

        inspector.performMicrometerVerification(piston);
        if (inspection.validateAgainstSpec(piston, pistonTolerance) && disc.checkDynamicBalance()) {
            std::cout << "[ОТК] Детали прошли проверку геометрии и балансировки. Штамп ОТК поставлен.\n";
        }

        BillOfMaterials bom("SPEC-ENG-BLOCK", 2, 5.0);
        AssembledUnit engineSubUnit("UNIT-V8-001", 0, 15.0, 6.0, false);
        AssemblyRobot robot("ROB-01", 12.0, 200.0, 28.0, 75.0, 6, 25.0, 0.05, false, true);

        robot.calibrateKinematicAxes();
        robot.pickAndPlaceComponent(piston, engineSubUnit);
        robot.pickAndPlaceComponent(disc, engineSubUnit);
        robot.tightenFastenerJoint(85.0);

        if (engineSubUnit.performTorqueCheck(85.0) && engineSubUnit.validateCompleteness(bom)) {
            std::cout << "[Сборка] Агрегат UNIT-V8-001 успешно собран роботом и прошел проверку по BOM.\n";
        }

        
        printHeader("4. ЛОГИСТИКА, ОТГРУЗКА И ФИНАНСОВЫЙ ОТЧЕТ");

        StockItem stockItem("SKU-UNIT-V8", 20, 2, 5);
        WarehouseWorker whWorker("EMP-040", "Кузнецов Д.Д.", 380.0, 160.0, 701, true, 25.0, "ZONE-A", true);
        whWorker.scanItemBarcode(stockItem);
        whWorker.packCrateForAssembly();

        TransportPallet pallet("PAL-9001", 25.0, 120.0, false);
        pallet.loadCartonBox(45.0);
        Forklift forklift(85.0, 4.5, 1500.0, false);
        forklift.elevateForks(2.0);
        forklift.transportPalletToDispatch(pallet);
        std::cout << "[Погрузка] Паллета упакована и доставлена в зону экспедиции. Вес брутто: "
                  << pallet.verifyTotalGrossWeight() << " кг.\n";

        ShipmentOrder shipment(8001, 4, "Москва", "А123ВС77", 250000.0, false);
        shipment.validateLotBarcodes();
        shipment.sealTruckTrailer("SEAL-778899");
        std::cout << "[Экспедиция] Сформирована накладная: " << shipment.generateBillOfLading() << "\n";

        PlantFinance finance(1500000.0, 800000.0, 120000.0, 450000.0);
        double operatorSalary = cncOperator.calculateMonthlyGrossSalary();
        finance.disbursePayrollFunds(operatorSalary);
        double newReserve = finance.receiveClientPayment(shipment);
        double margin = finance.calculateProfitMargin(500000.0, 250000.0);

        std::cout << "[Финансы] Выплачена зарплата оператору: " << operatorSalary << " руб.\n";
        std::cout << "[Финансы] Получена оплата по отгрузке. Операционный резерв: " << newReserve
                  << " руб. Маржинальность: " << margin << "%\n";

    } catch (const PlantException& e) {
        std::cerr << "[КРИТИЧЕСКАЯ ОШИБКА В ШТАТНОМ ЦИКЛЕ]: " << e.what() << "\n";
    }

    
    printHeader("5. СТРЕСС-ТЕСТ СИСТЕМЫ: ПЕРЕХВАТ АВАРИЙНЫХ СИТУАЦИЙ");

    
    try {
        Employee rookie("EMP-999", "Новичок Н.Н.", 300.0, 10.0, false);
        rookie.registerTurnstileEntry();
    } catch (const UnauthorizedOperationException& e) {
        std::cout << "[Перехвачено UnauthorizedOperationException]: " << e.what() << "\n";
    }

    
    try {
        MaterialBatch smallBatch("BATCH-SMALL", 15.0, "2026-10-28");
        smallBatch.deductWeight(50.0); 
    } catch (const MaterialShortageException& e) {
        std::cout << "[Перехвачено MaterialShortageException]: " << e.what() << "\n";
    }

    
    try {
        AutoPart rawPart("RAW-GEAR-01", 2.0, 100.0, 800.0, false); 
        AssembledUnit unit("UNIT-FAIL", 0, 10.0, 2.0, false);
        unit.attachComponent(rawPart);
    } catch (const AssemblyMismatchException& e) {
        std::cout << "[Перехвачено AssemblyMismatchException]: " << e.what() << "\n";
    }

    
    try {
        ToolFixture cutter("FIX-MILL-09", 5, 5, 12.0); 
        cutter.incrementWear();
    } catch (const ToolWearLimitException& e) {
        std::cout << "[Перехвачено ToolWearLimitException]: " << e.what() << "\n";
    }

   
    try {
        ManufacturingOrder urgentOrder(404, "PART-PST-01", 500, 10, "2026-10-28", 120);
        urgentOrder.checkDeadlineRisk(0); 
    } catch (const OrderDeadlineExceededException& e) {
        std::cout << "[Перехвачено OrderDeadlineExceededException]: " << e.what() << "\n";
    }

    std::cout << "\n[Система] Демонстрация работы завода успешно завершена!\n";
    return 0;
}