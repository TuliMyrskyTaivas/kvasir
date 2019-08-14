//////////////////////////////////////////////////////////////////////////
/// file: system_settings.h
///
/// summary: reading and writing of system settings
//////////////////////////////////////////////////////////////////////////

#ifndef KVASIR_SYSTEM_SETTINGS_H_INCLUDED
#define KVASIR_SYSTEM_SETTINGS_H_INCLUDED

#include <array>
#include <string>

namespace kvasir
{

//////////////////////////////////////////////////////////////////////////
struct BacklightData
{
	std::string event;                      // Event for activation of backlight
	std::string color;                      // Backlight color
	int dimmer = -1;                        // Backlight dimmer (1:Low, 2:Middle, 3:High)
};

//////////////////////////////////////////////////////////////////////////
struct BatteryData
{
	bool batterySave = false;
	int chargeTime = -1;
};

//////////////////////////////////////////////////////////////////////////
struct KeySettingsData
{
	int beepLevel = 1;                      // Beep level (0:Auto, 1-15: volume, 99:off)
	bool keyLock = false;                   // Key Lock status
	bool keySafe = false;                   // Key Safe status
};

struct AutoGainControlData
{
	int analogResponseTime;                 // -4 : +6
	int analogReferenceGain;                // -5 : +5
	int analogGainRange;                    //  0 : 15
	int digitalResponseTime;                // -8 : +8
	int gititalReferenceGain;               // -5 : +5
};

//////////////////////////////////////////////////////////////////////////
using OpeningMessageData = std::array<std::string, 4>;

//////////////////////////////////////////////////////////////////////////
class Scanner;

//////////////////////////////////////////////////////////////////////////
class SystemSettings
{	
	BacklightData m_backlight;
	BatteryData m_battery;
	KeySettingsData m_keySettings;
	OpeningMessageData m_openingMessage;
	AutoGainControlData m_autoGainControl;

	BacklightData GetBacklightSettings(const Scanner&) const;
	BatteryData GetBatterySettings(const Scanner&) const;
	KeySettingsData GetKeySettings(const Scanner&) const;
	OpeningMessageData GetOpeningMessage(const Scanner&) const;
	AutoGainControlData GetAutoGainControl(const Scanner&) const;

public:	
	~SystemSettings() = default;

	void Import(const std::string& path);
	void Export(const std::string& path) const;
	
	void Load(const Scanner& scanner);
	void Save(const Scanner& scanner) const;

	BacklightData& Backlight() noexcept
	{
		return m_backlight;
	}

	const BacklightData& Backlight() const noexcept
	{
		return m_backlight;
	}

	BatteryData& Battery() noexcept
	{
		return m_battery;
	}

	const BatteryData& Battery() const noexcept
	{
		return m_battery;
	}

	KeySettingsData& KeySettings() noexcept
	{
		return m_keySettings;
	}

	const KeySettingsData& KeySettings() const noexcept
	{
		return m_keySettings;
	}

	OpeningMessageData& OpeningMessage() noexcept
	{
		return m_openingMessage;
	}

	const OpeningMessageData& OpeningMessage() const noexcept
	{
		return m_openingMessage;
	}
};

} // namespace kvasir

#endif // KVASIR_SYSTEM_SETTINGS_H_INCLUDED