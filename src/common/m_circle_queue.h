/*
 * m_circle_queue.h
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#ifndef M_CIRCLE_QUEUE_H_
#define M_CIRCLE_QUEUE_H_

#include "m_buffer.h"

#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include <list>
#include <queue>
#include <iostream>
using namespace std;

template <class N>
struct CircleNode {
	N *node_;
	CircleNode<N> *next_;
};

/* circle queue which has a head node */
template <class Q>
class CircleQueue {
public:
	CircleQueue(int capacity):queue_capacity_(capacity){
		pthread_mutex_init(&lock_,0);
		sem_init(&full_,0,0);
		sem_init(&empty_,0,queue_capacity_);
		/* head node can be convenient for handle */
		head_=(CircleNode<Q>*)malloc(sizeof(CircleNode<Q>));
		head_->node_=0;head_->next_=0;
		CircleNode<Q> *temp,*real_head=0;
		CircleNode<Q> *cur_tail=head_;
		for(unsigned i=0;i<queue_capacity_+1;i++) {
			/* insert the node from the tail, by using a temp */
			temp=(CircleNode<Q>*)malloc(sizeof(CircleNode<Q>));
			temp->node_=0;temp->next_=0;
			if(i==0){
				front_=rear_=real_head=temp;
				cur_tail->next_=temp;
				cur_tail=cur_tail->next_;
			}
			else if(i==queue_capacity_){
				temp->next_=real_head;
				cur_tail->next_=temp;
				cur_tail=cur_tail->next_;//no need
			}
			else{
				cur_tail->next_=temp;
				cur_tail=cur_tail->next_;
			}
		}
	};

	virtual ~CircleQueue(){
		/* free the queue memory space. */
		CircleNode<Q> *p,*q=head_;
		for(unsigned i=0;i<=queue_capacity_+1;i++) {
			p=q;
			q=p->next_;
			free(p);
		}
		pthread_mutex_destroy(&lock_);
		sem_destroy(&full_);
		sem_destroy(&empty_);
	};

	bool push(Q *node) {
		/* if the queue is full, return false. eg:
		 * 0(head)+9(tail)=full, "(9-0+10)%(10)==9" is full like follows.
		 * */
		pthread_mutex_lock(&lock_);
		if(rear_->next_==front_){
			pthread_mutex_unlock(&lock_);
			return false;
		}
//		sem_wait(&empty_);
//		pthread_mutex_lock(&lock_);
		rear_->node_=node;
		rear_=rear_->next_;
//		sem_post(&full_);
		pthread_mutex_unlock(&lock_);
		return true;
	}

	Q* pop() {
		/* if the queue is empty, return 0 */
		pthread_mutex_lock(&lock_);
		if(rear_==front_) {
			pthread_mutex_unlock(&lock_);
			return 0;
		}
//		sem_wait(&full_);
//		pthread_mutex_lock(&lock_);
		Q *ret=front_->node_;
		front_=front_->next_;
//		sem_post(&empty_);
		pthread_mutex_unlock(&lock_);
		return ret;
	}

	bool empty() {
		return rear_==front_;
	}

private:
	int queue_capacity_;// actually the whole queue has queue_capacity_+1(head)+1(tail) nodes.
	CircleNode<Q> * head_;// a head to make queue handling more convenient.
	CircleNode<Q> * front_;
	CircleNode<Q> * rear_;

	pthread_mutex_t lock_;
	sem_t full_,empty_;

};

struct BlockCircleNode {
	Block *node_;
	BlockCircleNode *next_;
};

//class BlockCircleQueue {
//public:
//	BlockCircleQueue(int capacity):queue_capacity_(capacity){
//		pthread_mutex_init(&lock_,0);
//		/* head node can be convenient for handle */
//		head_=(BlockCircleNode *)malloc(sizeof(BlockCircleNode));
//		head_->node_=0;head_->next_=0;
//		BlockCircleNode *temp,*real_head=0;
//		BlockCircleNode *cur_tail=head_;
//		for(unsigned i=0;i<queue_capacity_+1;i++) {
//			/* insert the node from the tail, by using a temp */
//			temp=(BlockCircleNode*)malloc(sizeof(BlockCircleNode));
//			temp->node_=0;temp->next_=0;
//			temp->node_=new Block(BLOCK_SIZE);
//			if(i==0){
//				front_=rear_=real_head=temp;
//				cur_tail->next_=temp;
//				cur_tail=cur_tail->next_;
//			}
//			else if(i==queue_capacity_){
//				temp->next_=real_head;
//				cur_tail->next_=temp;
//				cur_tail=cur_tail->next_;//no need
//			}
//			else{
//				cur_tail->next_=temp;
//				cur_tail=cur_tail->next_;
//			}
//		}
//	};
//
//	virtual ~BlockCircleQueue(){
//		/* free the queue memory space. */
//		BlockCircleNode *p,*q=head_;
//		for(unsigned i=0;i<=queue_capacity_+1;i++) {
//			p=q;
//			q=p->next_;
//			p->node_->~Buffer();
//			free(p);
//		}
//		pthread_mutex_destroy(&lock_);
//	};
//
//	bool push(Block *node) {
//		/* if the queue is full, return false. eg:
//		 * 0(head)+9(tail)=full, "(9-0+10)%(10)==9" is full like follows.
//		 * */
//		pthread_mutex_lock(&lock_);
//		if(rear_->next_==front_){
//			pthread_mutex_unlock(&lock_);
//			return false;
//		}
//		rear_->node_->storeBlock(node->getAddr(), BLOCK_SIZE);
//		rear_=rear_->next_;
//		pthread_mutex_unlock(&lock_);
//		return true;
//	}
//
//	Block* pop() {
//		/* if the queue is empty, return 0 */
//		pthread_mutex_lock(&lock_);
//		if(rear_==front_) {
//			pthread_mutex_unlock(&lock_);
//			return 0;
//		}
//		Block *ret=front_->node_;
//		front_=front_->next_;
//		pthread_mutex_unlock(&lock_);
//		return ret;
//	}
//private:
//	int queue_capacity_;// actually the whole queue has queue_capacity_+1(head)+1(tail) nodes.
//	BlockCircleNode * head_;// a head to make queue handling more convenient.
//	BlockCircleNode * front_;
//	BlockCircleNode * rear_;
//
//	pthread_mutex_t lock_;
//};

class BlockCircleQueue {
public:
	BlockCircleQueue(int capacity):queue_capacity_(capacity){
		pthread_mutex_init(&lock_,0);
		/* head node can be convenient for handle */
		for(int i=0; i<queue_capacity_; i++) {
			Block *bk=new Block(BLOCK_SIZE);
			emptys_.push_back(bk);
		}
	};

	virtual ~BlockCircleQueue(){
		/* free the queue memory space. */
		for(unsigned i=0;i<=queue_capacity_;i++) {
//			Block *bk=emptys_;
		}
		pthread_mutex_destroy(&lock_);
	};

	bool push(Block *node) {
		/* if the queue is full, return false. eg:
		 * 0(head)+9(tail)=full, "(9-0+10)%(10)==9" is full like follows.
		 * */
		pthread_mutex_lock(&lock_);
		if(emptys_.empty()){
			pthread_mutex_unlock(&lock_);
			return false;
		}
		Block *bk=emptys_.front();
		bk->storeBlock(node->getAddr(), BLOCK_SIZE);
		blocks_.push_back(bk);
		emptys_.pop_front();
		pthread_mutex_unlock(&lock_);
		return true;
	}

	bool pop(Block *&block) {
		/* if the queue is empty, return 0 */
		pthread_mutex_lock(&lock_);
		if(blocks_.empty()) {
			pthread_mutex_unlock(&lock_);
			return false;
		}
		Block *bk=blocks_.front();
		block->storeBlock(bk->getAddr(), BLOCK_SIZE);
		emptys_.push_back(bk);
		blocks_.pop_front();
		pthread_mutex_unlock(&lock_);
		return true;
	}
private:
	int queue_capacity_;// actually the whole queue has queue_capacity_+1(head)+1(tail) nodes.
	list<Block *> emptys_;
	list<Block *> blocks_;

	pthread_mutex_t lock_;
};

#endif /* M_CIRCLE_QUEUE_H_ */
