//////////////////////////////////////////////////////////////////////////
/// file: scan_settings.h
///
/// summary: reading and writing of scan settings
//////////////////////////////////////////////////////////////////////////

#ifndef KVASIR_SCAN_SETTINGS_H_INCLUDED
#define KVASIR_SCAN_SETTINGS_H_INCLUDED

#include "uniden.h"
#include "system.h"

#include <memory>
#include <vector>
#include <variant>

namespace kvasir
{

/*
struct ChannelData
{
	int index;              // Channel index
	std::string name;       // Channel name (16 chars max)
	Modulation mod;         // Modulation
	bool attenuation;       // Attenuation (On/Off)
	CtcssDcsCode code;      // CTCSS/DCS status
	bool toneLockout;       // CTCSS/DCS tone lockout (On/Off)
	bool lockout;           // Channel lockout (Lockout/Unlocked)
	bool priority;          // Priority (On/Off)
	int alertTone;          // Alert tone (1 - 9, 0 means off)
	int alertToneLevel;     // Alert tone level (1 - 15, 0 means auto)
	int revIndex;           // Reverse channel index of the Channel Group
	int fwdIndex;           // Forward channel index of the Channel Group
	int sysIndex;           // System index of the Channel
	int grpIndex;           // Group index of the Channel
	int audioType;          // 0 - All, 1 - Analog only, 2 - Digital only
	int p25NAC;             // P25 NAC status (0 - 0xFFF, -1 means NAC search)
	int numberTag;          // Number tag (0 - 999, -1 means none)
	std::string alertColor; // Alert light color
	int alertLightPattern;  // Alert light pattern (0 - on, 1 - slow, 2 - fast)
	int volumeOffset;       // Volume offset (-3 - +3)
};

struct GroupData
{
	int  index;             // Group index
	char type;              // Group type: C - channel group, T - TGID group
	std::string name;       // Name (16 chars max)
	char quickKey;          // Quick key: 1 - 9, 0 means 10, .(dot) means none
	char lockout;           // Lockout/Unlocked	
	int sysIndex;           // System index	
	int seqNumber;          // Group sequence number of the System
	std::string latitude;   // North or South latitude
	std::string longitude;  // West or East longitude
	int range;              // Range: 1- 250, 1 = 0.5 mile or km
	bool gpsEnable;         // GPS location detection (On/Off)
	std::list<ChannelData> channels;
};

struct SystemData
{	
	int index;              // System index in device memory
	std::string type;       // System type (CNV, MOT, EDC, EDS, LTR, P25S, P25F)
	std::string name;       // System name (16 chars max)
	int quickKey;           // 0 - 99, -1 means none
	int holdTime;           // 0 - 255
	bool lockout;           // Lockout (Lockout/Unlocked)
	int delayTime;          // -10, -5, -2, 0, 2, 5, 10, 30		
	int sequenceNumber;     // system sequence number (1 - 500)
	int startKey;           // 0 - 9, -1 means none
	int numberTag;          // 0 - 999, -1 means none
	bool agcAnalog;         // AGC settings for analog audio (on/off)
	bool agcDigital;        // AGC settings for digital audio (on/off)
	int p25WaitingTime;     // P25 waiting time (0, 100, 200, ..., 900, 1000)
	bool protect;           // Protect bit status (on/off)
	std::list<GroupData> groups;
};

struct TrunkData
{
	int index;              // System index
	bool idSearch;          // ID search/scan mode (search/scan)
	bool statusBit;         // Motorola status bit (yes/ignore)
	int endCode;            // Motorola end code (0 - Ignore, 1 - Analog, 2 - Analog and Digital)
	bool afs;               // EDACS ID format (AFS/decimal)
	int emergencyAlert;     // 0 - 9, 0 means ignore
	int emergencyAlertLevel;// 0 - 15, 0 means off
	uint64_t customFleetMap;// Custom fleet map settings (each byte means Size Code for each Block (0 - 7)
	int tgidGroupHead;      // TGID index head of the system
	int tgidGroupTail;      // TGID index tail of the system
	int idLockoutGroupHead; // L/O TGID Group index head of the system
	int idLockoutGroupTail; // L/O TGID Group index tail of the system
	bool motorolaIdFormat;  // Motorola/P25 ID format (Hex/Decimal)
	std::string emgColor;   // Emergency Alert Light color
	int emgAlertPattern;    // Emergency Alert Light pattern (0 - on, 1 - slow, 2 - fast)
	int p25NacStatus;       // P25 NAC status (0 - 0xFFF, -1 means NAC search)
	bool priorityIdScan;    // Priority ID scan (On/Off)
};

struct SiteData
{
	int index;              // Site index
	std::string name;       // Site name
	int quickKey;           // 0 - 99, -1 means none
	int holdTime;           // Site hold time, 0 - 255
	bool lockout;           // Site lockout (Lockout/Unlocked)
	std::string modulation; // AUTO, FM, NFM
	bool attenuation;       // On/Off
	int reverseIndex;       // Reverse Site Index of the Scan Setting
	int forwardIndex;       // Forward Site Index of the Scan Setting
	int systemIndex;        // System index
	int channelHead;        // Channel index head of the Group List
	int channelTail;        // Channel index tail of the Group List
	int sequenceNumber;     // Site sequence number (1 - 255)
	int startKey;           // Startup configuration (0 - 9, -1 means none)
	double latitude;        // Site latitude (north or south)
	double longitude;       // Site longitude (west or east)
	int range;              // Site range (1 - 250, 1 = 0.5 mile or km)
	bool gpsEnable;         // GPS location detection (On/Off)
	std::string motType;    // Band type for MOT/EDACS (STD, SPL, CUSTOM)
	std::string edacsType;  // EDACS (WIDE/NARROW)
	int p25WaitingTime;     // 0, 100, 200, ..., 900, 1000
};

// Band settings for MOT800 Custom/VHF/UHF site
struct MotorolaBand
{
	int lower;              // Lower frequency
	int upper;              // Upper frequency	     
	int step;               // Step in kHz
	int offset;             // -1023 - 1023
};

// Band settings for APCO P25 site
struct P25Band
{
	int baseFreq;           // Base frequency MHz (25.0000MHz to 1300.0000MHz, 5.0Hz step)
	int spacingFreq;        // Spacing frequency (0.125kHz to 128.0kHz, 0.125kHz step)
};

using MotorolaBandPlanData = std::array<MotorolaBand, 6>;
using P25BandPlanData = std::array<MotorolaBand, 16>;

struct TrunkFreqInfoData
{
	int index;              // Trunk frequency index
	int freq;               // Trunk frequency
	int lcn;                // LCN (LTR system: 0 - 30, EDACS Wide/Narrow system: 0 - 20)
	bool lockout;           // Lockout/Unlocked
	int revIndex;           // Reverse frequency index of the Site
	int fwdIndex;           // Forward frequency index of the Site
	int sysIndex;           // System Index of the Frequency
	int grpIndex;           // Index of the Site
	int numberTag;          // Number tag, 0 - 999, -1 means none
	int volumeOffset;       // Volume offset -3 - +3
};

struct TGIDInfoData
{
	int index;              // TGID index
	std::string name;       // TGID name (16 chars max)
	bool lockout;           // Lockout/Unlocked
	bool priority;          // On/Off
	int alertTone;          // Alert tone (1 - 9, 0 means off)
	int alertToneLevel;     // Alert tone level (1 - 15, 0 means auto)
	int revIndex;           // Reverse TGID index of the Group
	int fwdIndex;           // Forward TGID index of the Group
	int sysIndex;           // System index of the TGID
	int grpIndex;           // Group index of the TGID
	int audioType;          // 0 - All, 1 - Analog only, 2 - Digital only
	int numberTag;          // Number tag (0 - 999, -1 means none)
	std::string alertColor; // Alert light color
	int alertLightPattern;  // Alert light pattern (0 - on, 1 - slow, 2 - fast)
	int volumeOffset;       // Volume offset (-3 - +3)
};

struct CloseCallSettingsData
{
	Modulation mod;         // Modulation
	bool attenuation;       // On/Off
	int delayTime;          // -10, -5, -2, 0, 1, 2, 5, 10, 30)
	int codeSearch;         // 0 - off, 1 - CTCSS/DCS, 2 - P25 NAC Search
	std::string bcastScreen;// Broadcast screen
	bool repeaterFind;      // On/Off
	int maxAutoStore;       // 1 - 256
	bool agcAnalog;         // Use AGC settings for analog audio (On/Off)
	bool agcDigital;        // Use AGC settings for digital audio (On/Off)
	int p25Waiting;         // P25 waiting time, ms (0, 100, ..., 900, 1000)
};

*/

template<typename Type> class System;
class ConventionalChannel;
class TrunkChannel;
class Scanner;

class ScanSettings
{				
public:
	~ScanSettings() = default;

	using TrunkSystem = System<TrunkChannel>;
	using ConventionalSystem = System<ConventionalChannel>;
	using UniversalSystem = std::variant<ConventionalSystem, TrunkSystem>;

	void Import(const std::string& path);
	void Export(const std::string& paht) const;

	void Load(const Scanner& scanner);
	void Save(const Scanner& scanner) const;

	const std::vector<UniversalSystem>& Systems() const
	{
		return m_systems;
	}	

	UniversalSystem& CreateSystem(const std::string& name, const SystemType type);
	void DeleteSystem(const std::string& name);

private:
	std::vector<UniversalSystem> m_systems;

	void GetSystems(const Scanner&);
};

} 

#endif  // KVASIR_SCAN_SETTINGS_H_INCLUDED