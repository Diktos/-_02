// --- ЗАВДАННЯ 2 ---
// Мета: Операція "Додавання всередину" (Варіант 3)
#include "DoublyLinkedList.h"
class Task2List : public DoublyLinkedList {
public:
    // Використовуємо готовий метод insertAt, щоб додати рівно в центр
    void insertInMiddle(double value) {
        int midIndex = size / 2;
        insertAt(midIndex, value);
    }
};