
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGREY 7
#define DARKGREY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15
#define BLINK 128


typedef enum verbosity
{
	VERBOSITY_LEVEL_0 = 0,
	VERBOSITY_LEVEL_1 = 1,
	VERBOSITY_LEVEL_2 = 2,
	VERBOSITY_LEVEL_3 = 3
}verbosity;

enum MSGTYPE
{
	Normal,
	Info,
	GreenInfo,
	Title,
	Warning,
	Error,
	Verbosity_1,
	Verbosity_2,
	Verbosity_3,
	GreenInfoNoPopup,
	WarningNoPopup,
	ErrorNoPopup
};

void InitPBar();
void lBar(int x, int n);
void DisplayMessage(int msgType, const wchar_t* str);
void logMessage(int msgType, const char* str,...);