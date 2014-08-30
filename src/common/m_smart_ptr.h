/*
 * m_smart_ptr.h
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#ifndef M_SMART_PTR_H_
#define M_SMART_PTR_H_

#include <iostream>
using namespace std;

template <class Obj>
class Counter {
	template <class T>
	friend class SmartPtr;

	Obj *ptr;
	int count;

	Counter(Obj *t):ptr(t){
		cout<<"constructor a object."<<endl;
		count=1;
	};
	virtual ~Counter(){
		cout<<"de constructor a object."<<endl;
		delete ptr;
	};
};

template <class T>
class SmartPtr {
public:
	SmartPtr(T *t):ptr(new Counter<T>(t)){
		cout<<"constructor!"<<"use = "<<ptr->count<<endl;
	};
	SmartPtr(const SmartPtr<T>& p):ptr(p.ptr){
		++ptr->count;
		cout<<"copy constructor! "<<"use = "<<p.ptr->count<<endl;
	};
	SmartPtr<T>& operator = (const SmartPtr<T> &t_ptr){
		cout<<"value!"<<endl;
		++t_ptr.ptr->count;
		if(--ptr->count==0)
			delete ptr;
		ptr=t_ptr.ptr;
		return *this;
	}
	virtual ~SmartPtr(){
		cout<<"deconstructor a object!"<<"use = "<<ptr->count<<endl;
		if(--ptr->count==0)
			delete ptr;
	};
	void Pripoui(){
		cout<<"jellp;hello"<<endl;
	};

private:
	Counter<T> *ptr;
};

#endif /* M_SMART_PTR_H_ */
