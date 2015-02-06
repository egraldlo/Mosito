/*
 * m_histogram.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: Casa
 */

#include "m_histogram.h"

Histogram::Histogram() {
	// TODO Auto-generated constructor stub
	for(unsigned i=0; i<HIST_SIZE; i++) {
		histogram_[i]=0;
	}
}

Histogram::~Histogram() {
	// TODO Auto-generated destructor stub
}

void *Histogram1::pthread_e(void *args) {
	Args *ar=(Args *)args;
	BufferIterator *bi=0;
	void *tuple=0;
	for(unsigned i=ar->i; i<ar->ts->blocks_.size(); ) {
		bi=ar->ts->blocks_[i]->createIterator();
		unsigned long value;
		unsigned range=0;
		while((tuple=bi->getNext())!=0) {
			value=*(unsigned long *)((char *)tuple+8);
			range=(value*HIST_SIZE)/10000000;
			ar->ts->hist_[range]->increase();
		}
		i=i+CPU_CORE;
	}
	return 0;
}

void Histogram1::pthreads_eval(const char *path) {
	FILE *file=fopen(path,"rb");
	int size=0;
	char *buffer_=(char *)malloc(BLOCK_SIZE);
	while((size=fread(buffer_, 1, BLOCK_SIZE, file))!=0) {
		Block *block=new Block(BLOCK_SIZE);
		block->storeBlock(buffer_, size);
		blocks_.push_back(block);
	}
	fclose(file);

	BufferIterator *bi=0;
	unsigned long long timer_;
	startTimer(&timer_);
	for(unsigned i=0; i<CPU_CORE; i++) {
		Args ag;
		ag.i=i;
		ag.ts=this;
		pthread_create(&pths_[i], 0, pthread_e, &ag);
	}

	for(unsigned i=0; i<CPU_CORE; i++) {
		pthread_join(pths_[i], 0);
	}

	cout<<"timer: "<<getSecond(timer_)<<endl;
}

void Histogram::eval(const char *path) {
	FILE *file=fopen(path,"rb");
	int size=0;
	char *buffer_=(char *)malloc(BLOCK_SIZE);
	while((size=fread(buffer_, 1, BLOCK_SIZE, file))!=0) {
		Block *block=new Block(BLOCK_SIZE);
		block->storeBlock(buffer_, size);
		blocks_.push_back(block);
	}
	fclose(file);

	BufferIterator *bi=0;
	unsigned long long timer_;
	startTimer(&timer_);
	for(unsigned i=0; i<blocks_.size(); i++) {
		bi=blocks_[i]->createIterator();
		void *tuple=0;
		unsigned range=0;
		while((tuple=bi->getNext())!=0) {
			unsigned long value=*(unsigned long *)((char *)tuple+8);
			histogram_[(value*HIST_SIZE)/10000000]++;
		}
	}
	cout<<"timer: "<<getSecond(timer_)<<endl;
}

void Histogram::getbound(unsigned long &start, unsigned long &end) {
	BufferIterator *bi=0;
		unsigned long long timer_;
		startTimer(&timer_);
		for(unsigned i=0; i<blocks_.size(); i++) {
			bi=blocks_[i]->createIterator();
			void *tuple=0;
			unsigned range=0;
			while((tuple=bi->getNext())!=0) {
				unsigned long value=*(unsigned long *)((char *)tuple+8);
				if(value>end) end=value;
				if(value<start) start=value;
			}
		}
		cout<<"timer:---- "<<getSecond(timer_)<<endl;
}

void *Histogram1::bound(void *args) {
	Args1 *ar=(Args1 *)args;
	BufferIterator *bi=0;
	void *tuple=0;
	for(unsigned i=ar->i; i<ar->ts->blocks_.size(); ) {
		bi=ar->ts->blocks_[i]->createIterator();
		unsigned long value;
		unsigned range=0;
		while((tuple=bi->getNext())!=0) {
			value=*(unsigned long *)((char *)tuple+8);
			if(value>ar->ts->end_[ar->i]) ar->ts->end_[ar->i]=value;
			if(value<ar->ts->start_[ar->i]) ar->ts->start_[ar->i]=value;
		}
		i=i+CPU_CORE;
	}
	return 0;

}

void Histogram1::getbound(unsigned long &start, unsigned long &end) {
	unsigned long long timer_;
	startTimer(&timer_);
	for(unsigned i=0; i<CPU_CORE; i++) {
		Args ag;
		ag.i=i;
		ag.ts=this;
		pthread_create(&pths_[i], 0, bound, &ag);
	}

	for(unsigned i=0; i<CPU_CORE; i++) {
		pthread_join(pths_[i], 0);
	}

	cout<<"timer: ---"<<getSecond(timer_)<<"      start: "<<start_[0]<<"  end: "<<end_[0]<<endl;
}

