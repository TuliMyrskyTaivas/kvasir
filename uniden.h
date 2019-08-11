//////////////////////////////////////////////////////////////////////////
/// file: uniden.h
///
/// summary: Uniden-specific types
//////////////////////////////////////////////////////////////////////////

#ifndef KVASIR_UNIDEN_H_INCLUDED
#define KVASIR_UNIDEN_H_INCLUDED

namespace kvasir
{

//////////////////////////////////////////////////////////////////////////
enum class Modulation
{
	None,
	AM,
	FM,
	NFM,
	WFM,
	FMB
};

//////////////////////////////////////////////////////////////////////////
enum class CtcssDcsCode : unsigned int
{
	None = 0,
	CTCSS_67_0_Hz = 64,
	CTCSS_69_3_Hz = 65,
	CTCSS_71_9_Hz = 66,
	CTCSS_74_4_Hz = 67,
	CTCSS_77_0_Hz = 68,
	CTCSS_79_7_Hz = 69,
	CTCSS_82_5_Hz = 70,
	CTCSS_85_4_Hz = 71,
	CTCSS_88_5_Hz = 72,
	CTCSS_91_5_Hz = 73,
	CTCSS_94_8_Hz = 74,
	CTCSS_97_4_Hz = 75,
	CTCSS_100_0_Hz = 76,
	CTCSS_103_5_Hz = 77,
	CTCSS_107_2_Hz = 78,
	CTCSS_110_9_Hz = 79,
	CTCSS_114_8_Hz = 80,
	CTCSS_118_8_Hz = 81,
	CTCSS_123_0_Hz = 82,
	CTCSS_127_3_Hz = 83,
	CTCSS_131_8_Hz = 84,
	CTCSS_136_5_Hz = 85,
	CTCSS_141_3_Hz = 86,
	CTCSS_146_2_Hz = 87,
	CTCSS_151_4_Hz = 88,
	CTCSS_156_7_Hz = 89,
	CTCSS_159_8_Hz = 90,
	CTCSS_162_2_Hz = 91,
	CTCSS_165_5_Hz = 92,
	CTCSS_167_9_Hz = 93,
	CTCSS_171_3_Hz = 94,
	CTCSS_173_8_Hz = 95,
	CTCSS_177_3_Hz = 96,
	CTCSS_179_9_Hz = 97,
	CTCSS_183_5_Hz = 98,
	CTCSS_186_2_Hz = 99,
	CTCSS_189_9_Hz = 100,
	CTCSS_192_8_Hz = 101,
	CTCSS_196_6_Hz = 102,
	CTCSS_199_5_Hz = 103,
	CTCSS_203_5_Hz = 104,
	CTCSS_206_5_Hz = 105,
	CTCSS_210_7_Hz = 106,
	CTCSS_218_1_Hz = 107,
	CTCSS_225_7_Hz = 108,
	CTCSS_229_1_Hz = 109,
	CTCSS_233_6_Hz = 110,
	CTCSS_241_8_Hz = 111,
	CTCSS_250_3_Hz = 112,
	CTCSS_254_1_Hz = 113,
	DCS_023 = 128,
	DCS_025 = 129,
	DCS_026 = 130,
	DCS_031 = 131,
	DCS_032 = 132,
	DCS_036 = 133,
	DCS_043 = 134,
	DCS_047 = 135,
	DCS_051 = 136,
	DCS_053 = 137,
	DCS_054 = 138,
	DCS_065 = 139,
	DCS_071 = 140,
	DCS_072 = 141,
	DCS_073 = 142,
	DCS_074 = 143,
	DCS_114 = 144,
	DCS_115 = 145,
	DCS_116 = 146,
	DCS_122 = 147,
	DCS_125 = 148,
	DCS_131 = 149,
	DCS_132 = 150,
	DCS_134 = 151,
	DCS_143 = 152,
	DCS_145 = 153,
	DCS_152 = 154,
	DCS_155 = 155,
	DCS_156 = 156,
	DCS_162 = 157,
	DCS_165 = 158,
	DCS_172 = 159,
	DCS_174 = 160,
	DCS_205 = 161,
	DCS_212 = 162,
	DCS_223 = 163,
	DCS_225 = 164,
	DCS_226 = 165,
	DCS_243 = 166,
	DCS_244 = 167,
	DCS_245 = 168,
	DCS_246 = 169,
	DCS_251 = 170,
	DCS_252 = 171,
	DCS_255 = 172,
	DCS_261 = 173,
	DCS_263 = 174,
	DCS_265 = 175,
	DCS_266 = 176,
	DCS_271 = 177,
	DCS_274 = 178,
	DCS_306 = 179,
	DCS_311 = 180,
	DCS_315 = 181,
	DCS_325 = 182,
	DCS_331 = 183,
	DCS_332 = 184,
	DCS_343 = 185,
	DCS_346 = 186,
	DCS_351 = 187,
	DCS_356 = 188,
	DCS_364 = 189,
	DCS_365 = 190,
	DCS_371 = 191,
	DCS_411 = 192,
	DCS_412 = 193,
	DCS_413 = 194,
	DCS_423 = 195,
	DCS_431 = 196,
	DCS_432 = 197,
	DCS_445 = 198,
	DCS_446 = 199,
	DCS_452 = 200,
	DCS_454 = 201,
	DCS_455 = 202,
	DCS_462 = 203,
	DCS_464 = 204,
	DCS_465 = 205,
	DCS_466 = 206,
	DCS_503 = 207,
	DCS_506 = 208,
	DCS_516 = 209,
	DCS_523 = 210,
	DCS_526 = 211,
	DCS_532 = 212,
	DCS_546 = 213,
	DCS_565 = 214,
	DCS_606 = 215,
	DCS_612 = 216,
	DCS_624 = 217,
	DCS_627 = 218,
	DCS_631 = 219,
	DCS_632 = 220,
	DCS_654 = 221,
	DCS_662 = 222,
	DCS_664 = 223,
	DCS_703 = 224,
	DCS_712 = 225,
	DCS_723 = 226,
	DCS_731 = 227,
	DCS_732 = 228,
	DCS_734 = 229,
	DCS_743 = 230,
	DCS_754 = 231
};

struct ReceptionStatus
{
	std::string freq;                       // Frequency or TGID
	std::string site;                       // System, site or search name
	std::string group;                      // Group name
	std::string channel;                    // Channel name
	CtcssDcsCode code = CtcssDcsCode::None; // CTCSS/DCS code	
	Modulation mod = Modulation::None;      // Modulation type
	int systemTag = -1;                     // Current system number tag
	int channelTag = -1;                    // Current channel number tag
	int p25Nac = -1;                        // P25 Network Access Code
	bool att = false;                       // Attenuation (on/off)
	bool squelch = false;                   // Squelch status (open/close)
	bool mute = false;                      // Mute status (on/off)
};

struct Backlight
{
	std::string event;                      // Event for activation of backlight
	std::string color;                      // Backlight color
	int dimmer = -1;                        // Backlight dimmer (1:Low, 2:Middle, 3:High)
};

struct Battery
{
	bool batterySave = false;
	int chargeTime = -1;
};

struct KeySettings
{
	int beepLevel = 1;                      // Beep level (0:Auto, 1-15: volume, 99:off)
	bool keyLock = false;                   // Key Lock status
	bool keySafe = false;                   // Key Safe status
};

using OpeningMessage = std::array<std::string, 4>;

struct SystemSettings
{
	Backlight backlight;
	Battery battery;
	KeySettings keys;
	OpeningMessage openingMessage;
};

} // namespace kvasir

#endif // KVASIR_UNIDEN_H_INCLUDED