/*!
 * \file
 * \brief Заголовочный файл с описанием класса
 *
 * Данный файл содержит в себе определения класса controller
 */
#ifndef SRC_CONTROLLER_S21_CONTROLLER_H_
#define SRC_CONTROLLER_S21_CONTROLLER_H_

#include "../model/s21_model.h"

namespace s21 {
class controller {
 private:
  /** указатель на модель */
  s21::model *model_;

 public:
  /** конструктор контроллера
   *\param[in] m указатель на модель
   */
  explicit controller(s21::model *m) : model_(m) {}
  /** вычисляет выражение
   * \param[in] expression выражение для вычисления
   */
  std::string processExpression(std::string expression);
  /** устанавливает значение x
   * \param[in] x значение для x
   */
  void setVarX(double x);
  /** установляет диапазон для построения графика
   * \param[in] x_min минимальное значение по оси x
   * \param[in] x_max максимальное значение по оси x
   * \param[in] y_min минимальное значение по оси y
   * \param[in] y_max максимальное значение по оси y
   */
  void setPlotRange(double x_min, double x_max, double y_min, double y_max);
  /** вычисляет значения для построения графика
   * \param[in] expression выражение для вычисления
   * \return std::pair<std::vector<double>, std::vector<double>> пара векторов
   * со значениями
   */
  std::pair<std::vector<double>, std::vector<double>> plot(
      std::string expression);
};
}  // namespace s21

#endif  // SRC_CONTROLLER_S21_CONTROLLER_H_
