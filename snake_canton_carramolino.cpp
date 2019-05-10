#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>

#define L 20
#define A 50

typedef struct {
	int x, y, movx, movy;
	char imagen;

}SNAKE;

typedef struct {
	int x, y;
	char imagen = '@';
}APPLE;

void setup(char espacio[L][A], SNAKE*, int *, APPLE *manz, char *tecla, int *dif);//en esta funcion se va a crear todo lo necesario para justo antes de que la serpiente comience el movimiento
void pantalla(char espacio[L][A]);//en esta funcion se va a dibujar la pantalla(matriz) sobre la que snake se va a mover
void fruta(APPLE *manz, SNAKE*);//en esta funcion se genera la posicion de la fruta de manera aleatoria con <stdlib.h> y <time.h>
void loop(char espacio[L][A], SNAKE*, int *, APPLE *manz, char *tecla, int *dif, FILE *, int *);//aqui es donde se desarrollará tanto el movimiento de la serpiente como las condiciones del juego
void movimiento(SNAKE*, int *, char *tecla, int *dif);//esta funcion se usara para generar el movimiento de snake en funcion de las teclas del teclado (W, A, S, D)
void puntuacion(FILE *ppunt, int *, int*);


int main() {
	int i, j, tam = 4, dif, puntua = 0;
	SNAKE *snake;
	APPLE manz;
	char tecla;
	char espacio[L][A];
	FILE *ppunt;
	errno_t err = fopen_s(&ppunt, "puntuacion_snake.txt", "a+");
	if (err == 0)
		printf("Se generó un archivo con las puntuaciones\n");
	else {
		printf("el archivo no pudo abrirse");
		exit(-1);
	}
	snake = (SNAKE *)malloc(tam * sizeof(SNAKE));
	setup(espacio, snake, &tam, &manz, &tecla, &dif);
	loop(espacio, snake, &tam, &manz, &tecla, &dif, ppunt, &puntua);

	system("pause");
}

void setup(char espacio[L][A], SNAKE *snake, int *tam, APPLE *manz, char *tecla, int *dif) {
	int i, j, k, flag = 0, opc = 0;
	printf("\n\n\t\tIntroduce la dificultad que quiere\nFacil:1\nMedio:2\nDificil:3\nElige 1, 2 o 3:");
	scanf_s("%d", &opc);
	switch (opc) {
	case 1:
		*dif = 45;
		break;
	case 2:
		*dif = 30;
		break;
	case 3:
		*dif = 15;
		break;
	}
	system("cls");


	pantalla(espacio);
	(snake->x) = 4;
	(snake->y) = 4;
	(snake->imagen) = 241;
	for (i = 1; i < *tam; i++) {
		((snake + i)->x) = 4 - i;
		((snake + i)->y) = 4;
		((snake + i)->imagen) = '=';
	}

	fruta(manz, snake);
	//imprimir espacio, serpiente y fruta sobreescribiendose
	for (i = 0; i < L; i++) {
		for (j = 0; j < A; j++) {
			flag = 0;
			for (k = 0; k < *tam; k++) {
				if (j == ((snake + k)->x) && i == ((snake + k)->y)) {
					printf("%c", ((snake + k)->imagen));
					flag = 0;
					k = 100;//damos a k el valor de 100 para que en la condicion del for salte
							//y asi el problema de que el else se cumple en el siguiente valor de la k 
							//y nos imprima el campo lo solventamos
				}
				else {
					flag = 1;
				}


			}
			if ((manz->x) == j && ((manz->y) == i)) {
				printf("%c", manz->imagen);
				flag = 0;
			}
			if (flag == 1) {
				printf("%c", espacio[i][j]);
			}

		}
		printf("\n");
	}


	while (_kbhit() == 0) {
		if (_kbhit() == 1 && (_kbhit() == 'w' || _kbhit() == 'W' || _kbhit() == 'd' || _kbhit() == 'D' || _kbhit() == 's' || _kbhit() == 'S'))
			*tecla = _getch();
	}


}
void pantalla(char espacio[L][A]) {
	int i, j;
	for (i = 0; i < L; i++) {
		for (j = 0; j < A; j++) {
			if ((i == 0 || i == L - 1) || (j == 0 || j == A - 1)) {
				espacio[i][j] = '*';
			}
			else {
				espacio[i][j] = ' ';
			}
		}

	}

}
void fruta(APPLE *manz, SNAKE *snake) {
	//https://blog.martincruz.me/2012/09/obtener-numeros-aleatorios-en-c-rand.html
	srand(time(NULL));
	do {
		(manz->x) = 1 + rand() % ((A - 1) - 1);
		(manz->y) = 1 + rand() % ((L - 1) - 1);
	} while (((manz->x) == (snake->x)) && ((manz->y) == (snake->y)));
}
void loop(char espacio[L][A], SNAKE *snake, int *tam, APPLE *manz, char *tecla, int *dif, FILE *ppunt, int*puntua) {
	int vida = 1, i, j, k, d, flag = 0;
	do {
		movimiento(snake, tam, tecla, dif);
		for (i = 0; i < L; i++) {
			for (j = 0; j < A; j++) {
				flag = 0;
				for (k = 0; k < *tam; k++) {
					if (j == ((snake + k)->x) && i == ((snake + k)->y)) {
						printf("%c", ((snake + k)->imagen));
						flag = 0;
						k = 100;
					}
					else {
						flag = 1;
					}


				}
				if ((manz->x) == j && ((manz->y) == i)) {
					printf("%c", manz->imagen);
					flag = 0;
				}
				if (flag == 1) {
					printf("%c", espacio[i][j]);
				}
				if ((manz->x == snake->x) && (manz->y == snake->y)) {
					*tam += 1;
					snake = (SNAKE *)realloc(snake, (*tam) * sizeof(SNAKE));
					fruta(manz, snake);
					(snake + (*tam - 1))->imagen = '=';
					*puntua += 45 / (*dif);

				}
				if (snake->x == 0 || snake->x == (A - 1) || snake->y == 0 || snake->y == L - 1) {
					system("cls");
					printf("\n\n\n\n\t\t\t\tHAS PERDIDO!!");
					vida = 0;
					break;
				}
				//hay que conseguir una condicion para perder en que if(snake->x==snake+a->x && ..) siendo a todos los numeros reales entre 1 y *tam;
				for (d = 1; d < *tam; d++) {
					if (snake->x == (snake + d)->x && snake->y == (snake + d)->y) {
						system("cls");
						printf("\n\n\n\n\t\t\t\tHAS PERDIDO!!");
						vida = 0;
						break;
					}
				}
				if (vida == 0)
					break;
			}
			printf("\n");
			if (vida == 0)
				break;

		}
		system("cls");


	} while (vida == 1);
	puntuacion(ppunt, puntua, dif);
}

void movimiento(SNAKE *snake, int *tam, char *tecla, int *dif) {
	int i = 0;

	if (_kbhit() == 1) {
		*tecla = _getch();

		if ((*tecla == 'w' || *tecla == 'W')) {

			snake->movx = 0;
			snake->movy = -1;
		}
		//cuando el ultimo valor de movimiento 
		if ((*tecla == 's' || *tecla == 'S')) {

			snake->movx = 0;
			snake->movy = 1;
		}
		if ((*tecla == 'a' || *tecla == 'A')) {

			snake->movx = -1;
			snake->movy = 0;
		}
		if ((*tecla == 'd' || *tecla == 'D')) {

			snake->movx = 1;
			snake->movy = 0;
		}

		for (i = (*tam - 1); i > 0; i--) {
			((snake + i)->x) = ((snake + (i - 1))->x);
			((snake + i)->y) = ((snake + (i - 1))->y);
		}
		//estabamos cometiendo el error en cambiar la posicion de la cabeza antes que hacer las posiciones anteriores
		(snake->x) += (snake->movx);
		(snake->y) += (snake->movy);
	}
	else {
		if (*tecla == 'w' || *tecla == 'W') {
			snake->movx = 0;
			snake->movy = -1;
		}
		if (*tecla == 's' || *tecla == 'S') {
			snake->movx = 0;
			snake->movy = 1;
		}
		if (*tecla == 'a' || *tecla == 'A') {
			snake->movx = -1;
			snake->movy = 0;
		}
		if (*tecla == 'd' || *tecla == 'D') {

			snake->movx = 1;
			snake->movy = 0;
		}

		for (i = (*tam - 1); i > 0; i--) {
			((snake + i)->x) = ((snake + (i - 1))->x);
			((snake + i)->y) = ((snake + (i - 1))->y);
		}
		//estabamos cometiendo el error en cambiar la posicion de la cabeza antes que hacer las posiciones anteriores
		(snake->x) += (snake->movx);
		(snake->y) += (snake->movy);
		Sleep(*dif);
	}

}
void puntuacion(FILE *ppunt, int *puntua, int *dif) {
	char cad[10];
	system("cls");
	printf("introduzca el nombre:");
	getchar();
	gets_s(cad);

	fprintf(ppunt, "%s %d\n", cad, *puntua);

}
/*BLOC DE NOTAS
-Para imprimir la serpiente en el loop se me ocurre un if(i=y && j=x) para que la posicion coincida en la matriz
-Para aumentar el tamaño de la serpiente hay que aumentar la dimension con realloc()
FALLOS:
-PARA COMENZAR TIENE QUE SER CON 'W' 'D' O 'S'
-SI VAS HACIA EL CUERPO DE LA SERPIENTE MUERES;
*/