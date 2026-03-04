#include <iostream>
#include <chrono>
#include <iomanip>
#include <stack>
#include <string>
#include <sstream>
#include <cmath>

using namespace std; // Підключаємо стандартний простір імен

// ЗАВДАННЯ 1 та 2: Двозв'язний список (тип double) та додавання в середину
class DoublyLinkedList {
protected:
    struct Node { // Структура вузла
        double data; // Дані вузла (Варіант 5)
        Node* next, * prev; // Вказівники на наступний та попередній вузли
        Node(double val) : data(val), next(nullptr), prev(nullptr) {} // Конструктор вузла
    };
    Node* head = nullptr; // Вказівник на початок списку
    Node* tail = nullptr; // Вказівник на кінець списку
    int size = 0; // Лічильник кількості елементів

public:
    // ОПЕРАЦІЯ: Взяття за індексом
    double get(int index) {
        if (index < 0 || index >= size) return -1.0;

        Node* curr;
        // Якщо індекс у першій половині — йдемо від голови вперед
        if (index < size / 2) {
            curr = head;
            for (int i = 0; i < index; i++) curr = curr->next;
        }
        // Якщо індекс у другій половині — йдемо від хвоста назад
        else {
            curr = tail;
            for (int i = size - 1; i > index; i--) curr = curr->prev;
        }
        return curr->data;
    }

    // ОПЕРАЦІЯ: Додавання за індексом
    void insertAt(int index, double value) {
        if (index < 0 || index > size) return; // Перевірка коректності індексу
        Node* newNode = new Node(value); // Створюємо новий вузол

        if (size == 0) { // Якщо список порожній
            head = tail = newNode; // Новий вузол стає і початком, і кінцем
        }
        else if (index == 0) { // Якщо додаємо на самий початок
            newNode->next = head; // Наступним для нового стає колишня голова
            head->prev = newNode; // Попереднім для колишньої голови стає новий вузол
            head = newNode; // Оновлюємо вказівник голови
        }
        else if (index == size) { // Якщо додаємо в самий кінець
            newNode->prev = tail; // Попереднім для нового стає колишній хвіст
            tail->next = newNode; // Наступним для колишнього хвоста стає новий вузол
            tail = newNode; // Оновлюємо вказівник хвоста
        }
        else { // Якщо додаємо десь всередину
            Node* curr; // Створюємо робочий вказівник для пошуку вузла

            // Перевіряємо, де знаходиться потрібний індекс: у першій чи другій половині списку
            if (index < size / 2) {
                // Індекс у першій половині — вигідніше йти від початку
                curr = head; // Ставимо вказівник на стартовий елемент (голову)

                // Крутимо цикл вперед від 0 до потрібного індексу
                for (int i = 0; i < index; i++)
                    curr = curr->next; // Крокуємо вперед по вказівниках next

            }
            else {
                // Індекс у другій половині — вигідніше йти від кінця назустріч
                curr = tail; // Ставимо вказівник на останній елемент (хвіст)

                // Крутимо цикл у зворотньому напрямку від кінця до потрібного індексу
                for (int i = size - 1; i > index; i--)
                    curr = curr->prev; // Крокуємо назад по вказівниках prev (головна фішка двозв'язного списку)
            }
            size++; // Збільшуємо загальну кількість елементів
        }
    }
    // ОПЕРАЦІЯ: Видалення за індексом (ОПТИМІЗОВАНА)
    void removeAt(int index) {
        if (index < 0 || index >= size) return; // Відсікаємо криві індекси (менше 0 або більше поточного розміру)

        Node* curr; // Створюємо робочий вказівник

        // ОПТИМІЗАЦІЯ ПОШУКУ: Йдемо з того боку, куди ближче
        if (index < size / 2) {
            curr = head; // Індекс у першій половині — стартуємо з голови
            for (int i = 0; i < index; i++) curr = curr->next; // Крокуємо вперед
        }
        else {
            curr = tail; // Індекс у другій половині — стартуємо з хвоста
            for (int i = size - 1; i > index; i--) curr = curr->prev; // Крокуємо назад
        }

        // ПЕРЕПРИВ'ЯЗКА ВКАЗІВНИКІВ (Вирізаємо знайдений вузол зі списку)
        if (curr->prev) {
            curr->prev->next = curr->next; // Лівому сусіду кажемо, що його наступний тепер той, що йде ПІСЛЯ curr
        }
        else {
            head = curr->next; // Якщо ми видаляємо голову, то новою головою стає наступний елемент
        }

        if (curr->next) {
            curr->next->prev = curr->prev; // Правому сусіду кажемо, що його попередній тепер той, що йде ПЕРЕД curr
        }
        else {
            tail = curr->prev; // Якщо ми видаляємо хвіст, то новим хвостом стає попередній елемент
        }

        delete curr; // Фізично знищуємо вузол, звільняємо пам'ять
        size--; // Зменшуємо лічильник кількості елементів
    }

    // ЗАВДАННЯ 2: Операція "Додавання всередину" (Варіант 3)
    void insertInMiddle(double value) {
        int midIndex = size / 2; // Знаходимо індекс рівно по центру
        insertAt(midIndex, value); // Використовуємо існуючий метод для вставки
    }

    // Деструктор для очищення пам'яті
    ~DoublyLinkedList() {
        Node* curr = head; // Ставимо робочий вказівник на початок
        while (curr) { // Крутимо цикл, поки є вузли
            Node* next = curr->next; // ЗАПАМ'ЯТОВУЄМО адресу наступного вузла
            delete curr; // ЗНИЩУЄМО поточний вузол
            curr = next; // ПЕРЕХОДИМО до збереженого наступного вузла
        }
    }
};

// ЗАВДАННЯ 3: Масив з амортизацією
class AmortizedArray {
private:
    double* data; // Динамічний масив
    int capacity; // Фізична місткість масиву
    int size; // Кількість реально зайнятих елементів

public:
    AmortizedArray() : capacity(2), size(0) { // Конструктор
        data = new double[capacity]; // Виділяємо пам'ять під 2 елементи
    }

    void insertAt(int index, double value) {
        if (index < 0 || index > size) return; // Перевірка меж
        if (size == capacity) { // Якщо місце закінчилося
            capacity *= 2; // Амортизація: збільшуємо місткість удвічі
            double* newData = new double[capacity]; // Створюємо новий більший масив
            for (int i = 0; i < size; i++) newData[i] = data[i]; // Копіюємо старі дані
            delete[] data; // Видаляємо старий масив
            data = newData; // Переключаємо вказівник на новий
        }
        for (int i = size; i > index; i--) data[i] = data[i - 1]; // Зсуваємо всі елементи вправо
        data[index] = value; // Вставляємо нове значення у звільнену комірку
        size++; // Збільшуємо розмір
    }

    ~AmortizedArray() { // Деструктор
        delete[] data; // Очищаємо пам'ять масиву
    }
};

// ЗАВДАННЯ 5: Зворотній польський запис (ЗПЗ)
class RPNCalculator {
private:
    int priority(string op) { // Визначення пріоритету операторів
        if (op == "+" || op == "-") return 1; // Найнижчий пріоритет
        if (op == "*" || op == "/") return 2; // Середній пріоритет
        if (op == "^" || op == "sqrt") return 3; // Найвищий пріоритет
        return 0; // Якщо не оператор
    }

public:
    string convert(string expr) { // Перетворення рядка в ЗПЗ
        stringstream ss(expr); // Розбиваємо рядок на слова
        string token, result = ""; // Токен і результуючий рядок
        stack<string> s; // Стек для операторів
        while (ss >> token) { // Читаємо по одному токену
            if (isdigit(token[0])) { // Якщо це число
                result += token + " "; // Додаємо одразу у вивід
            }
            else if (token == "(") { // Якщо відкриваюча дужка
                s.push(token); // Закидаємо в стек
            }
            else if (token == ")") { // Якщо закриваюча дужка
                while (s.top() != "(") { // Виштовхуємо все до відкриваючої дужки
                    result += s.top() + " ";
                    s.pop();
                }
                s.pop(); // Видаляємо саму "(" зі стеку
            }
            else { // Якщо це оператор
                while (!s.empty() && priority(s.top()) >= priority(token)) { // Перевіряємо пріоритети
                    result += s.top() + " "; // Виштовхуємо старші оператори
                    s.pop();
                }
                s.push(token); // Кладемо поточний оператор у стек
            }
        }
        while (!s.empty()) { // Якщо вираз закінчився
            result += s.top() + " "; // Виштовхуємо залишки зі стеку
            s.pop();
        }
        return result; // Повертаємо готовий рядок ЗПЗ
    }

    double calculate(string rpn) { // Обчислення результату
        stringstream ss(rpn); // Розбиваємо ЗПЗ на токени
        string token;
        stack<double> s; // Стек для чисел
        while (ss >> token) { // Читаємо кожен токен
            if (isdigit(token[0])) { // Якщо число
                s.push(stod(token)); // Кладемо в стек
            }
            else { // Якщо оператор
                double b = s.top(); s.pop(); // Дістаємо друге число
                if (token == "sqrt") { // Якщо корінь (унарна операція)
                    s.push(sqrt(b));
                    continue;
                }
                double a = s.top(); s.pop(); // Дістаємо перше число
                if (token == "+") s.push(a + b); // Додаємо і кладемо результат у стек
                else if (token == "-") s.push(a - b); // Віднімаємо
                else if (token == "*") s.push(a * b); // Множимо
                else if (token == "/") s.push(a / b); // Ділимо
                else if (token == "^") s.push(pow(a, b)); // Підносимо до степеня
            }
        }
        return s.top(); // Фінальний результат лежить на вершині стеку
    }
};

int main() {
    cout << fixed << setprecision(9); // Налаштовуємо точність виводу для часу
    int testSizes[] = { 100, 1000, 10000 }; // Масив з кількостями елементів для тесту

    // ЗАВДАННЯ 4: ВИМІРЮВАННЯ ЧАСУ
    for (int n : testSizes) { // Проходимося по кожній кількості
        DoublyLinkedList dll; // Створюємо список
        AmortizedArray arr; // Створюємо масив
        for (int i = 0; i < n; i++) { // Заповнюємо їх початковими даними
            dll.insertAt(0, 2.28);
            arr.insertAt(0, 1.18);
        }

        auto start = chrono::high_resolution_clock::now(); // Засікаємо старт для списку
        dll.insertInMiddle(9.2); // Виконуємо операцію додавання в середину
        auto end = chrono::high_resolution_clock::now(); // Засікаємо фініш
        double timeDLL = chrono::duration<double>(end - start).count(); // Рахуємо різницю

        start = chrono::high_resolution_clock::now(); // Засікаємо старт для масиву
        arr.insertAt(n / 2, 9.3); // Вставляємо в середину масиву
        end = chrono::high_resolution_clock::now(); // Засікаємо фініш
        double timeArr = chrono::duration<double>(end - start).count(); // Рахуємо різницю

        // Виводимо результати порівняння
        cout << "N=" << n << " | List: " << timeDLL << "s | Array: " << timeArr << "s\n";
    }

    // ТЕСТУВАННЯ КАЛЬКУЛЯТОРА
    RPNCalculator calc; // Створюємо об'єкт калькулятора
    string expr = "( 5 + 3 ) * 2 ^ 2"; // Задаємо  вираз
    string rpn = calc.convert(expr); // Конвертуємо у ЗПЗ

    // Виводимо вихідні та кінцеві дані
    cout << expr << "\nRPN: " << rpn << "\nResult: " << calc.calculate(rpn) << "\n";

    return 0; 
}