//////////////////////////////////////////////////////////////////////////
/// file: scanner.h
///
/// summary: Scanner device controller.h
//////////////////////////////////////////////////////////////////////////

#ifndef KVASIR_SCANNER_H_INCLUDED
#define KVASIR_SCANNER_H_INCLUDED

#include <array>
#include <memory>
#include "uniden.h"

namespace kvasir
{

// Forward declaration of device settings
struct Device;

class Scanner
{
	struct Impl;
	std::unique_ptr<Impl> m_impl;
	volatile bool m_inProgrammingMode;

	
	void EnterProgrammingMode() const;
	void ExitProgrammingMode() const;

	Backlight GetBacklightSettings() const;
	Battery GetBatterySettings() const;
	KeySettings GetKeySettings() const;
	OpeningMessage GetOpeningMessage() const;

public:
	using Response = std::vector<std::string>;

	Scanner();
	~Scanner();

	void Connect(const Device& device);
	void Disconnect();

	Response IssueCommand(const std::string& command, size_t responseSize) const;

	std::string GetModel() const;
	std::string GetFirmwareVersion() const;
	ReceptionStatus GetReceptionStatus() const;
	SystemSettings GetSystemSettings() const;
};

} // namespace kvasir

#endif // KVASIR_SCANNER_H_INCLUDED