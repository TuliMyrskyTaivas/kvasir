//////////////////////////////////////////////////////////////////////////
/// file: system.h
///
/// summary: scan system 
//////////////////////////////////////////////////////////////////////////

#ifndef KVASIR_SYSTEM_H_INCLUDED
#define KVASIR_SYSTEM_H_INCLUDED

#include <optional>
#include <string>
#include <vector>
#include <list>

namespace kvasir
{

class Scanner;
class ConventionalChannel;
class TrunkChannel;

enum class SystemType
{
	Conventional,
	Motorola,
	EDACS,
	EDACS_SCAT,
	LTR,
	P25Standard,
	P25OneFrequency
};

// Forward declaration of Group template
template<typename Type> class Group;

// System template declaration
template<typename Type>
class System
{	
	friend class ScanSettings;

	const int m_index;
	std::string m_name;
	int m_sequenceNumber;	
	bool m_protected;
	bool m_locked;
	std::optional<int> m_holdTime;
	std::optional<int> m_quickKey;
	std::optional<int> m_startKey;	
	std::optional<int> m_delayTime;
	std::optional<int> m_numberTag;
	std::optional<int> m_agcAnalog;
	std::optional<int> m_agcDigital;
	std::list<Group<Type>> m_groups;

	explicit System(const int index, const std::string& name)
		: m_index(index)
		, m_name(name)
	{}

	explicit System(const int index, const std::vector<std::string>& sinInfo);

public:	

	virtual ~System() = default;	

	const std::list<Group<Type>>& Groups() const noexcept
	{
		return m_groups;
	}

	std::list<Group<Type>>& Groups()
	{
		return m_groups;
	}

	const std::string& Name() const noexcept
	{
		return m_name;
	}

	std::string& Name()
	{
		return m_name;
	}

	const int SequenceNumber() const noexcept
	{
		return m_sequenceNumber;
	}

	int& SequenceNumber() noexcept
	{
		return m_sequenceNumber;
	}

	const bool Protected() const noexcept
	{
		return m_protected;
	}

	bool& Protected() noexcept
	{
		return m_protected;
	}
};

// Extern specification for two common cases
extern template class System<ConventionalChannel>;
extern template class System<TrunkChannel>;

} // namespace kvasir

#endif // KVASIR_SYSTEM_H_INCLUDED