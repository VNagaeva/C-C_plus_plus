#include <iostream>

int main() {
	char x1, x2, x3;
	while (true) {
		printf("\nEnter 3 digits: ");
		std::cin >> x1 >> x2 >> x3;
		if (!isdigit(x1) || !isdigit(x2) || !isdigit(x3)) {
			printf("Error in input\n");
			return 1;
		}
		
		x1 = x1 - '0';
		x2 = x2 - '0';
		x3 = x3 - '0';
		char m;
		__asm {
			mov al, x1;
			mov bl, x3;
			cmp bl, al;
			ja DIVI;
			cmp bl, 9;
			je EXIT;
			inc bl;
			jmp EXIT;

		DIVI:
			cbw;
			mov cl, 2;
			div cl;
			jmp EXIT;

		EXIT:
			mov x1, al;
			mov x3, bl;
		}
		x1 = x1 + '0';
		x2 = x2 + '0';
		x3 = x3 + '0';
		printf("Result: %c%c%c\n", x1, x2, x3);
	}
	return 0;
}