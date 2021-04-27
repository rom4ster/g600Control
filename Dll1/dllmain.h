#pragma once




#ifdef DLL1_EXPORTS
#define G600_API __declspec(dllexport)
#else
#define G600_API __declspec(dllimport)
#endif

 #include "../hidapi/hidapi.h"


/*
		KB INFO 

		A-Z 0x04-0x1D  //Non Special
		1234567890  0x1E-0x27  <-- top row   //Non Special
		F1-F12      0x3A-0x45  
		1234567890  0x59-0x62  <-- numpad
		
		ENTER 28 //Non special
		ESC 29 //Non special
		BACKSPACE 2A //NON special
		TAB 2B  //Non special
		SPACE 2c // Non special
        // Start Non Special Section
		- 2D   
		= 2E
		[ 2F
		] 30
		\ 31
		; 33
		' 34
		` 35
		, 36
		. 37
		/ 38
		// END Non Special Section
		CAPS 39
		PRNTSCREEN 46
		SCRLOCK 47
		PAUSE 48
		INSERT 49
		HOME 4A
		PGUP 4B
		DELET 4C // Non Special
		END 4D
		PGDN 4E
		RIGHT 4F
		LEFT 50
		DOWN 51
		UP   52
		NUMLCK 53
		NUM/ 54
		NUM* 55
		NUM- 56
		NUM+ 57
		NUMENTER     58
		NUMDEL 63
		APPLICATION 65




*/



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
	char* getKey(char );
	G600Api();
	~G600Api();
	

};