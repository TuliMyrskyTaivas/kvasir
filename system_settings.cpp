//////////////////////////////////////////////////////////////////////////
/// file: system_settings.cpp
///
/// summary: reading and writing of system settings
//////////////////////////////////////////////////////////////////////////

#include "system_settings.h"
#include "scanner.h"

namespace kvasir
{

//////////////////////////////////////////////////////////////////////////
void SystemSettings::Load(const Scanner& scanner)	
{
	scanner.EnterProgrammingMode();

	m_backlight = GetBacklightSettings(scanner);
	m_battery = GetBatterySettings(scanner);
	m_keySettings = GetKeySettings(scanner);
	m_openingMessage = GetOpeningMessage(scanner);

	scanner.ExitProgrammingMode();
}

//////////////////////////////////////////////////////////////////////////
void SystemSettings::Save(const Scanner&) const
{
	throw std::logic_error("not implemented");
}

//////////////////////////////////////////////////////////////////////////
BacklightData SystemSettings::GetBacklightSettings(const Scanner& scanner) const
{	
	const auto response = scanner.IssueCommand("BLT\r", 3);
	return BacklightData{
		response[0],
		response[1],
		std::stoi(response[2])
	};
}

//////////////////////////////////////////////////////////////////////////
BatteryData SystemSettings::GetBatterySettings(const Scanner& scanner) const
{	
	const auto response = scanner.IssueCommand("BSV\r", 2);
	return BatteryData{
		static_cast<bool>(std::stoi(response[0])),
		std::stoi(response[1])
	};
}

//////////////////////////////////////////////////////////////////////////
KeySettingsData SystemSettings::GetKeySettings(const Scanner& scanner) const
{
	
	const auto response = scanner.IssueCommand("KBP\r", 3);
	return KeySettingsData{
		std::stoi(response[0]),
		static_cast<bool>(std::stoi(response[1])),
		static_cast<bool>(std::stoi(response[2]))
	};
}

//////////////////////////////////////////////////////////////////////////
OpeningMessageData SystemSettings::GetOpeningMessage(const Scanner& scanner) const
{	
	const auto response = scanner.IssueCommand("OMS\r", 4);
	return OpeningMessageData{
		response[0],
		response[1],
		response[2],
		response[3]
	};
}

//////////////////////////////////////////////////////////////////////////

} // namespace kvasir