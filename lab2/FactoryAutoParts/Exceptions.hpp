#pragma once
#include <stdexcept>
#include <string>

class PlantException : public std::runtime_error{
public: 
    explicit PlantException(const std::string& message) 
        : std::runtime_error(message){}
};

class MaterialShortageException : public PlantException {
public:
    explicit MaterialShortageException(const std::string& message = "Material shortage.")
        : PlantException(message) {}
};

class DefectiveBatchException : public PlantException {
public:
    explicit DefectiveBatchException(const std::string& message = "Defective batch.")
        : PlantException(message) {}
};

class MachineOverheatException : public PlantException {
public:
    explicit MachineOverheatException(const std::string& message = "Critical overheat.")
        : PlantException(message) {}
};

class ToolWearLimitException : public PlantException {
public:
    explicit ToolWearLimitException(const std::string& message = "Critical tool wear limit.")
        : PlantException(message){}
};

class ToleranceViolationException : public PlantException {
public:
    explicit ToleranceViolationException(const std::string& message = "Tolerance violation.")
        : PlantException(message){}
};

class StorageCellFullException : public PlantException {
public:
    explicit StorageCellFullException(const std::string& message = "Storage cell is full.")
        : PlantException(message){}
};

class ShiftNotStaffedException : public PlantException {
public:
    explicit ShiftNotStaffedException(const std::string& message = "Shift not staffed.")
        : PlantException(message){}
};

class UnauthorizedOperationException : public PlantException {
public:
    explicit UnauthorizedOperationException(const std::string& message = "Unauthorized operator.")
        : PlantException(message){}
};

class OrderDeadlineExceededException : public PlantException {
public:
    explicit OrderDeadlineExceededException(const std::string& message = "Order deadline exceeded.")
        : PlantException(message){}
};

class AssemblyMismatchException : public PlantException {
public:
    explicit AssemblyMismatchException(const std::string& message = "Assembly mismatch.")
        : PlantException(message){}
};

class SafetyShieldOpenException : public PlantException {
public:
    explicit SafetyShieldOpenException(const std::string& message = "Safety shield open.")
        : PlantException(message){}
};