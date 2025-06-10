#include "std_lib.h"

int div(int a, int b)
{
  int negative = 0;
  int res = 0;

  if (b == 0) {
    printString("Error: division by zero\r\n");
    return 0;  // bisa juga return INT_MAX atau nilai error lain
  }

  if (a < 0) {
    a = -a;
    negative = !negative;
  }
  if (b < 0) {
    b = -b;
    negative = !negative;
  }

  while (a >= b) {
    a -= b;
    res++;
  }

  return negative ? -res : res;
}

int mod(int a, int b)
{
   return a - div(a, b) * b;
}

void clear(byte *buf, unsigned int size)
{
  unsigned int i;
  for (i = 0; i < size; i++) {
    buf[i] = 0;
  }
}

int strcmp(char *str1, char *str2) {
  int i = 0; // initialize index

  while (str1[i] != '\0' && str2[i] != '\0') {
    if (str1[i] != str2[i]) {
      return (unsigned int)str1[i] - (unsigned int)str2[i]; // return difference if characters differ
    }
    i++;
  }
  return (unsigned int)str1[i] - (unsigned int)str2[i]; // return difference at the end of the strings, which is 0, meaning they are equal
}

void strcpy(char *dst, char *src)
{
  int i = 0; // initialize index

  while (src[i] != '\0') { // copy until null terminator
    dst[i] = src[i];
    i++;
  }
  dst[i] = '\0'; // add null terminator to the end of the destination string
}

/*void clear(byte *buf, unsigned int size)
{
  //TODO: Implementasi pembersihan buffer
}
  */

void atoi(char *str, int *num)
{
  int i = 0;
  int res = 0;
  int negative = 0;
  char c;

  if (str[0] == '-') {
    negative = 1;
    i++;
  }

  while ((c = str[i]) != '\0') {
    res = res * 10 + (c - '0');
    i++;
  }

  if (negative) {
    res = -res;
  }

  *num = res;
}


void itoa(int num, char *str)
{
  int i = 0;
  int negative = 0;
  int start, end;
  char temp;
  int digit;

  if (num == 0) {
    str[i++] = '0';
    str[i] = '\0';
    return;
  }

  if (num < 0) {
    negative = 1;
    num = -num;
  }

  while (num > 0) {
    digit = mod(num, 10);       // Ganti num % 10
    str[i++] = digit + '0';     //simpan angka ke str
    num = div(num, 10);         // Ganti num / 10
  }

  if (negative) {
    str[i++] = '-';
  }

  str[i] = '\0';

  // Reverse string
  start = 0;
  end = i - 1;
  while (start < end) {
    temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    start++;
    end--;
  }
}
