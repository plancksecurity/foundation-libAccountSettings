// ==== Auto generated ISP DB. ===
#include "isp_db.hh"

namespace account_settings {

const char* const StringPool = "16000-kbits.de\0"
	"7fun.de\0"
	"M-net\0"
	"M-net Telekommunikations GmbH\0"
	"Mailfence\0"
	"a-city.de\0"
	"alpenbazi.de\0"
	"amberg-mail.de\0"
	"augustakom.net\0"
	"bayern-mail.de\0"
	"club-goes-uefacup.de\0"
	"dachau-mail.de\0"
	"e Foundation\0"
	"e-foundation\0"
	"e.email\0"
	"ecloud.global\0"
	"erding-mail.de\0"
	"es-laendle.de\0"
	"familien-postfach.de\0"
	"fcn-cluberer.de\0"
	"franken-ist-cool.de\0"
	"franken-online.de\0"
	"frankenbeutel-mail.de\0"
	"ich-liebe-franken.de\0"
	"ich-mag-net.de\0"
	"imap.mailfence.com\0"
	"imap.mnet-online.de\0"
	"ingolstadt-mail.de\0"
	"loewen-mail.de\0"
	"mail.ecloud.global\0"
	"mail.mnet-online.de\0"
	"mail.pep-security.net\0"
	"mail.pep.security\0"
	"mail.peptest.ch\0"
	"mailfence.com\0"
	"maxi-allgaeu.de\0"
	"maxi-bayern.de\0"
	"maxi-dsl.de\0"
	"maxi-schwaben.de\0"
	"mnet-mail.de\0"
	"mnet-online.de\0"
	"muenchen-ist-toll.de\0"
	"muenchen-mail.de\0"
	"muenchen-surf.de\0"
	"myway.de\0"
	"nefkom.info\0"
	"nefkom.net\0"
	"neumarkt-mail.de\0"
	"pEp Security S.A.\0"
	"pEp Test\0"
	"pep-security.net\0"
	"pep.digital\0"
	"pep.security\0"
	"peptest.ch\0"
	"planck Test\0"
	"planck testnet\0"
	"planck.dev\0"
	"post-ist-da.de\0"
	"p≡p Security S.A.\0"
	"regensburg-mail.de\0"
	"rotgruenweiss.de\0"
	"smtp.mailfence.com\0"
	"starnberg-mail.de\0"
	"testnet.planck.dev\0"
	"unser-postfach.de\0"
	"weissblautsv.de\0"
	"wir-datschiburger.de\0";

const char* const StringPoolEnd = StringPool + 1057;
const unsigned StringPoolSize = 1057;

static const char* const SP = StringPool;

const AccountSettings_DB AccountList[] = {
	{ SP+59, SP+59,	// Mailfence	Mailfence
		{ SP+383, 993, AS_ACCESS(0x81012), AS_USERNAME(0x4001) },	// imap.mailfence.com
		{ SP+946, 465, AS_ACCESS(0x81021), AS_USERNAME(0x4001) }	// smtp.mailfence.com
	},
	{ SP+23, SP+29,	// M-net	M-net Telekommunikations GmbH
		{ SP+402, 993, AS_ACCESS(0x81012), AS_USERNAME(0x4001) },	// imap.mnet-online.de
		{ SP+475, 587, AS_ACCESS(0x41021), AS_USERNAME(0x4001) }	// mail.mnet-online.de
	},
	{ SP+186, SP+173,	// e-foundation	e Foundation
		{ SP+456, 993, AS_ACCESS(0x81012), AS_USERNAME(0x4001) },	// mail.ecloud.global
		{ SP+456, 587, AS_ACCESS(0x41021), AS_USERNAME(0x4001) }	// mail.ecloud.global
	},
	{ SP+784, SP+757,	// pep-security.net	pEp Security S.A.
		{ SP+495, 993, AS_ACCESS(0x81012), AS_USERNAME(0x4001) },	// mail.pep-security.net
		{ SP+495, 587, AS_ACCESS(0x41021), AS_USERNAME(0x4001) }	// mail.pep-security.net
	},
	{ SP+813, SP+890,	// pep.security	p≡p Security S.A.
		{ SP+517, 993, AS_ACCESS(0x81012), AS_USERNAME(0x4001) },	// mail.pep.security
		{ SP+517, 465, AS_ACCESS(0x81021), AS_USERNAME(0x4001) }	// mail.pep.security
	},
	{ SP+801, SP+890,	// pep.digital	p≡p Security S.A.
		{ SP+535, 993, AS_ACCESS(0x81012), AS_USERNAME(0x4001) },	// mail.peptest.ch
		{ SP+535, 465, AS_ACCESS(0x81021), AS_USERNAME(0x4001) }	// mail.peptest.ch
	},
	{ SP+826, SP+775,	// peptest.ch	pEp Test
		{ SP+826, 993, AS_ACCESS(0x81012), AS_USERNAME(0x4001) },	// peptest.ch
		{ SP+826, 587, AS_ACCESS(0x41021), AS_USERNAME(0x4001) }	// peptest.ch
	},
	{ SP+864, SP+837,	// planck.dev	planck Test
		{ SP+864, 993, AS_ACCESS(0x81012), AS_USERNAME(0x4001) },	// planck.dev
		{ SP+864, 587, AS_ACCESS(0x41021), AS_USERNAME(0x4001) }	// planck.dev
	},
	{ SP+983, SP+849,	// testnet.planck.dev	planck testnet
		{ SP+983, 993, AS_ACCESS(0x81012), AS_USERNAME(0x4001) },	// testnet.planck.dev
		{ SP+983, 587, AS_ACCESS(0x41021), AS_USERNAME(0x4001) }	// testnet.planck.dev
	},
};

const unsigned AccountListSize = 9;
const Domain2AS IspDB[] = {
	{ 0, 1},	// 16000-kbits.de
	{ 15, 1},	// 7fun.de
	{ 69, 1},	// a-city.de
	{ 79, 1},	// alpenbazi.de
	{ 92, 1},	// amberg-mail.de
	{ 107, 1},	// augustakom.net
	{ 122, 1},	// bayern-mail.de
	{ 137, 1},	// club-goes-uefacup.de
	{ 158, 1},	// dachau-mail.de
	{ 199, 2},	// e.email
	{ 207, 2},	// ecloud.global
	{ 221, 1},	// erding-mail.de
	{ 236, 1},	// es-laendle.de
	{ 250, 1},	// familien-postfach.de
	{ 271, 1},	// fcn-cluberer.de
	{ 287, 1},	// franken-ist-cool.de
	{ 307, 1},	// franken-online.de
	{ 325, 1},	// frankenbeutel-mail.de
	{ 347, 1},	// ich-liebe-franken.de
	{ 368, 1},	// ich-mag-net.de
	{ 422, 1},	// ingolstadt-mail.de
	{ 441, 1},	// loewen-mail.de
	{ 551, 0},	// mailfence.com
	{ 565, 1},	// maxi-allgaeu.de
	{ 581, 1},	// maxi-bayern.de
	{ 596, 1},	// maxi-dsl.de
	{ 608, 1},	// maxi-schwaben.de
	{ 625, 1},	// mnet-mail.de
	{ 638, 1},	// mnet-online.de
	{ 653, 1},	// muenchen-ist-toll.de
	{ 674, 1},	// muenchen-mail.de
	{ 691, 1},	// muenchen-surf.de
	{ 708, 1},	// myway.de
	{ 717, 1},	// nefkom.info
	{ 729, 1},	// nefkom.net
	{ 740, 1},	// neumarkt-mail.de
	{ 784, 3},	// pep-security.net
	{ 801, 5},	// pep.digital
	{ 813, 4},	// pep.security
	{ 826, 6},	// peptest.ch
	{ 864, 7},	// planck.dev
	{ 875, 1},	// post-ist-da.de
	{ 910, 1},	// regensburg-mail.de
	{ 929, 1},	// rotgruenweiss.de
	{ 965, 1},	// starnberg-mail.de
	{ 983, 8},	// testnet.planck.dev
	{ 1002, 1},	// unser-postfach.de
	{ 1020, 1},	// weissblautsv.de
	{ 1036, 1},	// wir-datschiburger.de
};

const unsigned IspDBSize = 49;


} // end of namespace account_settings.

// sizeof(Domain2AS) = 8
// sizeof(AccountSettings) = 64
// Data size: 1057 + 576 + 392 = 2025 Bytes.
// ===<End of generated file>===

