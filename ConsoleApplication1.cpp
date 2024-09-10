#include<iostream>
#include<stdio.h>
#include<locale.h>

// v = (5y + 2xz - 3) / (z-2) + 1
// x, z - byte, y, v - words
// x = 1h, y = 2h, z = 3h, v = 2h
//x = 5Ah, y = 2ABCh, z = 5Bh, v = 31Fh

short int assembler(char x, short int y, char z) {
    short int v = 0;

    __asm {
        mov al, x;
        mov bl, z;
        imul bl; // ax = xz
        mov bx, 2;
        imul bx; // dx:ax = 2xz

        mov bx, dx;
        mov cx, ax; // bx:cx = 2xz

        mov ax, y;
        mov dx, 5;
        imul dx; // dx:ax = 5y

        add ax, cx;
        adc dx, bx; // dx:ax = 5y + 2xz

        mov bx, dx;
        mov cx, ax; // bx:cx = 5y + 2xz

        mov ax, 3;
        cwd; // dx:ax = 3

        sub cx, ax;
        sbb bx, dx; // bx:cx = 5y + 2xz - 3

        mov al, z;
        cbw;
        sub ax, 2; // ax = z-2

        xchg ax, cx;
        mov dx, bx; // dx:ax = 5y + 2xz - 3; // cx = z-2

        idiv cx; // ax = (5y + 2xz - 3) / (z-2);

        add ax, 1;

        mov v, ax;
    }

    return v;
}

int main() {
    setlocale(LC_ALL, "rus");

    char x = 0x1; // Тестовое значение из изображения (слово)
    short int y = 0x2;
    char z = 0x3;
    // v = 2h
    short int v;         // Результат

    v = assembler(x, y, z);
    std::cout << "Результат асма v: " << v << std::endl;

    v = (5*y + 2*x*z - 3) / (z - 2) + 1;
    std::cout << "Результат плюсов v: " << v << std::endl;

    //x = 5Ah, y = 2ABCh, z = 5Bh, v = 31Fh
    x = 0x5a; // Тестовое значение из изображения (слово)
    y = 0x2abc;
    z = 0x5b;
    // v = 0x31f;
    v = assembler(x, y, z);
    std::cout << "Результат асма v: " << v << std::endl;

    v = (5 * y + 2 * x * z - 3) / (z - 2) + 1;
    std::cout << "Результат плюсов v: " << v << std::endl;

    int tmp;
    while (true) {
        // Блок ввода значений
        printf("Введите x: ");
        std::cin >> tmp;
        if (-128 <= tmp && tmp <= 127) x = (char)tmp;
        else
        {
            printf("Введенное значение не соответствует допустимому диапазону\n\n");
            break;
        }

        printf("Введите y: ");
        std::cin >> tmp;
        if (-32768 <= tmp && tmp <= 32767) y = (short int)tmp;
        else
        {
            printf("Введенное значение не соответствует допустимому диапазону\n\n");
            break;
        }

        printf("Введите z: ");
        std::cin >> tmp;
        if (-128 <= tmp && tmp <= 127 && tmp != 2) z = (char)tmp;
        else
        {
            printf("Введенное значение не соответствует допустимому диапазону\n\n");
            break;
        }

        v = assembler(x, y, z);
        std::cout << "Результат асма v: " << v << std::endl;

        v = (5 * y + 2 * x * z - 3) / (z - 2) + 1;
        std::cout << "Результат плюсов v: " << v << std::endl;
    }

    return 0;
}
// g++ -masm=intel tmp.cpp
// arch -x86_64 g++ -masm=intel something.cpp
