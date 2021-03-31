#pragma once




#ifdef DLL1_EXPORTS
#define G600_API __declspec(dllexport)
#else
#define G600_API __declspec(dllimport)
#endif

 #include "../hidapi/hidapi.h"

class  G600_API G600Api
{
	typedef char g600char;



private:

	g600char* global_char;
	static unsigned const short LOGITECH_VENDOR_ID = 0x046D;
	static unsigned const  short G600_PRODUCT_ID = 0xc24a;
	static unsigned const char REPORT_ID = 0xf3;
    bool HID_API_INIT;
	static unsigned const int REPORT_LENGTH = 154;
	hid_device* hidptr;
	bool findInfo(hid_device_info** outt, hid_device_info* list, int* path);
	unsigned char* makeHex(char *);
	char* makeString(unsigned char *);
	static unsigned  const int LOGITECH_COLOR_INDEX = 2;
	static unsigned const int LOGITECH_EFFECT_INDEX = 8;
	static unsigned const int LOGITECH_DURATION_INDEX = 10;
	static unsigned const int LOGITECH_FREQUENCY_INDEX = 22;
	static unsigned const int LOGITECH_DPI_INDEX = 24;
public:
	static const enum Codes {
		COLOR = 0,
		EFFECT = 1,
		DURATION = 2,
		FREQUENCY = 3,
		DPI = 4

	} Codes;
	typedef struct inj_struct {

		int code;
		char* value;
		int len;
	} inj_struct;

	g600char ** initMemRef();
	int reportLength();
	bool load();
	bool read(g600char * );
	bool write(g600char *);
	void inject(g600char *,inj_struct *);
	G600Api();
	~G600Api();
	

};