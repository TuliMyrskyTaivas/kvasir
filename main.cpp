//////////////////////////////////////////////////////////////////////////
/// file:	main.cpp
///
/// summary: entry point of the application
//////////////////////////////////////////////////////////////////////////

#include "config.h"
#include "logger.h"
#include "scanner.h"

#include <QtCore/QDir>
#include <QtCore/QTimer>
#include <QtCore/QStandardPaths>
#include <QtCore/QCoreApplication>
#include <QtSerialPort/QSerialPortInfo>
#include <QtMultimedia/QAudioRecorder>

#include <iostream>
#include <cstdlib>

#ifdef _WIN32
// It's required to initialize COM before working with Qt Multimedia
# include <objbase.h>
# include <rpcdce.h>
#endif _WIN32

class DiscoveryTask : public QObject
{
	Q_OBJECT
public:
	DiscoveryTask(QObject* parent = nullptr)
		: QObject(parent)
	{}

public slots:
	void run()	
	{
		try
		{
			const auto dataLocations = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
			if (dataLocations.empty())
				throw std::runtime_error("failed to determine path to the data directory");

			kvasir::Logger& log = kvasir::Logger::GetInstance();
			kvasir::Config config(QDir(dataLocations.first()).filePath("config.db").toStdString());

			log.Info() << "Configured devices:";
			for (const auto& device : config.GetDevices())
			{
				log.Info() << "\t- " << device.name << " at " << device.port << ' ' << device.baudRate
					<< ' ' << device.dataBits << (device.parityCheck ? 'E' : 'N') << device.stopBits;
			}

			kvasir::Scanner scanner;
			scanner.Connect(config.GetDevices().front());
			log.Info() << "Scanner model: " << scanner.GetModel();
			log.Info() << "Firmware version: " << scanner.GetFirmwareVersion();

			const auto status = scanner.GetReceptionStatus();
			log.Info() << "Current channel: " << status.channel;

			const auto sysSettings = scanner.GetSystemSettings();
			log.Info() << "System settings:";
			log.Info() << "\t- battery save: " << (sysSettings.battery.batterySave ? "on" : "off");
			log.Info() << "\t- battery charge time: " << sysSettings.battery.chargeTime;
			log.Info() << "\t- backlight color: " << sysSettings.backlight.color;
			log.Info() << "\t- backlight event: " << sysSettings.backlight.event;
			log.Info() << "\t- opening message: ";
			for (const auto& line : sysSettings.openingMessage)
				log.Info() << "\t-\t" << line;
			emit finished();
		}
		catch (const std::exception& e)
		{
			std::cerr << "failure: " << e.what() << std::endl;
			emit finished();
		}

		/*
		const auto portList = QSerialPortInfo::availablePorts();
		log.Info() << "COM ports:";
		for (const auto& port : portList)
		{
			log.Info() << "\t- " << port.portName().toStdString();
		}

		QAudioRecorder recorder;
		log.Info() << "Default audio input: " <<recorder.defaultAudioInput().toStdString();
		log.Info() << "Audio inputs:";
		for (const auto& inputs : recorder.audioInputs())
		{
			log.Info() << "\t- " << inputs.toStdString();
		}
		*/
	}	

signals:
	void finished();
};

int main(int argc, char* argv[])
{	
#ifdef _WIN32
	// It's required to initialize COM before working with Qt Multimedia
	::CoInitializeEx(0, COINIT_APARTMENTTHREADED /*COINIT_MULTITHREADED*/);
	::CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_PKT,
		RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
#endif // _WIN32
	QCoreApplication app(argc, argv);

	// Task parented to the application so that it
	// will be deleted by the application
	DiscoveryTask* task = new DiscoveryTask(&app);

	// This will cause the application to exit when
	// the task signals "finished"
	QObject::connect(task, SIGNAL(finished()), &app, SLOT(quit()));

	// This will run the task from the application event loop
	QTimer::singleShot(0, task, SLOT(run()));

	return app.exec();
}

#include "main.moc"