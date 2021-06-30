#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Для работы числового калькулятора в файл вводятся значения
//1.Операция(+,-,*,/,!,^)
//2.Символ s
//3. Два числа через пробел (+ s 4 5)
//Для работы векторного калькулятора:
//1. Операция
//2. Символ v
//3. Кол-во координат вектора(2, 3)
//4. Значения первого и второго вектора через пробел

struct TaskQueue {
// Число элементов в очереди
	int n;
	struct Task *first;
	struct Task *last;
};

// Результат вычислительного задания (узел очереди результатов)
struct Result {
	int isScalar;
	double res;
	int n;
	double *vres;
	struct Result *next;
};

// Очередь результатов
struct ResultQueue {
	int n;
	struct Result *first;
	struct Result *last;
};

// Типы операций
enum operation {
	add, sub, mult, divide, power, factorial
};
// Вычислительное задание (узел очереди заданий)
struct Task {
	int isScalar;
	enum operation op;
	double x;
	double y;
	int n;

	double *v1;
	double *v2;

	struct Task *next;
};

double calcFactorial(int n);
void freeTask(struct Task *t);
struct Task* readTask(FILE *in);
void freeTaskQueue(struct TaskQueue *tq);
struct Task* popTaskQueue(struct TaskQueue *tq);
void pushTaskQueue(struct TaskQueue *tq, struct Task *t);
void readTaskkQueue(struct TaskQueue *tq, FILE *in);
void freeResult(struct Result *r);
void printResult(struct Result *r, FILE *out);
void freeResultQueue(struct ResultQueue *rq);
void pushResultQueue(struct ResultQueue *rq, struct Result *r);
struct Result* popResultQueue(struct ResultQueue *rq);
struct Result* processTask(struct Task *t);
void processTaskQueue(struct TaskQueue *tq, struct ResultQueue *rq);
void printResultQueue(struct ResultQueue *rq, FILE *out);

int main() {
	char input[20], output[20];
	FILE *in, *out;
	printf("\nInput file name: ");
	scanf("%s", input);
	printf("Output file name: ");
	scanf("%s", output);
	in = fopen(input, "r");
	out = fopen(output, "w");
// Очередь заданий
	struct TaskQueue tq;
// Очередь результатов
	struct ResultQueue rq;
// Заполнение очереди заданий из входного файла
	readTaskkQueue(&tq, in);

// Обработка очереди заданий с образованием очереди результатов
	processTaskQueue(&tq, &rq);
// Вывод очереди результатов в файл с удалением результатов
	printResultQueue(&rq, out);
// Закрытие файлов
	fclose(in);
	fclose(out);
	return 0;
}

// Вычисление факториала
double calcFactorial(int n) {
	double res = 1.0;
	while (n > 0)
		res *= n--;
	return res;
}

void freeTask(struct Task *t) {
	if (!t->isScalar) {
		free(t->v1);
		free(t->v2);
	}
	free(t);
}

// Прочитать из файла очередное задание.
struct Task* readTask(FILE *in) {
	struct Task *t;
	int ch;
	do {
		ch = fgetc(in);
	} while ((ch == '\r') || (ch == '\n'));
// Если файл кончился
	if (ch == EOF)
		return NULL;
// создаем новую структуру задания
	t = (struct Task*) malloc(sizeof(struct Task));
	switch (ch) {
	case '+':
		t->op = add;
		break;
	case '-':
		t->op = sub;
		break;
	case '*':
		t->op = mult;
		break;
	case '/':
		t->op = divide;
		break;
	case '^':
		t->op = power;
		break;
	case '!':
		t->op = factorial;
		break;
	}
// Чтение из файла пробела
	fgetc(in);
// Чтение символа 's' или 'v'
	ch = fgetc(in);
	if (ch == 'c') {
		t->isScalar = 1;
		if (t->op != factorial) {
			fscanf(in, "%lf", &t->x);
			fscanf(in, "%lf", &t->y);
		} else {
			fscanf(in, "%lf", &t->x);
		}
	} else {
		t->isScalar = 0; // Читаем размерности векторов
		fscanf(in, "%d", &t->n);
		t->v1 = (double*) malloc(sizeof(double) * t->n);
		t->v2 = (double*) malloc(sizeof(double) * t->n);
		for (int i = 0; i < t->n; i++)
			fscanf(in, "%lf", &(t->v1[i]));
		for (int i = 0; i < t->n; i++)
			fscanf(in, "%lf", &(t->v2[i]));
	}
	return t;
}

// Освобождение памяти, выделенной под очередь заданий
void freeTaskQueue(struct TaskQueue *tq) {
	struct Task *cur = tq->first;
	struct Task *next;
	while (tq->n > 0) {
		next = cur->next;
		freeTask(cur);
		cur = next;
		tq->n--;
	}
}

// Берём задания из очереди
struct Task* popTaskQueue(struct TaskQueue *tq) {
	if (tq->n > 0) {
		tq->n--;
		struct Task *t = tq->first;
		tq->first = tq->first->next;
		if (tq->n == 0)
			tq->last = NULL;
		return t;
	} else
		return NULL;
}

// Добавление задания к очереди
void pushTaskQueue(struct TaskQueue *tq, struct Task *t) {
	if (tq->n == 0) {
		tq->n = 1;
		tq->first = t;
		tq->last = t;
	} else {
		tq->n++;
		tq->last->next = t;
		t->next = NULL;
		tq->last = t;
	}
}

// Заполнение очереди заданий из входного файла
void readTaskkQueue(struct TaskQueue *tq, FILE *in) {
	tq->n = 0;
// Цикл чтения заданий из входного файла
	while (1) {
// Чтение очередного задания из файла
		struct Task *t = readTask(in);
		if (t != NULL) {
			pushTaskQueue(tq, t);
		} else
			break;
	}
}

// Освобождение памяти, выделенной под результат
void freeResult(struct Result *r) {
	if (!r->isScalar)
		free(r->vres);
	free(r);
}

// Печать результата
void printResult(struct Result *r, FILE *out) {
	if (r->isScalar) {
		fprintf(out, "%.2lf\n", r->res);
	} else {
		for (int i = 0; i < r->n; i++) {
			if (i > 0)
				fprintf(out, " ");
			fprintf(out, "%.2lf", r->vres[i]);
		}
		fprintf(out, "\n");
	}
}

void freeResultQueue(struct ResultQueue *rq) {
	struct Result *cur = rq->first;
	struct Result *next;
	while (rq->n > 0) {
		next = cur->next;
		freeResult(cur);
		cur = next;
		rq->n--;
	}
}

// Добавление результата к очереди результатов
void pushResultQueue(struct ResultQueue *rq, struct Result *r) {
	if (rq->n == 0) {
		rq->n = 1;
		rq->first = r;
		rq->last = r;
	} else {
		rq->n++;
		rq->last->next = r;
		r->next = NULL;
		rq->last = r;
	}
}

// Извлечение результата из очереди результатов
struct Result* popResultQueue(struct ResultQueue *rq) {

	if (rq->n > 0) {
		rq->n--;
		struct Result *r = rq->first;
		rq->first = rq->first->next;
		if (rq->n == 0)
			rq->last = NULL;

		return r;
	} else
		return NULL;
}

// Выполнение вычисления задания. Процедура возвращает
// заполненную структуру-результат
struct Result* processTask(struct Task *t) {
	struct Result *r = (struct Result*) malloc(sizeof(struct Result));
// Операция скалярная
	if (t->isScalar) {
		r->isScalar = 1;
		switch (t->op) {
		case add:
			r->res = t->x + t->y;
			break;
		case sub:
			r->res = t->x - t->y;
			break;
		case mult:
			r->res = t->x * t->y;
			break;
		case divide:
			r->res = t->x / t->y;
			break;
		case power:
			r->res = pow(t->x, t->y);
			break;
		case factorial:
			r->res = calcFactorial(t->x);
			break;

		}
// Операция векторная
	} else {

		if (t->op == mult) {
			r->isScalar = 1;

			r->res = 0.0;
			for (int i = 0; i < t->n; i++) {
				r->res += t->v1[i] * t->v2[i];
			}

		} else {
			r->isScalar = 0;

			r->n = t->n;

			r->vres = (double*) malloc(sizeof(double) * t->n);

			if (t->op == add) {

				for (int i = 0; i < t->n; i++) {
					r->vres[i] = t->v1[i] + t->v2[i];
				}

			} else {

				for (int i = 0; i < t->n; i++) {
					r->vres[i] = t->v1[i] - t->v2[i];
				}
			}
		}
	}
	return r;
}

// Выполнение очереди заданий. Задания изымаются из очереди заданий,
// результаты добавляются к очереди результатов
void processTaskQueue(struct TaskQueue *tq, struct ResultQueue *rq) {
	rq->n = 0;
	while (tq->n > 0) {
// Извлечение задания из очереди
		struct Task *t = popTaskQueue(tq);
// Получение результа задания
		struct Result *r = processTask(t);
		freeTask(t);
// Добавление результата к очереди результатов
		pushResultQueue(rq, r);
	}
}

// Вывод результатов в выходной файл
// с удалением результатов
void printResultQueue(struct ResultQueue *rq, FILE *out) {
	while (rq->n > 0) {
		struct Result *r = popResultQueue(rq);
		printResult(r, out);
		freeResult(r);
	}
}