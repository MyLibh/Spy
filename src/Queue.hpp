#pragma once

#include <atomic>  // std::atomic
#include <array>   // std::array
#include <string>  // std::string
#include <cassert> // assert
#include <iomanip> // std::setw

#include "Debugger.hpp"
#include "Guard.hpp"

namespace NQueue
{

#pragma region CONSTANTS

constexpr size_t DEF_QUEUE_SIZE = 100;

#pragma endregion

template<typename T = int, size_t SIZE = DEF_QUEUE_SIZE>
class Queue final
{
public:
	typedef       T  &_rVal;
	typedef       T &&_rrVal;
	typedef const T  &_crVal;

	explicit Queue()    noexcept;
	Queue(const Queue&) noexcept;
	Queue(Queue&&)      noexcept;
	~Queue();

	Queue &operator=(const Queue&) noexcept;
	Queue &operator=(Queue&&)      noexcept;

	void push(_crVal);
	void push(_rrVal);
	void pop();

	_crVal front() const noexcept;
	_crVal back()  const noexcept;

	bool empty() const noexcept;

	constexpr size_t size() const noexcept { return SIZE; }

	HASH_GUARD(inline void rehash() { hash_ = std::move(makeHash()); })

	HASH_GUARD(inline std::string_view getHash() const { return hash_; })

	void swap(Queue&) noexcept(std::_Is_nothrow_swappable<T>::value);

	bool ok()                            const noexcept;
	void dump(std::ostream& = std::cout) const;

private:
	CANARY_GUARD(const std::string CANARY_VALUE;)
	CANARY_GUARD(std::string canaryStart_;)
	HASH_GUARD(std::string hash_;)

	std::atomic<size_t>  first_,
			             second_;

	std::array<T, SIZE>  buffer_;

	CANARY_GUARD(std::string canaryFinish_;)

	HASH_GUARD
	( 
		std::string makeHash() const
		{
			std::string tmp;
			tmp += std::to_string(first_);
			tmp += std::to_string(second_);
			for (auto &&x : buffer_) tmp += std::to_string(x);

			return std::string(NHash::Hash(tmp).getHash());
		}
	)

	static size_t numberOfInstances;
};

//====================================================================================================================================
//==========================================================STATIC_VARIABLES==========================================================
//====================================================================================================================================

template<typename T, size_t SIZE>
size_t Queue<T, SIZE>::numberOfInstances = 0;

//====================================================================================================================================
//=========================================================METHOD_DEFINITION==========================================================
//====================================================================================================================================

#pragma region METHOD_DEFINITION

template<typename T, size_t SIZE>
inline Queue<T, SIZE>::Queue() noexcept :
	CANARY_GUARD(CANARY_VALUE(NHash::Hash("Queue" + ++numberOfInstances).getHash()), )
	CANARY_GUARD(canaryStart_(CANARY_VALUE), )
	HASH_GUARD(hash_(), )

	first_(),
	second_(),
	buffer_()

	CANARY_GUARD(, canaryFinish_(CANARY_VALUE))
{
HASH_GUARD(hash_ = makeHash();)
CANARY_GUARD(numberOfInstances--;)

	numberOfInstances++;

GUARD_CHECK()
}

template<typename T, size_t SIZE>
inline Queue<T, SIZE>::Queue(const Queue &crQueue) noexcept :
	CANARY_GUARD(CANARY_VALUE(NHash::Hash("Queue" + ++numberOfInstances).getHash()), )
	CANARY_GUARD(canaryStart_(CANARY_VALUE), )
	HASH_GUARD(hash_(crStorage.hash_), )

	first_(crQueue.first_),
	second_(crQueue.second_),
	buffer_(crQueue.buffer_)

	CANARY_GUARD(, canaryFinish_(CANARY_VALUE))
{
CANARY_GUARD(numberOfInstances--;)

	numberOfInstances++;

GUARD_CHECK()
}

template<typename T, size_t SIZE>
inline Queue<T, SIZE>::Queue(Queue &&rrQueue) noexcept :
	CANARY_GUARD(CANARY_VALUE(NHash::Hash("Storage" + ++numberOfInstances).getHash()), )
	CANARY_GUARD(canaryStart_(CANARY_VALUE), )
	HASH_GUARD(hash_(std::move(rrStorage.hash_)), )

	first_(std::move(rrQueue.first_)),
	second_(std::move(rrQueue.second_)),
	buffer_(std::move(rrQueue.buffer_))

	CANARY_GUARD(, canaryFinish_(CANARY_VALUE))
{
CANARY_GUARD(numberOfInstances--;)

	numberOfInstances++;

	rrStorage.buf_.fill(NULL);

HASH_GUARD(rrStorage.hash_.clear();)

GUARD_CHECK()
}

template<typename T, size_t SIZE>
inline Queue<T, SIZE>::~Queue()
{
	numberOfInstances--;

	GUARD_CHECK()
}

template<typename T, size_t SIZE>
inline Queue<T, SIZE> &Queue<T, SIZE>::operator=(const Queue &crQueue) noexcept
{
GUARD_CHECK()

	if (this != &crQueue)
	{
		first_  = crQueue.first_;
		second_ = crQueue.second_;
		buffer_ = crQueue.buffer_;

	HASH_GUARD(rehash();)
	}

GUARD_CHECK()

	return (*this);
}

template<typename T, size_t SIZE>
inline Queue<T, SIZE> &Queue<T, SIZE>::operator=(Queue &&rrQueue) noexcept
{
GUARD_CHECK()

	assert(this != &rrQueue);

HASH_GUARD(hash_ = std::move(rrQueue.hash_);)

	first_  = std::move(rrQueue.first_);
	second_ = std::move(rrQueue.second_);
	buffer_ = std::move(rrQueue.buffer_);
	
HASH_GUARD(rrQueue.hash_.clear();)

	rrQueue.first_  = NULL;
	rrQueue.second_ = NULL;
	rrQueue.buffer_.fill(NULL);

GUARD_CHECK()

	return (*this);
}

template<typename T, size_t SIZE>
inline void Queue<T, SIZE>::push(_crVal val)
{
GUARD_CHECK()

	buffer_[first_] = val;

	first_ = ++first_ % SIZE;

HASH_GUARD(rehash();)

GUARD_CHECK()
}

template<typename T, size_t SIZE>
inline void Queue<T, SIZE>::push(_rrVal val)
{
GUARD_CHECK()

	buffer_[first_] = val;

	first_ = ++first_ % SIZE;

HASH_GUARD(rehash();)

GUARD_CHECK()
}

template<typename T, size_t SIZE>
inline void Queue<T, SIZE>::pop()
{
GUARD_CHECK()

	second_ = ++second_ % SIZE;

HASH_GUARD(rehash();)

GUARD_CHECK()
}

template<typename T, size_t SIZE>
inline typename Queue<T, SIZE>::_crVal Queue<T, SIZE>::front() const noexcept
{
	return buffer_[second_];
}

template<typename T, size_t SIZE>
inline typename Queue<T, SIZE>::_crVal Queue<T, SIZE>::back() const noexcept
{
	return buffer_[first_];
}

template<typename T, size_t SIZE>
inline bool Queue<T, SIZE>::empty() const noexcept
{
	return (first_ == second_);
}

template<typename T, size_t SIZE>
inline void Queue<T, SIZE>::swap(Queue &rQueue) noexcept(std::_Is_nothrow_swappable<T>::value)
{
GUARD_CHECK()

	using std::swap; // To have all possible swaps
	
	swap(first_, rQueue.first_);
	swap(second_, rQueue.second_);
	buf_.swap(rStorage.buf_);

HASH_GUARD(swap(hash_, rStorage.hash_);)

GUARD_CHECK()
}

template<typename T, size_t SIZE>
inline bool Queue<T, SIZE>::ok() const noexcept
{
	return (CANARY_GUARD(canaryStart_ == CANARY_VALUE && canaryFinish_ == CANARY_VALUE && )
		HASH_GUARD(hash_ == makeHash() && )
		&buffer_ && (first_ <= SIZE)  && (second_ <= SIZE));
}

template<typename T, size_t SIZE>
void Queue<T, SIZE>::dump(std::ostream &rOstr /* = std::cout */) const
{
	NDebugger::Info("\t[QUEUE DUMP]", NDebugger::TextColor::LightGreen, true, rOstr);

	rOstr << "Queue <" << typeid(T).name() << ", " << SIZE << "> [0x" << this << "]\n{\n"
		  << "\tbuffer [" << SIZE << "] = 0x" << &buffer_ << "\n\t{\n";

	if (!empty())
	{
		for (size_t i = second_; ; i = ++i % SIZE)
		{
			rOstr << "\t\t[" << std::setw(3) << i << "] =" << std::setw(8) << buffer_[i] << std::endl;

			if (i == first_) break;
		}
	}
	else rOstr << "\t\tempty";

	rOstr << "\n\t}\n\n";

	std::cout << "\tfirst  = " << first_  << "\n"
		      << "\tsecond = " << second_ << "\n\n";

	CANARY_GUARD
	(
		rOstr << "\tCANARY_VALUE  = " << CANARY_VALUE << std::endl;

		rOstr << "\tCANARY_START  = " << canaryStart_;
		if (canaryStart_ == CANARY_VALUE) NDebugger::Info(" TRUE",  NDebugger::TextColor::Green, true, rOstr);
		else                              NDebugger::Info(" FALSE", NDebugger::TextColor::Red,   true, rOstr);

		rOstr << "\tCANARY_FINISH = " << canaryFinish_;
		if (canaryFinish_ == CANARY_VALUE) NDebugger::Info(" TRUE",  NDebugger::TextColor::Green, true, rOstr);
		else                               NDebugger::Info(" FALSE", NDebugger::TextColor::Red,   true, rOstr);
		)

		HASH_GUARD
		(
			rOstr << "\n\tHASH = " << hash_;
			if (hash_ == makeHash()) NDebugger::Info(" TRUE",  NDebugger::TextColor::Green, true, rOstr);
			else                     NDebugger::Info(" FALSE", NDebugger::TextColor::Red,   true, rOstr);
		)

		rOstr << "}\n";

		NDebugger::Info("\t[   END    ]\n", NDebugger::TextColor::LightGreen, true, rOstr);
}

#pragma endregion

} // namespace NQueue