#include <iostream>
using namespace std;

struct Stack
{
    int capacity = 100;
    char string_value[100];
    double num_value[100];
    int top = -1;
};

int size(Stack &st)
{
    return st.top + 1;
}

bool isEmpty(Stack &st)
{
    return st.top == -1 ? true : false;
}

bool isFull(Stack &st)
{
    return st.top >= (st.capacity - 1) ? true : false;
}

void push(Stack &st, char c,double i )
{
    if (isFull(st))
    {
        cout << "\nStack is full. Overflow condition!!!" << endl;
        return;
    }
    st.top++;
    st.string_value[st.top] = c;
    st.num_value[st.top] = i ;
}

char pop_char(Stack &st)
{
    char p;
    if (isEmpty(st))
    {
        cout << "\n Stack is empty!" << endl;
        return 'e';
        // err
    }
    p = st.string_value[st.top];
    st.top--;
    return p;
}

double pop_num(Stack &st)
{
    double p;
    if (isEmpty(st))
    {
        cout << "\n Stack is empty!" << endl;
        return -9999;
        // err
    }
    p = st.num_value[st.top];
    st.top--;
    return p;
}

char peek_char(Stack &st)
{
    if (isEmpty(st))
    {
        printf("Stack is empty");
        return 'e';
    }
    return st.string_value[st.top];
}


double peek_int(Stack &st)
{
    if (isEmpty(st))
    {
        printf("Stack is empty");
        return 'e';
    }
    return st.num_value[st.top];
}
