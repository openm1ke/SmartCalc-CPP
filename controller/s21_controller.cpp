#include "s21_controller.h"

std::string s21::controller::processExpression(std::string expression) {
  model_->setExpression(expression);
  return model_->calculate();
}

void s21::controller::setVarX(double x) { model_->setVarX(x); }

void s21::controller::setPlotRange(double x_min, double x_max, double y_min,
                                   double y_max) {
  model_->setPlotRange(x_min, x_max, y_min, y_max);
}

std::pair<std::vector<double>, std::vector<double>> s21::controller::plot(
    std::string expression) {
  model_->setExpression(expression);
  return model_->plot();
}
