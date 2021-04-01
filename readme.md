Are you tired of using LGS for you G600 MMO Gaming Mouse? Wish there was a better way to do things, well for those who own a Logitech G600 there just might be. While not a complete replacement this development API and its planned GUI software component will be faster and better than LGS. 

Also this tool provides a non garbage API for manipulating the mouse. 

WARNING THIS MANIPULATES THE MOUSE WITH HID PACKETS. <- Know what that entails. 


This sounds too good to be true is it?
Great thought. There are things that LGS can do that this software can't, some of which like button mappings are planned
but most are not.
Two things that come off the top of my head include Host profiles, meaning software profile support and gamer asthetic :C.
If you want to create these things though be my guest, im open to prs. 



NOTE: The DLL1 folder contains the actual code, the rest is just the dev environment I set up. 

IMPLEMENTED FEATURES:

Manual HIDDATA manipulation (for those who want it)
RGB Color
RGB Effect (its like pulsing or rainbow)
RGB Duration (for breathing effect)
Polling Rate (AKA Frequency)
DPI/GSHIFT DPI 

FEATURES IN DEVELOPMENT:

GSHIFT RGB Color 
BUTTON Mappings
GSHIFT BUTTON Mappings
UI/wayToUseThis


OTHER TODOS

Make a better looking readme
Use more C++ paradigms and refactor whole project
Look into Integrations if possible with other software (no guarentees)
Make length getters for types


HOW TO USE

1.  Make an instance :  G600Api *  g600 = new G600Api();   (Have not tested with smart pointers but might work)
2.  Load API and Memory Reference:       g600->load(); auto out = g600->initMemRef();
3. Do stuff
4. Delete G600Api Object with delete or smartpointer reference removal or something.


API REFERENCE 

	g600char ** initMemRef();

        Sets global char * for G600 Operations (Kinda hacky solution for bypassing memory being cleared on dll exit) 
        Make sure you only use the reference returned by this for the paramters to read/write/inject.

	int reportLength();

        Getter for the Report Length for G600 HID report        

	bool load();

        finds a mouse and populates a find info struct to set up the HID connection        

	bool read(g600char * );

        Reads in raw HID input into the parameter. Returns true on success else false

	bool write(g600char *);

        Writes HID input represented as a character stream  (conversion will happen automatically) to mouse.
        Returns true on success else false

	void inject(g600char *,inj_struct *);

        while manipulating the out param of read directly is an option, inject provides a more structured way to do it.
        Inputing the g600char * along with an inj_struct that has the specific type of change and associated values will manipulate the g600char* for you
        that value can then be used in write to apply the changes. 


	typedef struct inj_struct {

		int code;
		char* value;
		int len;
	} inj_struct;


        the code is the type of change to make (see below). The value is a string that contains the fragment 
        representing the new data (ONLY for the part that will be replaced, do not pad with 0s or anything else) in hex ofc.
        the length is how much of your provided string. 

	static const enum Codes {
		COLOR = 0, // non gshift color 6 hex letters
		EFFECT = 1,  // 00 01 or 02 
		DURATION = 2, // 00-0f
		FREQUENCY = 3, // 00-07
		DPI = 4        // [Shift_DPI][DPI_Default][DPI_valx4] where Shift_DPI and DPI_val between 04 and a4 and DPI_Default between 01 and 04 
       //CYCLE MOUSE PROFILES FOR DPI CHANGES TO BE REFLECTED !!!!!!!!!!!

	} Codes;

     These are the different codes 




QUESTIONS? Make an issue or go here
https://discord.gg/BRXGMF6geJ


FEATURE REQUEST? Please dont make an issue right away  for that, talk with me first THEN make an issue (otherwise I will lose track of these). 
https://discord.gg/BRXGMF6geJ

WANT TO HELP? help is always welcome
Make a PR or something or chat with me here https://discord.gg/BRXGMF6geJ



PROJECTS THAT I USED AND AM CREDITING

HIDAPI -> For actually doing the HID communication 
https://github.com/signal11/hidapi
LibRatbag -> Great data on what parts of the code control what, saved me manual testing. 
https://github.com/libratbag/libratbag
