#include "hashfunctions.h"

#define DATAPATH "testdatenHashfus.txt"

void main()
{
	Hashtest *test = new Hashtest(Hashtest::fnv_hash);
	test->readFromFile(DATAPATH);
	test->testfu();
	//test->readStrings(DATAPATH);
	//test->runtime_test();
}