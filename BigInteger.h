
#ifndef BIGINTEGER_H
#define BIGINTEGER_H


#include<stdio.h>
#include<stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} node;

// Define the BigInt structure
typedef struct BigInteger {
    node* head;
    int length;
    int sign;
} BigInt;

struct BigInteger initialize(char *);
struct BigInteger add(struct BigInteger,struct BigInteger);
struct BigInteger sub(struct BigInteger ,struct BigInteger);
struct BigInteger mul(struct BigInteger,struct BigInteger);
struct BigInteger div1(struct BigInteger,struct BigInteger);
//struct BigInteger mod(struct BigInteger,struct BigInteger);

int  zero(struct BigInteger,struct Node *);
void display(struct BigInteger);

#endif
