
#include <iostream>

using namespace std;

short int Assembler(char y, char z, short int x) {
	short int v = 0;
	__asm {
		mov al, y;
		cbw;
		add ax, 5;
		imul x; //dx:ax

		mov bx, ax;
		mov cx, dx;

		mov al, 3;
		imul z;
		cwd;

		add bx, ax;
		adc dx, cx;

		mov al, 3;
		imul z;
		sub ax, 4;

		xchg ax, bx;

		idiv bx;

		inc ax;

		mov v, ax;
	}
	return v;
}

int main()
{
	char y, z;
	short int v_c1, v_c2;
	short int x, v_asm1, v_asm2;
	short int v1 = 0x2, v2 = -0x15AC;
	int i;
	printf("Select test 1 or 2:\n");
	scanf_s("%d", &i);
	switch (i)
	{
	case 1:
		x = 0x2;
		y = -0x7;
		z = 0x2;
		v_asm1 = Assembler(y, z, x);
		v_c1 = (0x2 * (-0x7 + 5) + 3 * 0x2) / (3 * 0x2 - 4) + 1;
		printf("asm:\n base10: %d, base16: %x\n", v_asm1, v_asm1);
		printf("c:\n base10: %d, base16: %x\n", v_c1, v_c1);
		printf("From source task:\n base10: %d, base16: %x\n", v1, v1);
		break;
	case 2:
		x = 0x3FB5;
		y = 0x7D;
		z = -0x7E;
		v_c2 = (0x3FB5 * (0x7D + 5) + 3 * (-0x7E)) / (3 * (-0x7E) - 4) + 1;

		v_asm2 = Assembler(y, z, x);
		printf("asm:\n base10: %d, base16: %x\n", v_asm2, v_asm2);
		printf("c:\n base10: %d, base16: %x\n", v_c2, v_c2);
		printf("from source task:\n base10: %d, base16: %x\n", v2, v2);
		break;
	}
}

mov al, y
cbw
sub x, 4
// mov x, bx
// sub bx, 4
imul bx // imul x
// double word dx:ax
mov bx, ax
mov cx, dx
mov al, 3
imul z // v ax
cwd// dx:ax double word

sub ax, bx
sub dx, cx // dx:ax

mov bx, ax // dx:bx

mov al, 2
imul y // ax word
add ax, 3

// dx:bx / ax

xchg ax, bx // dx:ax / bx ax - chas
sub ax, 2
mov v, ax