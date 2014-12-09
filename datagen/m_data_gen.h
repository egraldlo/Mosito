/*
 * m_data_gen.h
 *
 *  Created on: Dec 9, 2014
 *      Author: Casa
 */

#ifndef M_DATA_GEN_H_
#define M_DATA_GEN_H_

#include <iostream>
#include <string>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class DataGen {
public:
	DataGen();
	virtual ~DataGen();
};

int main(int argc, char **argv) {
	cout<<"this is a simple data generator!"<<endl
			<<"note: you can use 'datagen 3' to generate 3 pages"<<endl;

	string name="table.left";
	FILE *table=fopen(name.c_str(), "w");

	/* 64*1024-8 space to store data. */
	const char *arg=argv[1];
	int pages=atoi(arg);

	int zero=0;
	int number=2340;
	char *line=(char *)malloc(28);
	char *tail=(char *)malloc(8);
	memcpy(tail, &zero, 4);
	memcpy(tail+4, &zero, 4);
	memcpy(tail+8, &zero, 4);
	memcpy(tail+12, &number, 4);

	cout<<"we will generate "<<pages<<" pages!"<<endl;
	while(pages!=0) {
		int tuples=2340;
		while(tuples!=0) {
			memset(line, 0, sizeof(line));
			unsigned long attr1=rand()%10000;
			memcpy(line, &attr1, 8);
			int attr2=rand()%10000;
			memcpy(line+8, &attr2, 4);
			int attr3=rand()%10000;
			memcpy(line+12, &attr3, 4);
			int attr4=rand()%10000;
			memcpy(line+16, &attr4, 4);
			int attr5=rand()%10000;
			memcpy(line+20, &attr5, 4);
			int attr6=rand()%10000;
			memcpy(line+24, &attr6, 4);

			fwrite(line, 28, 1, table);
			tuples--;
		}

		fwrite(tail, 16, 1, table);

		cout<<"already write "<<pages--<<" pages!"<<endl;
	}

	fclose(table);

	return 0;
}

#endif /* M_DATA_GEN_H_ */
