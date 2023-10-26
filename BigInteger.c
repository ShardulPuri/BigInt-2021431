#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Added to include string functions
#include"BigInteger.h"
// Define the node structure
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
BigInt sub(BigInt num1, BigInt num2);
// Function to create a BigInt from a string
BigInt initialize(const char* s) {
    BigInt b;
    b.head = NULL;
    b.length = 0;
    b.sign = 1;

    if (*s == '-') {
        b.sign = -1;
        s++;
    }

    while (*s) {
        node* temp = (node*)malloc(sizeof(node));
        if (!temp) {
            printf("Memory Allocation Failed");
            exit(1);
        }
        temp->data = *s - '0'; // Convert character to integer
        temp->next = b.head;
        b.head = temp;
        b.length++;
        s++;
    }

    return b;
}
//Function to compare
int compare(BigInt num1,BigInt num2)
{
    if(num1.length>num2.length)
    {
        return 1;
    }
    if(num1.length<num2.length)
    {
        return -1;
    }
    else
    {
        node* itr1=num1.head;
        node* itr2=num2.head;
        int res;
        while(itr1)
        {
            if(itr1->data>itr2->data)
            {
                res=1;
            }
            else if(itr1->data<itr2->data)
            {
                res=-1;
            }
            itr1=itr1->next;
            itr2=itr2->next;
        }
        return res;
    }
}
// Function to display a BigInt
void display(node* itr) {
    if (itr) {
        display(itr->next);
        printf("%d", itr->data);
    }
}
int zero(BigInt num,struct Node *n){
    if(!n)
        return 0;
    if(zero(num,n->next))
        return 1;
    else{
        if(n->next)
            free(n->next);
        n->next=NULL;
        return n->data;
    }

}
BigInt add(BigInt num1, BigInt num2) {
    if (num1.sign == -1 && num2.sign == 1) {
        // If num1 is negative and num2 is positive, we subtract num2 from num1
        num2.sign = -1; // Change num2 to negative
        return sub(num2, num1);
    }
    if (num1.sign == 1 && num2.sign == -1) {
        // If num1 is positive and num2 is negative, we subtract num1 from num2
        num2.sign = 1; // Change num2 to positive
        return sub(num1, num2);
    }

    int sum = 0, carry = 0;
    node* itr1 = num1.head;
    node* itr2 = num2.head;
    BigInt result;
    result.head = NULL;
    result.length = 0;

    while (itr1 || itr2 || carry) {
        sum = carry;

        if (itr1) {
            sum += itr1->data;
            itr1 = itr1->next;
        }
        if (itr2) {
            sum += itr2->data;
            itr2 = itr2->next;
        }

        carry = sum / 10;
        node* temp = (node*)malloc(sizeof(node));
        if (!temp) {
            printf("Memory Allocation Failed");
            exit(1);
        }
        temp->data = sum % 10;
        temp->next = NULL;

        // Append the new node to the result in the correct order
        if (result.head == NULL) {
            result.head = temp;
        } else {
            node* tail = result.head;
            while (tail->next != NULL) {
                tail = tail->next;
            }
            tail->next = temp;
        }

        result.length++;
    }

    return result;
}

BigInt sub(BigInt num1, BigInt num2) {
    if((num1.sign==1 && num2.sign==-1))
    {
        num2.sign=1;
        return add(num1,num2);
    }
    if((num1.sign==-1 && num2.sign==1))
    {
        num1.sign=1;
        return add(num1,num2);
    }
    int diff, borrow = 0;
    BigInt result;
    result.head = NULL;
    result.length = 0;
    node* itr1;
    node* itr2;
    int k=compare(num1,num2);
    if(k==1)
    {
        itr1 = num1.head;
        itr2 = num2.head;
    }
    else
    {
        itr1 = num2.head;
        itr2 = num1.head;
    }
    while (itr1 || borrow) {
        diff = borrow;

        if (itr1) {
            diff += itr1->data;
            itr1 = itr1->next;
        }
        if (itr2) {
            diff -= itr2->data;
            itr2 = itr2->next;
        }
        if (diff < 0) {
            diff += 10;
            borrow = -1;
        } else {
            borrow = 0;
        }

        node* temp = (node*)malloc(sizeof(node));
        if (!temp) {
            printf("Memory Allocation Failed");
            exit(1);
        }
        temp->data = diff;
        temp->next = NULL;

        if (result.head == NULL) {
            result.head = temp;
        } else {
            node* tail = result.head;
            while (tail->next != NULL) {
                tail = tail->next;
            }
            tail->next = temp;
        }

        result.length++;
    }
    //truncate(result.head);
    zero(result,result.head);
    return result;
}
BigInt mul(BigInt num1, BigInt num2) {
    int product, carry = 0;
    BigInt result;
    result.head = NULL;
    result.length = 0;
    result.sign = num1.sign * num2.sign; // Determine the sign of the result

    // Handle the case when either operand is zero
    if ((num1.head->data == 0 && num1.length==1) || (num2.head->data == 0 && num2.length==1))
    {
        result.head = (node*)malloc(sizeof(node));
        result.head->data = 0;
        result.head->next = NULL;
        result.length = 1;
        return result;
    }

    node* itr2 = num2.head;
    int shift = 0;  // Number of positions to shift the result to the left

    while (itr2) {
        BigInt tempResult;
        tempResult.head = NULL;
        tempResult.length = 0;
        carry = 0;

        node* itr1 = num1.head;
        while (itr1) {
            product = (itr1->data * itr2->data) + carry;
            carry = product / 10;

            node temp;
            temp.data = product % 10;
            temp.next = NULL;

            if (tempResult.head == NULL) {
                tempResult.head = (node*)malloc(sizeof(node));
                *tempResult.head = temp;
            } else {
                node* tail = tempResult.head;
                while (tail->next != NULL) {
                    tail = tail->next;
                }
                tail->next = (node*)malloc(sizeof(node));
                *(tail->next) = temp;
            }

            tempResult.length++;
            itr1 = itr1->next;
        }

        // Add any remaining carry to the result
        if (carry > 0) {
            node temp;
            temp.data = carry;
            temp.next = NULL;
            node* tempTail = tempResult.head;
            while (tempTail->next != NULL) {
                tempTail = tempTail->next;
            }
            tempTail->next = (node*)malloc(sizeof(node));
            *(tempTail->next) = temp;
            tempResult.length++;
        }

        // Shift the result to the left (multiply by 10) for the next iteration
        for (int i = 0; i < shift; i++) {
            node temp;
            temp.data = 0;
            temp.next = tempResult.head;
            tempResult.head = (node*)malloc(sizeof(node));
            *(tempResult.head) = temp;
            tempResult.length++;
        }

        // Add the temporary result to the final result
        result = add(result, tempResult);

        itr2 = itr2->next;
        shift++;
    }
    return result;
}
BigInt div1(BigInt a,BigInt b)
{

    BigInt quo,dif;
    quo.head   = NULL;
    quo.sign   = 1;
    quo.length = 0;

    if(!a.head || ! b.head)
        return quo;

    if((b.length==1)&&(b.head->data==0))
        return quo;

    if(((a.length==1)&&(a.head->data==0)) || (a.length<b.length)){
        quo.head   = (struct Node * )malloc(sizeof(struct Node));
        quo.head->data=0;
        quo.head->next=NULL;
        quo.length = 1;
        return quo;
    }
    int i=0,d,q=0;
    struct Node *temp=NULL,*stop=b.head;
    d = a.length - b.length;

    while(i<d){
        temp=(struct Node *)malloc(sizeof(struct Node ));
        temp->data  = 0 ;
        temp->next = b.head;
        b.head = temp;
        i++;
    }


    char asign=a.sign,bsign=b.sign;
    a.sign=b.sign=1;


    while(1){

        //dif=sub(a,b);

        if(compare(a,b)){
            struct Node  *t;
            if(b.head!=stop){
                t = b.head;
                b.head=b.head->next;
                free(t);
                d--;

                t=(struct Node *)malloc(sizeof(struct Node ));
                t->data = q;
                t->next = quo.head;
                quo.head = t;
                quo.length++;
                q=0;

            }
            else{
                t=(struct Node *)malloc(sizeof(struct Node ));
                t->data = q;
                t->next = quo.head;
                quo.head = t;
                quo.length++;
                q=0;

                a.sign=asign;
                b.sign=bsign;

                if((a.sign==-1)^(b.sign==-1))
                    quo.sign=-1;

                zero(quo,quo.head);

                quo.length=0;
                struct Node  * itr4=quo.head;
                while(itr4){
                    quo.length++;
                    itr4=itr4->next;
                }



                return quo;
            }
        }
        else{
            q++;
            a=sub(a,b);

        }
    }
}
