#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>
#include <sstream>

// Базовый класс для всех исключений
class ShapeException : public std::exception {
private:
    std::string message;
public:
    ShapeException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// Ошибка выхода за границы экрана
class OutOfScreenError : public ShapeException {
public:
    OutOfScreenError(int x, int y, int maxX, int maxY)
        : ShapeException(buildMessage(x, y, maxX, maxY)) {}
private:
    static std::string buildMessage(int x, int y, int maxX, int maxY) {
        std::ostringstream oss;
        oss << "Точка (" << x << "," << y << ") вне экрана! "
            << "Экран: 0.." << maxX-1 << " x 0.." << maxY-1;
        return oss.str();
    }
};

// Ошибка некорректных параметров
class InvalidParameterError : public ShapeException {
public:
    InvalidParameterError(const std::string& paramName, int value)
        : ShapeException(buildMessage(paramName, value)) {}
    InvalidParameterError(const std::string& msg)
        : ShapeException(msg) {}
private:
    static std::string buildMessage(const std::string& name, int val) {
        std::ostringstream oss;
        oss << "Некорректный параметр " << name << " = " << val
            << ". Значение должно быть положительным.";
        return oss.str();
    }
};

// Ошибка нехватки места на экране
class NoSpaceError : public ShapeException {
public:
    NoSpaceError(const std::string& shapeName, const std::string& operation)
        : ShapeException(buildMessage(shapeName, operation)) {}
private:
    static std::string buildMessage(const std::string& name, const std::string& op) {
        std::ostringstream oss;
        oss << "Невозможно выполнить " << op << " для " << name
            << ": фигура не помещается на экране";
        return oss.str();
    }
};

// Ошибка повторного поворота
class AlreadyRotatedError : public ShapeException {
public:
    AlreadyRotatedError(const std::string& shapeName)
        : ShapeException(buildMessage(shapeName)) {}
private:
    static std::string buildMessage(const std::string& name) {
        std::ostringstream oss;
        oss << "Фигура " << name << " уже была повернута! "
            << "Повторный поворот запрещен.";
        return oss.str();
    }
};

#endif
