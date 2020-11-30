#include <iostream>
using namespace std;

struct Queue
{
    int capacity = 100;
    char string_value[100];
    double num_value[100];
    int front = 0;
    int rear = -1;
};

int size(Queue &st)
{
    return st.rear - st.front + 1;
}

bool isEmpty(Queue &st)
{
    return (st.front > st.rear || st.front < 0) ? true : false;
}

bool isFull(Queue &st)
{
    return (size(st) == st.capacity) ? true : false;
}

void offer(Queue &st, char c, double i)
{
    if (isFull(st))
    {
        cout << "\nQueue is full. Overflow condition!!!" << endl;
        return;
    }
    st.rear++;
    st.string_value[st.rear] = c;
    st.num_value[st.rear] = i;
}

char remove_char(Queue &st)
{
    char p;
    if (isEmpty(st))
    {
        cout << "\n Queue is empty!" << endl;
        return 'e';
        // err
    }
    p = st.string_value[st.front];
    st.front++;
    return p;
}
double remove_num(Queue &st)
{
    double p;
    if (isEmpty(st))
    {
        cout << "\n Queue is empty!" << endl;
        return -9999;
        // err
    }
    p = st.num_value[st.front];
    st.front++;
    return p;
}

char peek_char(Queue &st)
{
    if (isEmpty(st))
    {
        printf("Stack is empty");
        return 'e';
    }
    return st.string_value[st.front];
}

double peek_num(Queue &st)
{
    if (isEmpty(st))
    {
        printf("Stack is empty");
        return 'e';
    }
    return st.num_value[st.front];
}
