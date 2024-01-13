#include "s21_model.h"

s21::model::model() {
  var_x_ = 0.0;
  x_min_ = 0.0;
  x_max_ = 0.0;
  y_min_ = 0.0;
  y_max_ = 0.0;
  step_ = 0.01;

  infix_ = "";
  postfix_ = "";
  normalized_ = "";
}

void s21::model::setExpression(std::string expression) { infix_ = expression; }

void s21::model::setVarX(double x) { var_x_ = x; }

void s21::model::setStep(double step) { step_ = step; }

void s21::model::setPlotRange(double x_min, double x_max, double y_min,
                              double y_max) {
  x_min_ = x_min;
  x_max_ = x_max;
  y_min_ = y_min;
  y_max_ = y_max;
  step_ = 0.01;
  double range = fabs(x_min_) + fabs(x_max_);
  if (range >= 100) step_ = 0.1;
  if (range >= 1000) step_ = 1;
  if (range >= 10000) step_ = 10;
  if (range >= 100000) step_ = 100;
  if (range >= 1000000) step_ = 1000;
}

void s21::model::replace_x() {
  size_t pos = infix_.find("x");
  while (pos != std::string::npos) {
    infix_.replace(pos, 1, "(" + std::to_string(var_x_) + ")");
    pos = infix_.find("x", pos + 1);
  }
}

std::pair<std::vector<double>, std::vector<double>> s21::model::plot() {
  std::vector<double> x;
  std::vector<double> y;
  std::string result = "";
  std::string expression = infix_;  //  копируем исходную строку
  double res;
  for (var_x_ = x_min_; var_x_ <= x_max_; var_x_ += step_) {
    if (fabs(var_x_) < S21_EPSILON) var_x_ = 0;
    replace_x();
    int error = normalize_infix();
    if (error) {
      break;  // ошибка в выражении
    }
    normalized_to_postfix();
    result = execute_postfix();
    res = std::stod(result);
    if ((res < 0 && res < y_min_) || (res > 0 && res > y_max_)) res = NAN;
    x.push_back(var_x_);
    y.push_back(res);
    clear_fields();
    infix_ = expression;  //  восстанавливаем исходную строку
  }
  clear_fields();
  return std::make_pair(x, y);
}

std::string s21::model::calculate() {
  std::string result = "";
  replace_x();
  int error = normalize_infix();
  if (error) {
    result = view_error(error);
  } else {
    normalized_to_postfix();
    result = execute_postfix();
  }
  clear_fields();
  return result;
}

void s21::model::clear_fields() {
  postfix_ = "";
  normalized_ = "";
}

int s21::model::parse_e(std::string::iterator* it) {
  char c = **it;
  if (c == 'e') {
    normalized_ += c;
    ++(*it);
    if (c = **it, c == '+' || c == '-') {
      normalized_ += c;
      ++(*it);
    }
    if (!isdigit(**it)) return 5;
    while (c = **it, isdigit(c)) {
      normalized_ += c;
      ++(*it);
    }
    if (**it == 'e') return 10;
  }
  return 0;
}

int s21::model::parse_dot(std::string::iterator* it) {
  char c = **it;
  if (c == '.') {
    normalized_ += c;
    ++(*it);
    if (!isdigit(**it)) return 5;
    while (c = **it, isdigit(c)) {
      normalized_ += c;
      ++(*it);
    }
    int error = parse_e(it);
    if (error) return error;
    if (**it == '.') return 2;
  }

  int error = parse_e(it);
  if (error) return error;

  return 0;
}
int s21::model::normalize_infix() {
  int error = 0;
  int unary = 0;
  std::string trimmed_infix = trim_and_lower(infix_);
  if (trimmed_infix.empty()) return 1;

  for (auto it = trimmed_infix.begin(); it != trimmed_infix.end();) {
    char c = *it;
    if (isdigit(c)) {
      while (c = *it, isdigit(c)) {
        normalized_ += c;
        ++it;
      }
      error = parse_dot(&it);
      if (error) return error;

      while (unary > 0) {
        normalized_ += ')';
        --unary;
      }
      continue;
    } else if (std::strchr("sctalm", c)) {
      int operation = parse_operation(&it);
      if (operation == 0) return 3;  // ошибка в написании оператора
      c = operation_code(operation);
    } else if ((it == trimmed_infix.begin() ||
                (it != trimmed_infix.begin() &&
                 std::strchr("+-*/^(sntlgd", *(it - 1))))) {
      if (c == '-') {
        normalized_ += '(';
        unary++;
        c = '~';
      }
      if (c == '+') {
        ++it;
        continue;
      }
    }
    normalized_ += c;
    ++it;
  }
  if (error == 0) {
    error = validate_string();
  }
  return error;
}

int s21::model::validate_string() {
  int open_bracket = 0, closed_bracket = 0;

  for (size_t i = 0; i < normalized_.length(); ++i) {
    char c = normalized_[i];
    char r = (i + 1 < normalized_.length()) ? normalized_[i + 1] : '\0';
    char l = (i > 0) ? normalized_[i - 1] : '\0';

    if (!std::strchr("0123456789~-+*/^%()aioesctlgq.", c)) return 4;

    if (c == ')') closed_bracket++;
    if (c == '(') open_bracket++;

    if (i == 0) {
      if (c == 'e') return 11;
      if (c == '.') return 5;
      if (c == ')') return 6;
      if (std::strchr("*/^%", c)) return 7;
      if (std::strchr("-+*/^%~aiosctlgq", c) && r == '\0') return 7;
    } else if (i > 0) {
      if (c == 'e' && !isdigit(l)) return 11;
      if (c == '.' && (!isdigit(l) || !isdigit(r))) return 5;
      if (c == ')' && !std::strchr("0123456789)", l)) return 8;
      if (c == ')' && r != '\0' && isdigit(r)) return 3;
      if (c == '(' && r == '\0') return 8;
      if (c == '(' && r != '\0' &&
          (!std::strchr("+-*/^%~aiosctlgq(", l) ||
           !std::strchr("+-0123456789~aiosctlgq(", r)))
        return 8;
      if (std::strchr("/^%*-+~aiosctlgq", c) && r == '\0') return 7;
      if (std::strchr("/^%*", c) && r != '\0' && l != ')' && !isdigit(l) &&
          std::strchr("0123456789~aiosctlgq(", r))
        return 9;
      if (std::strchr("aiosctlgq", c) &&
          (!std::strchr("/^%*-+~(", l) || !std::strchr("0123456789(", r)))
        return 9;
    }
  }

  if (open_bracket != closed_bracket) return 6;

  return 0;
}

std::string s21::model::execute_postfix() {
  std::string result = "";
  std::stack<double> numbers;
  std::regex reg_one("[+\\-/*^~%aiosctlgq]");
  std::regex reg_two("[~aiosctlgq]");
  std::istringstream iss(postfix_);
  std::ostringstream oss;
  std::string part;

  while (iss >> part) {
    if (std::regex_match(part, reg_one)) {
      double one = 1.0;
      double two = numbers.top();
      numbers.pop();
      if (!std::regex_match(std::string(1, part[0]), reg_two)) {
        one = numbers.top();
        numbers.pop();
      }
      numbers.push(compute(one, two, part[0]));
    } else {
      numbers.push(std::stod(part));
    }
  }

  if (!numbers.empty()) {
    oss << std::fixed << std::setprecision(9) << numbers.top();
    result = oss.str();
    result.erase(result.find_last_not_of('0') + 1, std::string::npos);
    if (result.back() == '.') {
      result.pop_back();
    }
  } else {
    result = view_error(0);
  }

  return result;
}

double s21::model::compute(double one, double two, char op) {
  double res = 0;
  switch (op) {
    case '~':
      res = -1 * two;
      break;
    case '+':
      res = one + two;
      break;
    case '-':
      res = one - two;
      break;
    case '*':
      res = one * two;
      break;
    case '/':
      res = one / two;
      break;
    case '^':
      res = pow(one, two);
      break;
    case 'c':
      res = cosl(two);
      break;
    case 's':
      res = sinl(two);
      break;
    case 't':
      res = tanl(two);
      break;
    case 'o':
      res = acosl(two);
      break;
    case 'i':
      res = asinl(two);
      break;
    case 'a':
      res = atanl(two);
      break;
    case 'l':
      res = log(two);
      break;
    case 'g':
      res = log10(two);
      break;
    case 'q':
      res = sqrt(two);
      break;
    case '%':
      res = one - (std::floor(one / two) * two);
      break;
  }
  return res;
}

int s21::model::parse_operation(std::string::iterator* it) {
  static const std::string operations[] = {
      "cos", "sin", "sqrt", "tan", "acos", "asin", "atan", "ln", "log", "mod"};
  int res = 0;

  for (int i = 0; i < 10; ++i) {
    const std::string& operation = operations[i];
    if (std::equal(*it, *it + operation.size(), operation.begin())) {
      res = i + 1;
      *it += operation.size() - 1;
      break;
    }
  }

  return res;
}

char s21::model::operation_code(int i) {
  switch (i) {
    case 1:
      return 'c';
    case 2:
      return 's';
    case 3:
      return 'q';
    case 4:
      return 't';
    case 5:
      return 'o';
    case 6:
      return 'i';
    case 7:
      return 'a';
    case 8:
      return 'l';
    case 9:
      return 'g';
    case 10:
      return '%';
    default:
      return ' ';  // Возвращаем символ по умолчанию (может потребоваться
                   // обработка ошибки)
  }
}

std::string s21::model::trim_and_lower(const std::string& str) {
  static const std::regex space_pattern("\\s");
  std::string trimmed = std::regex_replace(str, space_pattern, "");
  std::transform(trimmed.begin(), trimmed.end(), trimmed.begin(), ::tolower);
  return trimmed;
}

void s21::model::normalized_to_postfix() {
  std::stack<char> operatorStack;

  static const std::regex tokenRegex(
      R"(\d+(\.\d+)?(?:[eE][-+]?\d+)?|[+\-*/%~^()csqtoialg])");

  auto tokensBegin =
      std::sregex_iterator(normalized_.begin(), normalized_.end(), tokenRegex);
  auto tokensEnd = std::sregex_iterator();

  for (auto it = tokensBegin; it != tokensEnd; ++it) {
    std::smatch match = *it;
    std::string token = match.str();

    if (std::isdigit(token[0])) {
      postfix_ += token + " ";
    } else if (token[0] == '(') {
      operatorStack.push('(');
    } else if (token[0] == ')') {
      while (!operatorStack.empty() && operatorStack.top() != '(') {
        postfix_ += operatorStack.top();
        postfix_ += " ";
        operatorStack.pop();
      }
      operatorStack.pop();  // Pop '('
    } else {
      // Token is an operator
      while (!operatorStack.empty() &&
             check_priority(operatorStack.top()) >= check_priority(token[0]) &&
             token[0] != '^') {
        postfix_ += operatorStack.top();
        postfix_ += " ";
        operatorStack.pop();
      }
      operatorStack.push(token[0]);
    }
  }

  // Pop any remaining operators from the stack
  while (!operatorStack.empty()) {
    postfix_ += operatorStack.top();
    postfix_ += " ";
    operatorStack.pop();
  }
}

int s21::model::check_priority(char c) {
  if (c == '(') return 0;
  if (c == ')') return 1;
  if (c == '+' || c == '-') return 2;
  if (c == '*' || c == '/' || c == '%' || c == '^') return 3;
  if (c == '~') return 4;
  if (c == 's' || c == 'c' || c == 't') return 5;
  if (c == 'a' || c == 'i' || c == 'o' || c == 'q') return 6;
  if (c == 'l' || c == 'g') return 7;
  return -1;  // Обработка ошибки или невалидного символа
}

std::string s21::model::view_error(int i) {
  static const std::string SMART_CALC_ERROR[12] = {
      "unknown error",
      "empty string",
      "extra dot",
      "parse operator",
      "unknown symbol",
      "dot without int or float part",
      "missed or extra bracket",
      "unary/binary operator without operand",
      "empty brackets",
      "two or more unary/binary operators in a row",
      "extra E or e",
      "exponent without base"};

  if (i >= 0 && i < 12) {
    return SMART_CALC_ERROR[i];
  }
  return SMART_CALC_ERROR[0];  // Возвращаем "unknown error" для некорректного
                               // индекса
}
