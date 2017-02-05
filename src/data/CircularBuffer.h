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

		//TODO should be timed wait
		void push(std::unique_ptr<unsigned char> rgb, std::unique_ptr<unsigned short> depth, int64_t timestamp)
		{
			boost::mutex::scoped_lock lock(mutex);
			not_full.wait(lock, boost::bind(&CircularBuffer::is_not_full, this));
			//container.push_front({std::make_pair(std::move(depth), std::move(rgb)), timestamp});
			container.push_front(Frame(std::move(depth), std::move(rgb), timestamp));
			lock.unlock();	
			not_empty.notify_one();
		}
		
		//TODO should be timed wait
		void pop(std::shared_ptr<unsigned char> & rgb, std::shared_ptr<unsigned short> & depth, int64_t & timestamp)
		{
			boost::mutex::scoped_lock lock(mutex);
			not_empty.wait(lock, boost::bind(&CircularBuffer::is_not_empty, this));
			/*std::pair<std::pair<std::unique_ptr<uint8_t>, std::unique_ptr<uint8_t>>, int64_t> & item = container.back();
			depth = std::move(item.first.first);
			rgb = std::move(item.first.second);
			depth.reset((unsigned short *)item.first.first.release());
			rgb.reset((unsigned char *)item.first.second.release());
			timestamp = item.second;*/

			Frame & frame = container.back();
			depth = frame.depth;
			rgb = frame.rgb;
			timestamp = frame.timestamp;
			container.pop_back();
			lock.unlock();
			not_full.notify_one();
		}

		bool empty()
		{
			return container.size() == 0;
		}

	private:
		struct Frame
		{
			std::shared_ptr<unsigned short> depth;
			std::shared_ptr<unsigned char> rgb;
			int64_t timestamp;

			Frame(std::unique_ptr<unsigned short> && depth, std::unique_ptr<unsigned char> && rgb, int64_t timestamp) noexcept
			:depth(std::move(depth)),
			 rgb(std::move(rgb)),
			 timestamp(timestamp)
			{}

			Frame(const Frame & other) noexcept
			:depth(other.depth),
			 rgb(other.rgb),
			 timestamp(other.timestamp)
			{}
		};

		bool is_not_empty(){ return container.size() > 0; }
		bool is_not_full(){ return container.size() < container.capacity(); }

		boost::mutex mutex;
		boost::condition  not_empty;
		boost::condition not_full;

		/*using ImagePair = std::pair<std::unique_ptr<uint8_t>, std::unique_ptr<uint8_t>>;
		using Buffer = boost::circular_buffer_space_optimized<std::pair<ImagePair, int64_t>>;
		Buffer container;*/

		using Buffer = boost::circular_buffer_space_optimized<Frame>;
		Buffer container;
};
#endif //CIRCULARBUFFER_H_