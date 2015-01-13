/*
 * m_data_gen.h
 *
 *  Created on: Dec 9, 2014
 *      Author: Casa
 */

#ifndef M_DATA_GEN_H_
#define M_DATA_GEN_H_

#include <iostream>
#include <sstream>
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
	cout<<"this is a simple data generator!"<<endl;

	/*
	 * 1st args: pages number
	 * 2nd args: left table name
	 * 3rd args: right table name
	 * 4th args: random gain.
	 *
	 * example: dgen 3 left right 1000.
	 *          to generate two tables which are left table and right table
	 *          which has three pages.
	 *  */

	/* 64*1024-8 space to store data. */
	const char *arg=argv[1];
	int pages=atoi(arg);
	int pages1=pages;
	const char *arg4=argv[4];
	int gain=atoi(arg4);

	int zero=0;
	int number=2340;
	char *line=(char *)malloc(28);
	char *tail=(char *)malloc(8);
	memcpy(tail, &zero, 4);
	memcpy(tail+4, &zero, 4);
	memcpy(tail+8, &zero, 4);
	memcpy(tail+12, &number, 4);

	string left(argv[2]);
	string right(argv[3]);

	stringstream s_left,s_right;
	s_left<<"table."<<left;
	s_right<<"table."<<right;

	/* left table */
	const char * l_name=s_left.str().c_str();
	FILE *ltable=fopen(l_name, "w");

	cout<<"we will generate "<<pages<<" pages!"<<endl;
	while(pages!=0) {
		int tuples=2340;
		while(tuples!=0) {
			memset(line, 0, sizeof(line));
			unsigned long attr1=rand()%gain;
			memcpy(line, &attr1, 8);
			int attr2=rand()%gain;
			memcpy(line+8, &attr2, 4);
			int attr3=rand()%gain;
			memcpy(line+12, &attr3, 4);
			int attr4=rand()%gain;
			memcpy(line+16, &attr4, 4);
			int attr5=rand()%gain;
			memcpy(line+20, &attr5, 4);
			int attr6=rand()%gain;
			memcpy(line+24, &attr6, 4);

			fwrite(line, 28, 1, ltable);
			tuples--;
		}

		fwrite(tail, 16, 1, ltable);

		cout<<"already write "<<pages--<<" pages!"<<endl;
	}

	fclose(ltable);

	/* right table */
		const char * r_name=s_right.str().c_str();
		FILE *rtable=fopen(r_name, "w");

		cout<<"we will generate "<<pages1<<" pages!"<<endl;
		while(pages1!=0) {
			int tuples=2340;
			while(tuples!=0) {
				memset(line, 0, sizeof(line));
				unsigned long attr1=rand()%gain;
				memcpy(line, &attr1, 8);
				int attr2=rand()%gain;
				memcpy(line+8, &attr2, 4);
				int attr3=rand()%gain;
				memcpy(line+12, &attr3, 4);
				int attr4=rand()%gain;
				memcpy(line+16, &attr4, 4);
				int attr5=rand()%gain;
				memcpy(line+20, &attr5, 4);
				int attr6=rand()%gain;
				memcpy(line+24, &attr6, 4);

				fwrite(line, 28, 1, rtable);
				tuples--;
			}

			fwrite(tail, 16, 1, rtable);

			cout<<"already write "<<pages1--<<" pages!"<<endl;
		}

		fclose(rtable);

	return 0;
}

#endif /* M_DATA_GEN_H_ */
