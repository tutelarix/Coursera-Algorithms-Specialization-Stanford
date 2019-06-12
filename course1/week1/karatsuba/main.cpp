#include <iostream>
#include <stdio.h>

using namespace std;


#define Size 1024
#define ReturnSize 1024

bool isZero(unsigned char * a, int lenA)
{
    for (int index = Size - lenA; index < Size; index++)
    {
        if (a[index] != 0) return false;
    }

    return true;
}

void shiftRightMax(unsigned char *array, int len)
{
    for (int i = 0; i< len; i++)
    {
        array[Size - len + i] = array[i];
        array[i] = 0;
    }
}

void shiftLeft(unsigned char *array, int &len, int pos)
{
    if (isZero(array, len)) return;

    for (int i = Size - len; i< Size; i++)
    {
        array[i - pos] = array[i];
        array[i] = 0;
    }
    len += pos;
}

void add(unsigned char *a, int & lenA, unsigned char *b, int lenB)
{
    unsigned char val_next = 0;
    unsigned char val = 0;
    int maxLen = lenA > lenB ? lenA : lenB;
    int index = 0;
    lenA = maxLen;

    for (index = Size - 1; index > Size - maxLen - 1; index--)
    {
        val = a[index] + b[index] + val_next;
        val_next = val / 10;
        a[index] = val % 10;
    }
    if (val_next != 0) {
        a[index] = val_next;
        lenA++;
    }
}

bool substract(unsigned char *a, int & lenA, unsigned char *b, int lenB)
{
    unsigned char * first;
    unsigned char * second;
    bool sign = true;
    int maxLen = lenA;

    if (lenA > lenB)
    {
        first = a;
        second = b;
    }
    else if (lenB > lenA) {
        first = b;
        second = a;
        sign = false;
        maxLen = lenB;
    }
    else
    {
        int i = 0;
        for (i = Size - lenA; i < Size; i++)
        {
            if (a[i] != b[i])
            {
                break;
            }
        }

        if (i == Size)
        {
            for (int index = Size - maxLen; index < Size; index++)
            {
                a[index] = 0;
            }
            lenA = 1;
            return sign;
        }
        else if (a[i] > b[i])
        {
            first = a;
            second = b;
        }
        else
        {
            first = b;
            second = a;
            sign = false;
        }
    }

    char val_next = 0;
    char val = 0;
    int index = 0;

    for (index = Size - 1; index > Size - maxLen - 1; index--)
    {
        if (first[index] - val_next < second[index]) {
            a[index] = first[index] + 10 - val_next - second[index];
            val_next = 1;
        }
        else
        {
            a[index] = first[index] - val_next - second[index];
            val_next = 0;
        }
    }

    for (index = Size - maxLen; index < Size; index++)
    {
        if (a[index] != 0) break;
    }
    lenA = Size - index;

    return sign;
}

void karatsuba(unsigned char *a, int lenA, unsigned char *b, int lenB, unsigned char *result, int & lenResult)
{
    if (lenA == 1 && lenB == 1)
    {
        int val = a[Size - 1] * b[Size - 1];
        result[ReturnSize - 1] = val % 10;
        result[ReturnSize - 2] = val / 10;
        lenResult = 1;
        if (result[ReturnSize - 2] != 0)
            lenResult = 2;
        return;
    }

    unsigned char z1[ReturnSize] = {};
    int lenz1 = 0;
    unsigned char z2[ReturnSize] = {};
    int lenz2 = 0;
    unsigned char z3[ReturnSize] = {};
    int lenz3 = 0;

    unsigned char x1[Size] = {};
    unsigned char y1[Size] = {};

    unsigned char x0[Size] = {};
    unsigned char y0[Size] = {};


    int maxLen = lenA > lenB ? lenA : lenB;
    int bm = maxLen / 2;

    int startIndex = Size - (maxLen - bm);
    for (int i = Size - maxLen; i < Size-bm; i++)
    {
        x1[startIndex] = a[i];
        y1[startIndex] = b[i];
        startIndex++;
    }

    startIndex = Size - bm;
    for (int i = Size - bm; i < Size; i++)
    {
        x0[startIndex] = a[i];
        y0[startIndex] = b[i];
        startIndex++;
    }

    karatsuba(x1, maxLen - bm, y1, maxLen - bm, z1, lenz1);
    karatsuba(x0, bm, y0, bm, z2, lenz2);

    int lenx1plusx0 = maxLen - bm;
    add(x1, lenx1plusx0, x0, bm);
    int leny1plusy0 = maxLen - bm;
    add(y1, leny1plusy0, y0, bm);

    karatsuba(x1, lenx1plusx0, y1, leny1plusy0, z3, lenz3);
    substract(z3, lenz3, z1, lenz1);
    substract(z3, lenz3, z2, lenz2);

    shiftLeft(z1, lenz1, (bm) * 2);
    shiftLeft(z3, lenz3, (bm));

    lenResult = 1;
    add(result, lenResult, z1, lenz1);
    add(result, lenResult, z2, lenz2);
    add(result, lenResult, z3, lenz3);
}




int main()
{
    cout << "Provide two numbers" << endl;
    unsigned char num1[Size] = {};
    int len1 = 0;
    unsigned char num2[Size] = {};
    int len2 = 0;
    unsigned char result[ReturnSize] = {};
    int lenResult = 0;

    bool isFirstNumberEntered = false;
    unsigned char c;
    int index = 0;
    unsigned char * readingArray = num1;


    // Read data
    do {
        c = getchar();
        if (c == '\n' || c == ' ')
        {
            if (isFirstNumberEntered)
            {
                len2 = index;
                break;
            }
            else
            {
                isFirstNumberEntered = true;
            }
            len1 = index;
            readingArray = num2;
            index = 0;
        }
        else
        {
            unsigned char value = c - '0';
            readingArray[index] = value;
            index++;
        }
    } while (true);

    shiftRightMax(num1, len1);
    shiftRightMax(num2, len2);

    //shiftLeft(num1, len1, 1);
    //shiftLeft(num2, len2, 2);

    //add(num1, len1, num2, len2);
    //bool res = substract(num1, len1, num2, len2);

    karatsuba(num1, len1, num2, len2, result, lenResult);

    printf("Result: ");
    for (int index = ReturnSize - lenResult; index < ReturnSize; index++)
    {
        printf("%d", result[index]);
    }
    //printf("   Sign: %d", res);
    printf("\n");

    return 0;
}


