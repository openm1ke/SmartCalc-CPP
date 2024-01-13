/*!
 * \file
 * \brief Заголовочный файл с описанием класса
 *
 * Данный файл содержит в себе определения класса view
 */
#ifndef SRC_VIEW_S21_VIEW_H_
#define SRC_VIEW_S21_VIEW_H_

#include <QApplication>
#include <QDialog>
#include <QLayout>
#include <QMainWindow>
#include <QMessageBox>
#include <QQuickStyle>
#include <QRegularExpression>
#include <QValidator>
#include <QVector>

#include "../controller/s21_controller.h"
#include "ui_view.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {
class view : public QMainWindow {
  Q_OBJECT

 public:
  /** Конструктор представления
   * \param[in] controller указатель на контроллер
   */
  explicit view(s21::controller *controller = nullptr);
  /** Деструктор представления */
  ~view();

 private slots:
  void button_number();
  void button_operation();
  void on_button_del_clicked();
  void on_button_equal_clicked();
  void on_button_ac_clicked();
  void on_button_unary_clicked();
  void on_button_x_clicked();
  void on_button_plot_clicked();
  void on_button_clear_plot_clicked();

 private:
  Ui::MainWindow *ui;
  s21::controller *controller;
  bool check_range();
  double x_min;
  double x_max;
  double y_min;
  double y_max;
};
}  // namespace s21

#endif  // SRC_VIEW_S21_VIEW_H_
