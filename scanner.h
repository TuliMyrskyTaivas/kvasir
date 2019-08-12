//////////////////////////////////////////////////////////////////////////
/// file: scanner.h
///
/// summary: Scanner device controller.h
//////////////////////////////////////////////////////////////////////////

#ifndef KVASIR_SCANNER_H_INCLUDED
#define KVASIR_SCANNER_H_INCLUDED

#include <memory>
#include <vector>
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

public:
	using Response = std::vector<std::string>;

	Scanner();
	~Scanner();

	void Connect(const Device& device);
	void Disconnect();

	Response IssueCommand(const std::string& command, size_t responseSize) const;
	void EnterProgrammingMode() const;
	void ExitProgrammingMode() const;

	std::string GetModel() const;
	std::string GetFirmwareVersion() const;
	ReceptionStatus GetReceptionStatus() const;	
};

} // namespace kvasir

#endif // KVASIR_SCANNER_H_INCLUDED