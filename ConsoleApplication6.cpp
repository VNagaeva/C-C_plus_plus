#include <iostream>
#include <bitset>

using namespace std;

int main()
{
    short int A[6] = { 101, 200, 301, 400, 500, 600 };
    char c = 2;
    short int C[6] = { 0, 0, 0, 0, 0, 0 };

    _asm {
        lea esi, A; // Создаем указатель на первый элемент массива А в esi
        lea edi, C; // Создаем указатель на первый элемент массива C в edi
        mov ecx, 6; // Создаем счетчик в еcx

    START:
        mov ax, [esi];
        mov bx, [esi];

        mov dx, 1100000000000000b; // Маска для отбора максимального

        and ax, dx;
        shr ax, 14;

        mov dx, 11b; // Маска для отбора минимального
        and bx, dx;

        mov dx, [esi];

        cmp ax, c;
        jne NEXT;

        cmp bx, c;
        jne NEXT;

        mov bx, 11000b;
        xor dx, bx;

        jmp NEXT;

    NEXT:
        mov[edi], dx;
        dec ecx;
        add esi, 2;
        add edi, 2;
        cmp ecx, 0;
        jne START;
    }

    for (int i = 0; i < 6; ++i) {
        cout << "A[i]: " << bitset<16>(A[i]) << ' ';
        printf("%x", A[i]);
        cout << "C[i]: " << bitset<16>(C[i]) << endl;
        printf("%x", C[i]);
        cout << endl;
    }
    return 0;
}
