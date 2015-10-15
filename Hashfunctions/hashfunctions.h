#include <iostream>
#include <iomanip>
#include <fstream>
#include <Windows.h>

#define MAXLEN 30
#define CAPACITY 2097152

class Hashtest
{
public:
	static unsigned long jenkins_one_at_a_time_hash(char *str, int len);
	static unsigned long knuth_hash(char *str, int len);
	static unsigned long Kernighan_Ritchie(char *str, int len);
	static unsigned long fnv_hash(char *str, int len);
private:
	char strings[900000][300];
	int *table;
	unsigned long (*hashfu)(char *, int);
	int elementCount;
public:
	Hashtest(unsigned long (*func)(char *, int));
	~Hashtest();
	void testfu();
	int file_size(char *path);
	int readFromFile(char *path);
	int readStrings(char *path);
	void insert(char *text);
	void runtime_test();
	void print();
};