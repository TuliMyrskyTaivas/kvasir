//////////////////////////////////////////////////////////////////////////
/// file: scan_settings.cpp
///
/// summary: reading and writing of scan settings
//////////////////////////////////////////////////////////////////////////

#include "scan_settings.h"
#include "scanner.h"
#include "system.h"
#include "logger.h"
#include "group.h"

#include <cassert>
#include <typeinfo>

namespace kvasir
{

//////////////////////////////////////////////////////////////////////////
void ScanSettings::GetSystems(const Scanner& scanner)
{
	std::vector<UniversalSystem> newSystems;
	auto response = scanner.IssueCommand("SCT\r", 1);
	int systemCount = std::stoi(response.front());	

	if (!systemCount)
	{
		m_systems.clear();
		return;
	}
	
	// Reserve space, get head and tail indexes
	newSystems.reserve(systemCount);
	const std::string headIndex = scanner.IssueCommand("SIH\r", 1).front();
	const std::string tailIndex = scanner.IssueCommand("SIT\r", 1).front();

	Logger& log = Logger::GetInstance();
	log.Debug() << "start reading " << systemCount
		<< " from offset #" << headIndex;
		
	std::string index = headIndex;	
	while (systemCount--)
	{
		log.Debug() << "reading system " << index;
		response = scanner.IssueCommand("SIN, " + index + "\r", 28);
				
		const int idx = std::stoi(index);		
		// Create the system
		if ("CNV" == response[Offset(SIN::Type)])
		{
			newSystems.emplace_back(ConventionalSystem(idx, response));
		}
		else
		{
			newSystems.emplace_back(TrunkSystem(idx, response));
		}		

		// Move to the next system in chain
		index = response[12];
	}

	std::swap(m_systems, newSystems);
}

//////////////////////////////////////////////////////////////////////////
void ScanSettings::Load(const Scanner& scanner)
try
{
	scanner.EnterProgrammingMode();
	GetSystems(scanner);
	scanner.ExitProgrammingMode();
}
catch (const std::exception& e)
{
	Logger::GetInstance().Error() << "failed to load scan settings: " << e.what();
	scanner.ExitProgrammingMode();
	throw e;
}


} // namespace kvasir