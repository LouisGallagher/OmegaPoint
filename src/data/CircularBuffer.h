#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

#include <boost/thread/mutex.hpp>
#include <boost/thread/lockable_concepts.hpp> 
#include <boost/circular_buffer.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>
#include <boost/call_traits.hpp>
#include <boost/bind.hpp>

class CircularBuffer
{
	public:
		CircularBuffer(int capacity = 100)
		:container(capacity)
		{

		}
		virtual ~CircularBuffer(){}

		void push(uint8_t * rgb, uint8_t * depth, int64_t timestamp)
		{
			boost::mutex::scoped_lock lock(mutex);
			not_full.wait(lock, boost::bind(&CircularBuffer::is_not_full, this));
			container.push_front({{depth, rgb}, timestamp});
			lock.unlock();	
			not_empty.notify_one();
		}
		
		void pop(uint8_t ** rgb, uint8_t ** depth, int64_t & timestamp)
		{
			boost::mutex::scoped_lock lock(mutex);
			not_empty.wait(lock, boost::bind(&CircularBuffer::is_not_empty, this));
			std::pair<std::pair<uint8_t *, uint8_t *>, int64_t> & item = container.back();
			*depth = item.first.first;
			*rgb = item.first.second;
			timestamp = item.second;
			container.pop_back();
			lock.unlock();
			not_full.notify_one();
		}

		bool empty()
		{
			return container.size() == 0;
		}

	private:
		bool is_not_empty(){ return container.size() > 0; }
		bool is_not_full(){ return container.size() < container.capacity(); }

		boost::mutex mutex;
		boost::condition  not_empty;
		boost::condition not_full;

		boost::circular_buffer_space_optimized<std::pair<std::pair<uint8_t *, uint8_t *>, int64_t>> container;
};

#endif //CIRCULARBUFFER_H_