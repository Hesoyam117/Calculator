#include <stdio.h>
#include <stdlib.h>

struct list1
{
    char sign, choose;
    int size;
    float *x, *y;
    struct list1 *next;
    //Добавление структур
};

struct list2
{
    float *res;
    struct list2 *res_next;
};

float *numb(char sign, float *x, float *y);
float *vect(char sign, int size, float *a, float *b);
float *add_numb(FILE *input, int size);
void add_el(struct list1 *current, FILE *input);
void res_add_el(struct list2 *res_current, struct list1 *current);

int main(int argc, char *argv[])
{
    char n = 'y';
    char in[50], out[50];
    FILE *input, *output;
    struct list1 *head, *current; 
    //указывает на начало списка и текущий элемент(head) и текущий элемент(current)
    struct list2 *head_res, *current_res;
    while (n == 'y')
    {
        printf("\nВведите название файла для входных данных (***.txt)");
        scanf("%s", in);
        printf("Введите название файла для выходных данных (***.txt)");
        scanf("%s", out);
        input = fopen(in, "r");
        if (feof(input) == 0)
        {
            head = malloc(sizeof(struct list1)); 
            // Выделение памяти для 1-го элемента 
            fscanf(input, " %c", &head->sign);
            fscanf(input, " %c", &head->choose);
            if (head->choose == 'v')
            {
                fscanf(input, " %i", &head->size);
            }
            else
            {
                head->size = 1;
            }
            if (head->sign != '!')
            {
                head->x = add_numb(input, head->size);
                head->y = add_numb(input, head->size);
            }
            else
            {
                head->x = add_numb(input, head->size);
                head->y = NULL;
            }
            current = head;

            while (feof(input) == 0)
            { 
                add_el(current, input);
                current = current->next;
            }
            head_res = malloc(sizeof(struct list2)); 
            // Выделение памяти для списка 
            current = head;
            if (current->choose == 'v')
            {
                head_res->res = vect(current->sign, current->size, current->x,
                                     current->y);
            }
            else
            {
                head_res->res = numb(current->sign, current->x, current->y);
            }
            head_res->res_next = NULL;
            current = current->next;
            current_res = head_res;
            while (current != NULL)
            { 
                res_add_el(current_res, current);
                // Переставляет указатель на след. элемент, пока не дойдет до последнего
                current = current->next;
                current_res = current_res->res_next;
            }
            current = head;
            current_res = head_res;
            fclose(input);
            output = fopen(out, "w");
            while (current != NULL) 
            //Выводит ответ
            {
                if (current->choose == 'v')
                {
                    fprintf(output, "(");
                    for (int i = 0; i < current->size; i++)
                    {
                        fprintf(output, " %.2f", current->x[i]);
                    }
                    fprintf(output, ") %c (", current->sign);
                    for (int i = 0; i < current->size; i++)
                    {
                        fprintf(output, " %.2f", current->y[i]);
                    }
                    fprintf(output, " ) = ");
                    if (current->sign != '*')
                    {
                        fprintf(output, "( ");
                        for (int i = 0; i < current->size; i++)
                        {
                            fprintf(output, "%.2f ", current_res->res[i]);
                        }
                        fprintf(output, ")\n");
                    }
                    else
                    {
                        fprintf(output, "%.2f\n", current_res->res[0]);
                    }
                }
                else if (current->choose == 's')
                {
                    fprintf(output, " %.2f %c %.2f = %.2f\n", current->x[0],
                            current->y[0], current->sign, current_res->res[0]);
                }
                current = current->next;
                current_res = current_res->res_next;
            }
            fclose(output);
        }
        printf("Продолжить работу?(y/n)");
        scanf("%s", &n);
    }
    return 0;
}

float *numb(char sign, float *x, float *y)
{
    float f, S, *res_numb;
    res_numb = malloc(sizeof(float));
    switch (sign)
    {
    case '+':
        res_numb[0] = x[0] + y[0];
        return res_numb;
    case '-':
        res_numb[0] = x[0] - y[0];
        return res_numb;
    case '*':
        res_numb[0] = x[0] * y[0];
        return res_numb;
    case '/':
        if (y != 0)
        {
            res_numb[0] = x[0] / y[0];
            return res_numb;
        }
        else
        {
            return 0;
        }
    case '!':
        f = 1;
        for (int i = 1; i <= x[0]; i++)
        {
            f *= i;
        }
        res_numb[0] = f;
        return res_numb;
    case '^':
        f = 1;
        S = 1;
        for (int i = 1; i <= y[0]; i++)
        {
            S *= x[0];
        }
        res_numb[0] = S;
        return res_numb;
    }
    return x;
    return y;
    free(x);
    free(y);
    free(res_numb);
}

float *vect(char sign, int size, float *a, float *b)
{
    float *res_vect;

    switch (sign)
    {
    case '+':
        res_vect = malloc(size * sizeof(float));
        for (int i = 0; i < size; i++)
        {
            res_vect[i] = a[i] + b[i];
        }
        return res_vect;

    case '-':
        res_vect = malloc(size * sizeof(float));
        for (int i = 0; i < size; i++)
        {
            res_vect[i] = a[i] - b[i];
        }
        return res_vect;

    case '*':
        res_vect = malloc(sizeof(float));
        res_vect[0] = 0;
        for (int i = 0; i < size; i++)
        {
            res_vect[0] += a[i] * b[i];
        }
        return res_vect;
    }
    return a;
    return b;
    free(a);
    free(b);
    free(res_vect);
}

//Считывает указатель
float *add_numb(FILE *input, int size)
{
    float *numb;
    numb = malloc(size * sizeof(float));
    for (int i = 0; i < size; i++)
    {
        fscanf(input, "%f", &numb[i]);
    }
    return numb;
}

//Добавляет элемент в список
void add_el(struct list1 *current, FILE *input)
{
    struct list1 *z = malloc(sizeof(struct list1));
    fscanf(input, " %c", &z->sign);
    fscanf(input, " %c", &z->choose);
    if (z->choose == 'v')
    {
        fscanf(input, " %i", &z->size);
    }
    else
    {
        z->size = 1;
    }
    if (z->sign != '!')
    {
        z->x = add_numb(input, z->size);
        z->y = add_numb(input, z->size);
    }
    else
    {
        z->x = add_numb(input, z->size);
        z->y = NULL;
    }
    z->next = NULL;    // Последний элемент списка
    current->next = z; // Установка указателя
}

void res_add_el(struct list2 *res_current, struct list1 *current)
{
    struct list2 *z_res = malloc(sizeof(struct list1));
    if (current->choose == 'v')
    {
        z_res->res = vect(current->sign, current->size, current->x, current->y);
    }
    else
    {
        z_res->res = numb(current->sign, current->x, current->y);
    }
    z_res->res_next = NULL;
    res_current->res_next = z_res;
}