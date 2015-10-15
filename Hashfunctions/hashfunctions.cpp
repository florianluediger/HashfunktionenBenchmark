#include "hashfunctions.h"

using namespace std;

unsigned long Hashtest::jenkins_one_at_a_time_hash(char *str, int len)
{
	

	unsigned long hash = 0;
    for(hash = 0; *str; str++)
    {
        hash += *str;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
	
	//return hash;
	return hash % CAPACITY;
}

unsigned long Hashtest::fnv_hash(char *str, int len)
{
	unsigned long hash = 2166136261;
	while(*str)
	{
		hash += (hash<<1) + (hash<<4) + (hash<<7) + (hash<<8) + (hash<<24);
		hash ^= (unsigned long)*str++;
	}
	//return hash;
	return hash % CAPACITY;
}

unsigned long Hashtest::knuth_hash(char *str, int len) 
{
	unsigned long hash;
	for (hash=len; len--;) 
	{
		hash = ((hash<<5)^(hash>>27))^*str++;
	}
	//return hash;
	return hash % CAPACITY;
}

unsigned long Hashtest::Kernighan_Ritchie(char *str, int len)
{
	unsigned long hash = 0;
	int c;
	while (c = *str++)
		hash += c;
	//return hash;
	return hash % CAPACITY;
}

Hashtest::Hashtest(unsigned long (*func)(char *, int))
{
	elementCount = 0;
	hashfu = func;
	table = (int *)calloc(CAPACITY, sizeof(int));
	for(int i = 0; i < CAPACITY; i++)
		table[i] = 0;
}

Hashtest::~Hashtest()
{
	free(table);
}

int Hashtest::file_size(char *path)
{
	FILE *pf;
	int len;
	pf = fopen(path, "r");

	if (!pf)
	{
		std::cout << "Datei nicht gefunden!" << endl;
		system("PAUSE");
		return 0;
	}

	fseek(pf, 0, SEEK_END);
	len = ftell(pf);
	fclose(pf);
	return len;
}

int Hashtest::readFromFile( char *path)
{
	char *str, *ptr, *splitString;
	int len, var = 0;
	fstream *file = new fstream(path, fstream::in);
	
	if(false == file->good() || !(len = file_size(path)))
	{
		cout << "Datei nicht gefunden!" << endl;
		system("PAUSE");
		return 0;
	}

	str = (char *)malloc(len+1);
	file->read(str, len);
	file->close();
	ptr = str;

	while(*ptr != str[len])	//Dies dient nur dazu Müll am Ende des Strings zu entfernen
		ptr++;
	*ptr = '\0';
	cout << "Datei gelesen: " << file->gcount() << endl;
	cout << "LEN: " << len << endl;
	
	splitString = strtok(str, "\n");
	while(splitString != NULL)
	{
		insert(splitString);
		splitString = strtok(NULL, "\n");
		var++;
	}
	cout << "VAR: " << var << endl;
	free(str);
	return 1;
}

int Hashtest::readStrings(char *path)
{
	char *str, *ptr, *splitString;
	int len, var = 0, i = 0;
	fstream *file = new fstream(path, fstream::in);
	
	if(false == file->good() || !(len = file_size(path)))
	{
		cout << "Datei nicht gefunden!" << endl;
		system("PAUSE");
		return 0;
	}

	str = (char *)malloc(len+1);
	file->read(str, len);
	file->close();
	ptr = str;

	while(*ptr != str[len])	//Dies dient nur dazu Müll am Ende des Strings zu entfernen
		ptr++;
	*ptr = '\0';
	cout << "Datei gelesen: " << file->gcount() << endl;
	cout << "LEN: " << len << endl;
	
	splitString = strtok(str, "\n");
	while(splitString != NULL)
	{
		memcpy(strings[i],splitString,MAXLEN);
		strings[i][MAXLEN] = '\0';
		//strcpy(strings[i],splitString);
		splitString = strtok(NULL, "\n");
		i++;
		var++;
	}
	cout << "VAR: " << var << "I: " << i << endl;
	free(str);
	return 1;
}

void Hashtest::insert(char *text)
{
	table[hashfu(text, sizeof(text))]++;
	elementCount++;
}

void Hashtest::testfu()
{
	int maxCollisions = 0,
		minCollisions = 10000,
		emptyCount = 0,
		variance = 0;
	double standardDeviation = 0,
		average = (double)elementCount/(double)CAPACITY;

	for(int i = 0; i < CAPACITY; i++)
	{
		if(table[i] > maxCollisions)
			maxCollisions = table[i];
		if(table[i] < minCollisions)
			minCollisions = table[i];
		if(table[i] == 0)
			emptyCount++;

		variance += (table[i] - average)*(table[i] - average);
	}
	variance /= CAPACITY;
	standardDeviation = sqrt(variance);
	
	cout << "Anzahl eingefuegter Elemente: " << elementCount << endl;
	cout << "Fuellgrad (=Mittelwert): " << average << endl;
	cout << "Standardabweichung: " << standardDeviation << endl;
	cout << "Groesste Anzahl Kollisionen: " << maxCollisions << endl;
	cout << "Kleinste Anzahl Kollisionen: " << minCollisions << endl;
	cout << "Anzahl leerer Felder: " << emptyCount << endl;
	cout << "#########################################" << endl;
	cout << "STABW: " << standardDeviation << endl;
	cout << "LEERE ZELLEN: " << emptyCount << endl;
	cout << "MAX KOLL: " << maxCollisions << endl;
}

void Hashtest::print()
{
	for(int i = 0; i < CAPACITY; i++)
	{
		cout << setw(10) << setiosflags(ios::left) << i
			<< resetiosflags(ios::left) << table[i] << endl;
	}
}

void Hashtest::runtime_test()
{
	LONGLONG g_Count1, g_Count2, g_Freq;

	if(!QueryPerformanceFrequency((LARGE_INTEGER *)&g_Freq))
		cout << "Performance Counter nicht vorhanden" << endl;

	QueryPerformanceCounter((LARGE_INTEGER *)&g_Count1);

	for(int i = 0; i < 900000; i++)
	{
		hashfu(strings[i], MAXLEN);
	}

	QueryPerformanceCounter((LARGE_INTEGER *)&g_Count2);

	double benchmarkTime = (((double)(g_Count2 - g_Count1))/((double)g_Freq)) * 1000.0;

	cout << "Das Hashen hat " << benchmarkTime << "ms gedauert" << endl;
}