#include "view/s21_view.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  a.setStyle("Fusion");

  s21::model model;
  s21::controller controller(&model);
  s21::view view(&controller);

  view.setWindowTitle("SmartCalc_v2.0");
  view.setFixedSize(635, 355);
  view.show();

  return a.exec();
}
