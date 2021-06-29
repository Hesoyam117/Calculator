#include <stdio.h>
#include <stdlib.h>

//Использование стека для обратной польской нотации.
//Входные данные:
//Пример: 33 2 + 2 * 21 - n
//В конце каждой строки пишем n
//Промежуточные и итоговый результаты: 35.00 70.00 49.00

typedef struct element
{
    float data;
    struct element *next;
} stack;

stack *head_stack = NULL;

void push_element(stack **head, float data);
float pop_element(stack **head);
float numb(char sign, float x, float y);

int main(int argc, char *argv[])
{

    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    char n = 'y', str[50], t = '0';
    char in[50], out[50];

    int i = 0;
    float num, x, y;

    FILE *input, *output;

    while (n == 'y')
    {
        printf("Input file name: ");
        scanf(" %s", in);

        printf("Output file name: ");
        scanf(" %s", out);

        input = fopen(in, "r");
        output = fopen(out, "w");

        while (feof(input) == 0)
        {
            fscanf(input, "%c", &str[i]);
            if (str[i] == ' ')
            {
                if (t == '0')
                {
                    num = strtof(str, NULL);
                    //Делит строку на отдельные элементы
                    push_element(&head_stack, num);
                    i = 0;
                }
            }
            else if (str[i] == '*' || str[i] == '+' || str[i] == '-' || str[i] == '/' || str[i] == '^')
            {
                y = pop_element(&head_stack);
                x = pop_element(&head_stack);

                push_element(&head_stack, numb(str[i], x, y));
                fprintf(output, "%.2f  ", numb(str[i], x, y));
                t = '1';
            }
            else if (str[i] == 'n')
            {
                while (head_stack != NULL)
                    pop_element(&head_stack);
                fprintf(output, "\n");
            }
            else
            {
                t = '0';
                i = i + 1;
            }
        }

        fclose(input);
        fclose(output);

        printf("Do you want to continue? (y/n) ");
        scanf(" %c", &n);
    }
    return 0;
}

float numb(char sign, float x, float y)
{
    float res, f, S;
    switch (sign)
    {
    case '+':
        res = x + y;
        return res;
        break;
    case '-':
        res = x - y;
        return res;
        break;
    case '*':
        res = x * y;
        return res;
        break;
    case '/':
        if (y != 0)
            res = x / y;
        return res;
        break;
    case '!':
        f = 1;
        for (int i = 1; i <= x; i++)
        {
            f *= i;
        }
        res = f;
        return res;
    case '^':
        f = 1;
        S = 1;
        for (int i = 1; i <= y; i++)
        {
            S *= x;
        }
        res = S;
        return res;
    }
    return x;
}

void push_element(stack **head, float data)
{
    stack *z = malloc(sizeof(stack));
    z->next = *head;
    z->data = data;
    *head = z;
}

float pop_element(stack **head)
{
    stack *z;
    float data;
    z = *head;
    *head = (*head)->next;
    data = z->data;
    return data;
}