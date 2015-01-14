/*
 * m_task.h
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#ifndef M_TASK_H_
#define M_TASK_H_

#include "m_message.h"
#include "m_serialization.h"
#include "../physical/m_query_plan.h"
#include "../physical/m_scan.h"
using namespace physical;

#include <iostream>
using namespace std;

#include <boost/iostreams/stream.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/iostreams/device/back_inserter.hpp>

typedef void (*Func)(void *args);

template<typename T>
static Message1 Serialize(T object) {
	std::string serial_str;
	boost::iostreams::back_insert_device<std::string> inserter(serial_str);
	boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > ostr(inserter);
	boost::archive::binary_oarchive oa(ostr);
	oa<<object;
	ostr.flush();
	Message1 ret;
	assert(serial_str.size()<=Message1::Capacity());
	memcpy(ret.message,serial_str.data(),serial_str.size());
	ret.length=serial_str.size();
	return ret;
}

template<typename T>
static T Deserialize(Message1 output) {
	std::string received(output.message,output.length);

	boost::iostreams::basic_array_source<char> device(received.data(),received.size());
	boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s(device);
	boost::archive::binary_iarchive ia(s);

	T ret;
	ia>>ret;
	return ret;
}

class Task {
public:
	Task(Func func,void *args):function_(func),args_(args) {};
	virtual ~Task() {};

	void run() {
		function_(args_);
	}

private:
	void (*function_)(void *args);
	void *args_;
};

class TaskInfo {
public:
	TaskInfo(QueryPlan *qp):ser_qp_(qp) {};
	TaskInfo(const TaskInfo& r){
		ser_qp_=r.ser_qp_;
	}

	TaskInfo():ser_qp_(0){};
	~TaskInfo() {};

	inline void run() {
		ser_qp_->prelude();
		ser_qp_->execute(0);
		ser_qp_->postlude();
	}

	/* serialize will return a message which has the operator object. */
	static Message1 serialize(TaskInfo input) {
		return Serialize<TaskInfo>(input);
	}

	static TaskInfo deserialize(Message1 output) {
		return Deserialize<TaskInfo>(output);
	}

private:
	QueryPlan *ser_qp_;
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		/* every nest loop relation must be registered. */
		register_data(ar);
		register_obj(ar);
		ar  & ser_qp_;
	}
};

#endif /* M_TASK_H_ */
