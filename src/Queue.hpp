#pragma once

#include <atomic> // std::atomic
#include <array>  // std::array
#include <string> // std::string
#include <>

#include "Debugger.hpp"
#include "Guard.hpp"

namespace Queue
{

#pragma region CONSTANTS

constexpr size_t DEF_QUEUE_SIZE = 100;

#pragma endregion

template<typename T, size_t SIZE = DEF_QUEUE_SIZE>
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

	constexpr size_t size() const noexcept { return SIZE; }

	HASH_GUARD(inline void rehash() { hash_ = std::move(makeHash()); })

	HASH_GUARD(inline std::string_view getHash() const { return hash_; })

	void swap(Queue&) noexcept(std::_Is_nothrow_swappable<T>::value);

	bool ok()                            const noexcept;
	void dump(std::istream& = std::cout) const;

private:
	std::atomic<size_t>              first_,
			                         second_;

	std::atomic<std::array<T, SIZE>> buffer_;
};

//====================================================================================================================================
//=========================================================METHOD_DEFINITION==========================================================
//====================================================================================================================================

#pragma region METHOD_DEFINITION

template<typename T, size_t SIZE>
inline Storage<T, SIZE>::Storage() _NOEXCEPT :
CANARY_GUARD(CANARY_VALUE(NHash::Hash("Storage" + ++numberOfInstances).getHash()), )
CANARY_GUARD(canaryStart_(CANARY_VALUE), )
HASH_GUARD(hash_(), )

buf_()

CANARY_GUARD(, canaryFinish_(CANARY_VALUE))
{
	HASH_GUARD(hash_ = makeHash();)
		CANARY_GUARD(numberOfInstances--;)
		numberOfInstances++;

	GUARD_CHECK()
}

template<typename T, size_t SIZE>
inline Storage<T, SIZE>::Storage(const Storage &crStorage) _NOEXCEPT :
CANARY_GUARD(CANARY_VALUE(NHash::Hash("Storage" + ++numberOfInstances).getHash()), )
CANARY_GUARD(canaryStart_(CANARY_VALUE), )
HASH_GUARD(hash_(crStorage.hash_), )

buf_(crStorage.buf_)

CANARY_GUARD(, canaryFinish_(CANARY_VALUE))
{
	CANARY_GUARD(numberOfInstances--;)
		numberOfInstances++;

	GUARD_CHECK()
}

template<typename T, size_t SIZE>
inline Storage<T, SIZE>::Storage(Storage &&rrStorage) noexcept :
CANARY_GUARD(CANARY_VALUE(NHash::Hash("Storage" + ++numberOfInstances).getHash()), )
CANARY_GUARD(canaryStart_(CANARY_VALUE), )
HASH_GUARD(hash_(std::move(rrStorage.hash_)), )

buf_(std::move(rrStorage.buf_))

CANARY_GUARD(, canaryFinish_(CANARY_VALUE))
{
	CANARY_GUARD(numberOfInstances--;)
		numberOfInstances++;

	rrStorage.buf_.fill(NULL);
	HASH_GUARD(rrStorage.hash_.clear();)

		GUARD_CHECK()
}

template<typename T, size_t SIZE>
inline Storage<T, SIZE>::~Storage()
{
	numberOfInstances--;

	GUARD_CHECK()
}

template<typename T, size_t SIZE>
inline Storage<T, SIZE> &Storage<T, SIZE>::operator=(const Storage &crStorage) noexcept
{
	GUARD_CHECK()

		if (this != &crStorage)
		{
			buf_ = crStorage.buf_;

			HASH_GUARD(rehash();)
		}

	GUARD_CHECK()

		return (*this);
}

template<typename T, size_t SIZE>
inline Storage<T, SIZE> &Storage<T, SIZE>::operator=(Storage &&rrStorage) noexcept
{
	GUARD_CHECK()

		assert(this != &rrStorage);

	buf_ = std::move(rrStorage.buf_);
	HASH_GUARD(hash_ = std::move(rrStorage.hash_);)

		rrStorage.buf_.fill(NULL);
	HASH_GUARD(rrStorage.hash_.clear();)

		GUARD_CHECK()

		return (*this);
}

template<typename T, size_t SIZE>
inline typename Storage<T, SIZE>::rVal_ Storage<T, SIZE>::operator[](size_t index)
{
	GUARD_CHECK()

		if (index >= SIZE) throw std::out_of_range(std::string("[") + __FUNCTION__ + "] Storage out of range\n");

	GUARD_CHECK()

		return buf_[index];
}

template<typename T, size_t SIZE>
inline typename Storage<T, SIZE>::crVal_ Storage<T, SIZE>::operator[](size_t index) const
{
	GUARD_CHECK()

		if (index >= SIZE) throw std::out_of_range(std::string("[") + __FUNCTION__ + "] Storage out of range\n");

	GUARD_CHECK()

		return buf_[index];
}

template<typename T, size_t SIZE>
inline void Storage<T, SIZE>::swap(Storage &rStorage) noexcept(std::_Is_nothrow_swappable<T>::value)
{
	GUARD_CHECK()

		using std::swap; // To have all possible swaps

	buf_.swap(rStorage.buf_);
	HASH_GUARD(swap(hash_, rStorage.hash_);)

		GUARD_CHECK()
}

template<typename T, size_t SIZE>
inline bool Storage<T, SIZE>::ok() const _NOEXCEPT
{
	return (CANARY_GUARD(canaryStart_ == CANARY_VALUE && canaryFinish_ == CANARY_VALUE && )
		HASH_GUARD(hash_ == makeHash() && )
		&buf_);
}

template<typename T, size_t SIZE>
void Storage<T, SIZE>::dump(std::ostream &rOstr /* = std::cout */) const
{
	rOstr << "[STORAGE DUMP]\n"
		<< "Storage <" << typeid(T).name() << ", " << SIZE << "> [0x" << this << "]\n{\n"
		<< "\tbuffer [" << SIZE << "] = 0x" << &buf_ << "\n\t{\n";

	for (size_t i = 0; i < SIZE; ++i) rOstr << "\t\t[" << std::setw(3) << i << "] =" << std::setw(8) << buf_[i] << std::endl;

	rOstr << "\n\t}\n\n";

	CANARY_GUARD
	(
		rOstr << "\tCANARY_VALUE  = " << CANARY_VALUE << std::endl;

	rOstr << "\tCANARY_START  = " << canaryStart_;
	if (canaryStart_ == CANARY_VALUE) NDebugger::Info(" TRUE", NDebugger::TextColor::Green, true, rOstr);
	else                              NDebugger::Info(" FALSE", NDebugger::TextColor::Red, true, rOstr);

	rOstr << "\tCANARY_FINISH = " << canaryFinish_;
	if (canaryFinish_ == CANARY_VALUE) NDebugger::Info(" TRUE", NDebugger::TextColor::Green, true, rOstr);
	else                               NDebugger::Info(" FALSE", NDebugger::TextColor::Red, true, rOstr);
	)

		HASH_GUARD
		(
			rOstr << "\n\tHASH = " << hash_;
	if (hash_ == makeHash()) NDebugger::Info(" TRUE", NDebugger::TextColor::Green, true, rOstr);
	else                     NDebugger::Info(" FALSE", NDebugger::TextColor::Red, true, rOstr);
	)

		rOstr << "}\n[     END     ]\n\n";
}


#pragma endregion

} // namespace NQueue