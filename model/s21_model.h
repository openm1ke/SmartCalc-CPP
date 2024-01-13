/*!
 * \file
 * \brief Заголовочный файл с описанием класса
 *
 * Данный файл содержит в себе определения класса model
 */
#ifndef SRC_MODEL_S21_MODEL_H_
#define SRC_MODEL_S21_MODEL_H_

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
// NOLINTNEXTLINE(build/c++11)
#include <regex>

/*!
 * Константа для сравнения двух вещественных чисел
 */
#define S21_EPSILON 0.0000001

namespace s21 {
class model {
 private:
  /** хранит исходное выражение */
  std::string infix_;
  /** хранит преобразованное в постфиксное выражение */
  std::string postfix_;
  /** хранит нормализованное (проверенное на ошибки) выражение */
  std::string normalized_;
  double var_x_;
  double x_min_;
  double x_max_;
  double y_min_;
  double y_max_;
  double step_;
  /*!
  \brief Проверяет корректность ввода выражения в инфиксной записи
  */
  void normalized_to_postfix();
  /*!
  \brief Очищает переменные модели
  */
  void clear_fields();
  /*!
  \brief Заменяет знак x в выражении
  */
  void replace_x();
  /*!
  \brief Вспомогательная функция для парсинга числа в экспоненциальной форме
  */
  int parse_dot(std::string ::iterator* it);
  /*!
  \brief Вторая функция для парсинга экспоненциального числа
  */
  int parse_e(std::string::iterator* it);
  /*!
  \brief Функция вычисления результата двух чисел
  */
  double compute(double one, double two, char op);
  /*!
  \brief Функция запуска вычисления всего выражения
  */
  std::string execute_postfix();
  /*!
  \brief Функция нормализации выражения проверяет на ошибки
  */
  int normalize_infix();
  /*!
  \brief Опеределяет приоритет операции
  */
  int check_priority(char c);
  /*!
  \brief Функция нахождения тригонометрической операции в строке
  */
  int parse_operation(std::string::iterator* it);
  /*!
  \brief Функция дополнительной проверки на ошибки после нормализации
  */
  int validate_string();
  /*!
  \brief Функция замены найденой операции в выражении на один символ
  */
  char operation_code(int i);

 public:
  /*!
  \brief Конструктор
  */
  model();
  /*!
  \brief Функция установки выражения
  \param[in] expression Исходная строка инфиксного выражения
  */
  void setExpression(std::string expression);
  /*!
  \brief Функция установки переменной x
  \param[in] x установка значения переменной x
  */
  void setVarX(double x);
  /*!
  \brief Функция установки шага для построения графика
  \param[in] step установка значения переменной шага
  */
  void setStep(double step);
  /*!
  \brief Функция вывода строки с ошибкой по индексу
  \param[in] i индекс ошибки
  */
  std::string view_error(int i);
  /*!
  \brief Основная функция запуска вычисления
  */
  std::string calculate();
  /*!
  \brief Функция удаления всех пробелов и приведения к нижнему регистру
  \param[in] str исходная строка
  */
  std::string trim_and_lower(const std::string& str);
  /*!
  \brief Функция установки границ для построения графика
  \param[in] x_min, x_max, y_min, y_max установка границ
  */
  void setPlotRange(double x_min, double x_max, double y_min, double y_max);
  /*!
  \brief Функция расчета графика
  */
  std::pair<std::vector<double>, std::vector<double>> plot();
};
}  // namespace s21

#endif  // SRC_MODEL_S21_MODEL_H_
