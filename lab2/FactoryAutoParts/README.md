# Лабораторная работа №2. Крупный ООП-проект

## Предметная область: Завод по производству автокомпонентов (`FactoryAutoParts`)

Проект представляет собой объектную модель управления производственным циклом завода автодеталей: от поставки и входного контроля сырья до выплавки, механообработки на станках с ЧПУ, роботизированной сборки, метрологического контроля ОТК, складской логистики и финансовых расчётов с контрагентами.

### Структура модулей

- `Exceptions.hpp` — иерархия из 12 пользовательских классов исключений.
- `MaterialAndDetails.hpp` / `.cpp` — сырьё, сплавы, полимеры, партии материалов, автодетали, спецификации (BOM) и сборочные узлы (11 классов).
- `ProductionAndEquipment.hpp` / `.cpp` — промышленное оборудование, станки, оснастка, датчики, конвейерные линии и цеха (10 классов).
- `QualityControl.hpp` / `.cpp` — допуски, стандарты качества, лазерное 3D-сканирование, стресс-тесты, отбраковка и сертификация (8 классов).
- `WarehouseAndLogistics.hpp` / `.cpp` — складской учёт, ячейки, стеллажи, паллеты, погрузчики, поставщики и отгрузки (10 классов).
- `OrdersAndManagement.hpp` / `.cpp` — персонал завода, производственные смены, заказы, клиенты, договоры и финансы предприятия (11 классов).

---

## Описание классов

Формат строки: `Класс | число полей | число методов | связанные классы`.

| Класс | Поля | Методы | Ассоциации (связанные классы) |
|---|---|---|---|
| AssembledUnit | 5 | 4 | AutoPart, BillOfMaterials |
| AssemblyRobot | 5 | 3 | Machine, AutoPart, AssembledUnit |
| AutoPart | 5 | 2 | — |
| BillOfMaterials | 3 | 4 | — |
| BrakeDisc | 4 | 2 | AutoPart |
| CarHeadlight | 4 | 2 | AutoPart |
| CastingFurnace | 4 | 3 | Machine |
| Client | 4 | 3 | — |
| ClientContract | 5 | 3 | Client |
| CncMachine | 5 | 3 | Machine, AutoPart |
| ComplianceCertificate | 4 | 2 | — |
| DefectReport | 5 | 2 | AutoPart |
| DeliveryRoute | 4 | 3 | — |
| DimensionScanner | 4 | 3 | AutoPart, ToleranceLimit |
| Employee | 5 | 3 | — |
| EnginePiston | 4 | 2 | AutoPart |
| Forklift | 4 | 3 | TransportPallet |
| LabStressTest | 4 | 3 | — |
| Machine | 6 | 4 | — |
| MachineOperator | 4 | 3 | Employee, Machine |
| MaintenanceRecord | 5 | 2 | Machine |
| ManufacturingOrder | 6 | 4 | ProductionLine |
| MaterialBatch | 5 | 3 | Supplier |
| MetalAlloy | 3 | 2 | RawMaterial |
| PlantEngineer | 4 | 3 | Employee, CncMachine, Machine, IncidentReport |
| PlantFinance | 4 | 4 | SupplyOrder, ShipmentOrder |
| PlasticComposite | 3 | 2 | RawMaterial |
| ProductionLine | 6 | 4 | ProductionShift |
| ProductionShift | 5 | 4 | — |
| QualityInspection | 5 | 3 | AutoPart, ToleranceLimit, DefectReport |
| QualityInspector | 4 | 3 | Employee, AutoPart |
| QualityStandard | 4 | 2 | — |
| RawMaterial | 4 | 3 | — |
| ScrapBin | 4 | 3 | AutoPart |
| SensorData | 4 | 3 | — |
| ShiftSupervisor | 5 | 4 | Employee |
| ShipmentOrder | 6 | 4 | — |
| StampingPress | 4 | 3 | Machine |
| StockItem | 4 | 2 | — |
| StorageCell | 4 | 3 | — |
| StorageRack | 4 | 3 | StorageCell |
| Supplier | 4 | 3 | — |
| SupplyOrder | 6 | 4 | Supplier |
| ToleranceLimit | 4 | 2 | — |
| ToolFixture | 4 | 3 | — |
| TransmissionGear | 4 | 2 | AutoPart |
| TransportPallet | 4 | 3 | — |
| Warehouse | 5 | 4 | StorageRack, StorageCell |
| WarehouseWorker | 4 | 3 | Employee, StockItem |
| Workshop | 4 | 3 | ProductionLine, Machine |

---

## Исключения (12)

- `PlantException` (базовое исключение завода)
- `MaterialShortageException`
- `DefectiveBatchException`
- `MachineOverheatException`
- `ToolWearLimitException`
- `ToleranceViolationException`
- `StorageCellFullException`
- `ShiftNotStaffedException`
- `UnauthorizedOperationException`
- `OrderDeadlineExceededException`
- `AssemblyMismatchException`
- `SafetyShieldOpenException`

---

## Итоговая статистика проекта

| Показатель | Значение по проекту | Требование задания |
|---|---|---|
| Предметные классы | 50 (62 с учётом классов исключений) | >50 |
| Поля | 222 | >150 |
| Поведения (уникальные методы) | 150 | >100 |
| Ассоциации | 42 | >30 |
| Исключения | 12 | >12 |