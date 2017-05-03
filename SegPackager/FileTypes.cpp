#include "FileTypes.h"

std::map<const int, std::string> FileTypes = {
	{ 0,  "sar"  },
	{ 1,  "bin"  },
	{ 2,  "gcx"  },
	{ 3,  "txn"  },
	{ 4,  "mdh"  },
	{ 5,  "mds"  },	
	{ 6,  "lt2"  },
	{ 7,  "cv2"  },
	{ 8,  "mtar" },
	{ 9,  "mtsq" },
	{ 10, "far"  },
	{ 11, "mtcm" },
	{ 12, "geom" },
	{ 13, "mdn"  },
	{ 14, "van"  }, //unknown
	{ 15, "nav"  },
	{ 16, "cvd"  },
	{ 17, "cnp"  },
	{ 18, "zon"  },
	{ 19, "rpd"  },
	{ 20, "abc"  },
	{ 21, "nv2"  },
	{ 22, "spu"  },
	{ 23, "fcv"  },
	{ 24, "phs"  },
	{ 25, "eqpp" },
	{ 26, "phpr" },
	{ 27, "phes" },
	{ 28, "sds"  },
	{ 29, "vab"  },
	{ 30, "ssp"  },
	{ 31, "rvb"  },
	{ 32, "gsp"  },
	{ 33, "dld"  },
	{ 34, "rdv"  },
	{ 35, "octt" },
	{ 36, "octl" },
	{ 37, "vfp"  },
	{ 38, "octs" },
	{ 39, "39",  }, //unknown
	{ 40, "40",  }, //unknown
	{ 41, "41",  }, //unknown
	{ 42, "ptl"  },
	{ 43, "cpef" },
	{ 44, "44"   },
	{ 44, "45"   },
	{ 44, "46"   },
	{ 44, "47"	 },
	{ 44, "48"	 },
	{ 44, "49"	 },
	{ 44, "50"	 },
	{ 44, "51"	 },
	{ 44, "52"	 },
	{ 44, "53"	 },
	{ 93, "jpg"  },
	{ 94, "ico"  },
	{ 95, "la2"  },
	{ 96, "slot" },
	{ 97, "vpo"  },
	{ 98, "fpo"  },
	{ 99, "cv4"  },
	{ 100, "mcl" },
	{ 101, "vlm" },
	{ 102, "lh4" },
	{ 103, "csr" },
	{ 104, "var" },
	{ 105, "img" },
	{ 106, "vib" },
	{ 107, "rat" },
	{ 108, "rcm" },
	{ 109, "ola" },
	{ 110, "raw" },
	{ 111, "mtra"}

	/*missing types
	nni
	dlp
	mtfa
	at3
	psq
	tri
	pdl
	bpef
	dbd
	lt3
	mdc
	dci
	png
	pam
	sfp
	cv2
	mcl
	eft
	mdl
	mdb
	*/

};

//int number = getFileTypeNumber("txn");
int getFileTypeNumber(std::string fileType) {
	std::map<const int, std::string>::iterator it;
	for (it = FileTypes.begin(); it != FileTypes.end(); ++it)
	if (it->second == fileType)
		return it->first;
}
