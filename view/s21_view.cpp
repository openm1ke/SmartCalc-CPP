#include "s21_view.h"

s21::view::view(s21::controller* controller)
    : QMainWindow(), ui(new Ui::MainWindow) {
  this->controller = controller;
  ui->setupUi(this);
  ui->button_mul->setProperty("operation", "*");
  ui->button_div->setProperty("operation", "/");
  ui->button_plus->setProperty("operation", "+");
  ui->button_minus->setProperty("operation", "-");
  ui->button_sqrt->setProperty("operation", "sqrt(");
  ui->button_sin->setProperty("operation", "sin(");
  ui->button_cos->setProperty("operation", "cos(");
  ui->button_tan->setProperty("operation", "tan(");
  ui->button_asin->setProperty("operation", "asin(");
  ui->button_acos->setProperty("operation", "acos(");
  ui->button_atan->setProperty("operation", "atan(");
  ui->button_log->setProperty("operation", "log(");
  ui->button_ln->setProperty("operation", "ln(");
  ui->button_mod->setProperty("operation", "mod");
  ui->button_open_br->setProperty("operation", "(");
  ui->button_close_br->setProperty("operation", ")");
  ui->button_power->setProperty("operation", "^");
  connect(ui->button_0, SIGNAL(clicked()), SLOT(button_number()));
  connect(ui->button_1, SIGNAL(clicked()), SLOT(button_number()));
  connect(ui->button_2, SIGNAL(clicked()), SLOT(button_number()));
  connect(ui->button_3, SIGNAL(clicked()), SLOT(button_number()));
  connect(ui->button_4, SIGNAL(clicked()), SLOT(button_number()));
  connect(ui->button_5, SIGNAL(clicked()), SLOT(button_number()));
  connect(ui->button_6, SIGNAL(clicked()), SLOT(button_number()));
  connect(ui->button_7, SIGNAL(clicked()), SLOT(button_number()));
  connect(ui->button_8, SIGNAL(clicked()), SLOT(button_number()));
  connect(ui->button_9, SIGNAL(clicked()), SLOT(button_number()));
  connect(ui->button_dot, SIGNAL(clicked()), SLOT(button_number()));
  connect(ui->button_mul, SIGNAL(clicked()), SLOT(button_operation()));
  connect(ui->button_div, SIGNAL(clicked()), SLOT(button_operation()));
  connect(ui->button_minus, SIGNAL(clicked()), SLOT(button_operation()));
  connect(ui->button_plus, SIGNAL(clicked()), SLOT(button_operation()));
  connect(ui->button_sin, SIGNAL(clicked()), SLOT(button_operation()));
  connect(ui->button_cos, SIGNAL(clicked()), SLOT(button_operation()));
  connect(ui->button_tan, SIGNAL(clicked()), SLOT(button_operation()));
  connect(ui->button_sqrt, SIGNAL(clicked()), SLOT(button_operation()));
  connect(ui->button_asin, SIGNAL(clicked()), SLOT(button_operation()));
  connect(ui->button_acos, SIGNAL(clicked()), SLOT(button_operation()));
  connect(ui->button_atan, SIGNAL(clicked()), SLOT(button_operation()));
  connect(ui->button_log, SIGNAL(clicked()), SLOT(button_operation()));
  connect(ui->button_ln, SIGNAL(clicked()), SLOT(button_operation()));
  connect(ui->button_mod, SIGNAL(clicked()), SLOT(button_operation()));
  connect(ui->button_open_br, SIGNAL(clicked()), SLOT(button_operation()));
  connect(ui->button_close_br, SIGNAL(clicked()), SLOT(button_operation()));
  connect(ui->button_power, SIGNAL(clicked()), SLOT(button_operation()));
  connect(ui->input_text, SIGNAL(returnPressed()),
          SLOT(on_button_equal_clicked()));

  QRegularExpression re("[+-]?([0-9]*[.])?[0-9]+");
  QValidator* validator = new QRegularExpressionValidator(re, this);
  ui->input_x->setValidator(validator);
  ui->x_min->setValidator(validator);
  ui->x_max->setValidator(validator);
  ui->y_min->setValidator(validator);
  ui->y_max->setValidator(validator);
}

s21::view::~view() { delete ui; }

void s21::view::button_operation() {
  QObject* sender = this->sender();
  QPushButton* button = qobject_cast<QPushButton*>(sender);
  QByteArray expr = ui->input_text->text().toUtf8();
  if (expr.size() == 0 || (expr.size() == 1 && expr.at(0) == '0'))
    ui->input_text->clear();
  ui->input_text->insert(button->property("operation").toString());
}

void s21::view::button_number() {
  QObject* sender = this->sender();
  QPushButton* button = qobject_cast<QPushButton*>(sender);
  QByteArray expr = ui->input_text->text().toUtf8();
  if (expr.size() == 1 && expr.at(0) == '0') ui->input_text->clear();
  ui->input_text->insert(button->text());
}

void s21::view::on_button_del_clicked() {
  QByteArray expr = ui->input_text->text().toUtf8();
  int s = expr.size();
  if (s > 0) expr.resize(s - 1);
  if (s == 1) expr.replace(0, 1, "0");
  ui->input_text->setText(expr.data());
}

//  Нажата кнопка "="
void s21::view::on_button_equal_clicked() {
  QString expr = ui->input_text->text();
  if (expr.contains("x") && ui->input_x->text().isEmpty()) {
    QMessageBox::warning(this, "Внимание!", "Введите значение x в поле ввода");
    return;
  } else {
    //  установка значения x в контроллере
    controller->setVarX(ui->input_x->text().toDouble());
  }

  //  вызов метода контроллера и получение результата
  QString res =
      QString::fromStdString(controller->processExpression(expr.toStdString()));
  //  установка результата в лейбл
  ui->result->setText(res);
}

void s21::view::on_button_ac_clicked() {
  ui->result->clear();
  ui->input_text->setText("0");
}

void s21::view::on_button_unary_clicked() {
  QString expr = ui->input_text->text().toUtf8();
  int minus = expr.lastIndexOf("-");
  if (minus == -1) {
    expr.append("-");
  } else {
    expr.replace(minus, 1, "");
  }
  ui->input_text->setText(expr);
}

void s21::view::on_button_x_clicked() {
  QString expr = ui->input_text->text().toUtf8();
  expr.append("x");
  ui->input_text->setText(expr);
}

bool s21::view::check_range() {
  if (ui->x_min->text().isEmpty() || ui->x_max->text().isEmpty() ||
      ui->y_max->text().isEmpty() || ui->y_min->text().isEmpty()) {
    qDebug() << "Error! Enter range for plotting!";
    QMessageBox::warning(this, "Внимание!",
                         "Введите значения для границ графика");
    return false;
  }

  this->x_min = ui->x_min->text().toDouble();
  this->x_max = ui->x_max->text().toDouble();
  this->y_min = ui->y_min->text().toDouble();
  this->y_max = ui->y_max->text().toDouble();

  //  проверка корректности ввода для границ графика
  if (this->x_max <= this->x_min || this->y_max <= this->y_min) {
    QMessageBox::warning(this, "Внимание!",
                         "Ошибка при вводе значений границ графика");
    return false;
  }

  //  установка границ графика
  controller->setPlotRange(this->x_min, this->x_max, this->y_min, this->y_max);

  return true;
}

void s21::view::on_button_plot_clicked() {
  //  проверка и установка границ графика
  if (!this->check_range()) return;

  //  контейнеры для построения графика
  QVector<double> x, y;
  QString expr = ui->input_text->text();
  //  вызов метода контроллера и получение результата
  auto result = controller->plot(expr.toStdString());

  x = QVector<double>(result.first.begin(), result.first.end());
  y = QVector<double>(result.second.begin(), result.second.end());

  //  очистка перед прорисовкой
  ui->result->clear();
  //  прорисовка графика
  ui->widget->addGraph();
  ui->widget->graph(0)->setData(x, y);
  ui->widget->xAxis->setLabel("x");
  ui->widget->yAxis->setLabel("y");
  ui->widget->xAxis->setRange(this->x_min, this->x_max);
  ui->widget->yAxis->setRange(this->y_min, this->y_max);
  ui->widget->replot();
  ui->widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void s21::view::on_button_clear_plot_clicked() {
  ui->widget->graph(0)->data()->clear();
  ui->widget->xAxis->setRange(-10, 10);
  ui->widget->yAxis->setRange(-10, 10);
  ui->widget->replot();
}
