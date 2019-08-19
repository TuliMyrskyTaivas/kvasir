//////////////////////////////////////////////////////////////////////////
/// file: system.cpp
///
/// summary: scan system 
//////////////////////////////////////////////////////////////////////////

#include "system.h"
#include "uniden.h"
#include "group.h"

namespace kvasir
{

template class System<ConventionalChannel>;
template class System<TrunkChannel>;

//////////////////////////////////////////////////////////////////////////
template<>
System<ConventionalChannel>::System(const int index, const std::vector<std::string>& sinInfo)
	: m_index(index)
	, m_name(sinInfo[Offset(SIN::Name)])
	, m_sequenceNumber(std::stoi(sinInfo[Offset(SIN::SeqNumber)]))
{}

//////////////////////////////////////////////////////////////////////////
template<>
System<TrunkChannel>::System(const int index, const std::vector<std::string>& sinInfo)
	: m_index(index)
	, m_name(sinInfo[Offset(SIN::Name)])
	, m_sequenceNumber(std::stoi(sinInfo[Offset(SIN::SeqNumber)]))
{}

} // namespace kvasir