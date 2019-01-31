#ifndef _PACKET_H_
#define _PACKET_H_

#include <MemeNet/Export.h>
#include <MemeCore/ITrackable.h>
#include <MemeCore/IComparable.h>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	enum Priority : uint8_t
	{
		Immediate,
		High,
		Medium,
		Low,

		MAX_PRIORITY
	};

	/* * * * * * * * * * * * * * * * * * * * */

	enum Reliability : uint8_t
	{
		Unreliable,
		UnreliableSequenced,
		Reliable,
		ReliableOrdered,
		ReliableSequenced,
		UnreliableWithAckReceipt,
		ReliableWithAckReceipt,
		ReliableOrderedWithAckReceipt,

		MAX_RELIABILITY
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_NETWORK_API SendSettings final
		: public ITrackable
	{
		Priority	priority;
		Reliability reliability;
		char		ordering;
		bool		broadcast;
		uint32_t	receiptNumber;

		SendSettings(const SendSettings & copy) : SendSettings(
			copy.priority,
			copy.reliability,
			copy.ordering, 
			copy.broadcast,
			copy.receiptNumber)
		{
		}
		
		SendSettings(Priority priority, Reliability reliability, char ordering, bool broadcast, uint32_t receiptNumber)
			: priority(priority)
			, reliability(reliability)
			, ordering(ordering)
			, broadcast(broadcast)
			, receiptNumber(receiptNumber)
		{
		}
		
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_NETWORK_API Address final
		: public ITrackable
		, public IComparable<Address>
	{
		String		addr;
		uint16_t	port;

		Address()
			: Address(String(), 0)
		{
		}
		
		Address(const String & addr)
			: Address(addr, NULL)
		{
		}
		
		Address(const String & addr, uint16_t port)
			: addr(addr)
			, port(port)
		{
		}
		
		Address(const Address & copy)
			: Address(copy.addr, copy.port)
		{
		}

		inline operator bool() const
		{
			return !addr.empty() && port;
		}

		inline void serialize(std::ostream & out) const override
		{
			out << addr;
		}
		
		inline bool equals(const Address & copy) const override
		{
			return
				addr == copy.addr &&
				port == copy.port;
		}
		
		inline bool lessThan(const Address & copy) const override
		{
			return
				addr < copy.addr &&
				port < copy.port;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_NETWORK_API GUID final
		: public ITrackable
		, public IComparable<uint64_t>
		, public IComparable<GUID>
	{
		uint64_t id;

		GUID()
			: GUID(0)
		{
		}
		
		GUID(uint64_t id)
			: id(id)
		{
		}
		
		GUID(const GUID & copy)
			: GUID(copy.id)
		{
		}
		
		inline operator uint64_t() const
		{
			return id;
		}

		inline void serialize(std::ostream & out) const override
		{
			out << id << " ";
		}
		
		inline bool equals(const uint64_t & value) const override
		{
			return (*this) == value;
		}
		
		inline bool equals(const GUID & value) const override
		{
			return id == value.id;
		}
		
		inline bool lessThan(const uint64_t & value) const override
		{
			return (*this) < value;
		}
		
		inline bool lessThan(const GUID & value) const override
		{
			return id < value.id;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_NETWORK_API Packet final
		: public ITrackable
	{
		Address		addr;
		GUID		guid;
		uint32_t	size;
		uint8_t *	data;

		Packet()
			: Packet(Address(), GUID(), 0, NULL)
		{
		}
		
		Packet(const Address & addr, const GUID & guid, uint32_t size, uint8_t * data)
			: addr(addr)
			, guid(guid)
			, size(size)
			, data(data)
		{
		}
		
		Packet(const Packet & copy)
			: Packet(copy.addr, copy.guid, copy.size, copy.data)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << addr << " " 
				<< guid << " " 
				<< size << " " 
				<< data << " ";
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PACKET_H_
