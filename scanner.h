//////////////////////////////////////////////////////////////////////////
/// file: scanner.h
///
/// summary: Scanner device controller.h
//////////////////////////////////////////////////////////////////////////

#ifndef KVASIR_SCANNER_H_INCLUDED
#define KVASIR_SCANNER_H_INCLUDED

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

	std::string IssueCommand(const std::string& command) const;

public:
	Scanner();
	~Scanner();

	void Connect(const Device& device);
	void Disconnect();

	std::string GetModel() const;
	std::string GetFirmwareVersion() const;
	ReceptionStatus GetReceptionStatus() const;
};

} // namespace kvasir

#endif // KVASIR_SCANNER_H_INCLUDED