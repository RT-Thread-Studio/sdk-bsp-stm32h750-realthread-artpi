/** 
* \file DisplayManager.cpp
* This example of functions allows to personalize the display messages. \n
*	-	Implement Init Progress Bar function to add a progress bar.
*	-	Implement load Bar function to animate the loading process 0% to 100%.
*	-	Implement DisplayMessage function to ensure the display of internal DLL messages.
*	-	Implement logMessage function to personalize the display (user messages). 
*.
* Go to the source code : \ref DisplayManager 
\example DisplayManager
* This example of functions allows to personalize the display messages. \n
*	-	Implement Init Progress Bar function to add a progress bar.
*	-	Implement load Bar function to animate the loading process 0% to 100%.
*	-	Implement DisplayMessage function to ensure the display of internal DLL messages.
*	-	Implement logMessage function to personalize the display (user messages).
*  \code{.cpp}
**/

#define _CRT_SECURE_NO_WARNINGS  // will disable warnings to use vsprintf instead vsprintf_s

#include <stdio.h> 
#include <DisplayManager.h>
#include <stdarg.h>
#include <string>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
HANDLE  console = GetStdHandle(STD_OUTPUT_HANDLE);;
CONSOLE_SCREEN_BUFFER_INFO SBInfo, CurSBInfo;
#endif

#define WIDH 50 
using namespace std;

unsigned int verbosityLevel;
unsigned int Progress = 0;


void InitPBar()
{
#ifdef _WIN32
	if (verbosityLevel == VERBOSITY_LEVEL_1)
	{
		for (int idx = 0; idx < WIDH; idx++)
		{
			console = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(console, LIGHTGREY);
			putc(177, stdout);
		}
		GetConsoleScreenBufferInfo(console, &SBInfo);
		printf(" %d%c", Progress,'%');
		SetConsoleTextAttribute(console, LIGHTCYAN);
		putc('\r', stdout);
		Progress = 0;
	}
#endif
}

void lBar(int currProgress, int total)
{
	unsigned int alreadyLoaded = 0;
    if (total == 0) return;
	if (currProgress > total)
		currProgress = total;

	/*Calculuate the ratio of complete-to-incomplete.*/
	float ratio = currProgress / (float)total;
    unsigned int   counter = (unsigned int)ratio * WIDH;

	if ((counter > alreadyLoaded) && (verbosityLevel == VERBOSITY_LEVEL_1))
	{
#ifdef _WIN32
		SetConsoleTextAttribute(console, FOREGROUND_GREEN);

		for (DWORD Idx = Progress; Idx < (counter - alreadyLoaded); Idx++)
		{
			putc(219, stdout);
		}
		Progress = counter;
		GetConsoleScreenBufferInfo(console, &CurSBInfo);
		SetConsoleCursorPosition(console, SBInfo.dwCursorPosition);

        printf(" %d%%", (int)(ratio * 100));
		SetConsoleCursorPosition(console, CurSBInfo.dwCursorPosition);
#else

		printf("\033[00;32m");
		printf("[");
		for (int i = 0; i < counter; i++)
		{
			printf("=");
		}
		for (int i = counter; i < WIDH; i++)
		{
			printf(" ");
		}
		// draw the percentage progress

		printf("] %3d%% \r", (int)(ratio * 100)); /* Move to the first column*/
		printf("\033[39;49m");
#endif
	}
	alreadyLoaded = counter;
}


void DisplayMessage(int msgType, const wchar_t* str)
{

#ifdef _WIN32
	switch (msgType)
	{
	default:
	case  Normal:
		SetConsoleTextAttribute(console, WHITE);
		break;
	case  GreenInfo:
		SetConsoleTextAttribute(console, GREEN);
		break;
	case  Info:
		SetConsoleTextAttribute(console, LIGHTGREY);
		break;
	case  Title:
		SetConsoleTextAttribute(console, LIGHTCYAN);
		break;
	case  Error:
		SetConsoleTextAttribute(console, RED);
		break;
	case  Warning:
		SetConsoleTextAttribute(console, YELLOW);
		break;
	case ErrorNoPopup:
		SetConsoleTextAttribute(console, RED);
		break;
	case WarningNoPopup:
		SetConsoleTextAttribute(console, YELLOW);
		break;
	case Verbosity_1:
		SetConsoleTextAttribute(console, CYAN);
		break;
	case Verbosity_2:
		SetConsoleTextAttribute(console, CYAN);
		break;
	case Verbosity_3:
		SetConsoleTextAttribute(console, CYAN);
		break;
	}

	if ((msgType != Verbosity_1 && msgType != Verbosity_2 && msgType != Verbosity_3) || ((msgType == Verbosity_1 && verbosityLevel >= VERBOSITY_LEVEL_1) || (msgType == Verbosity_2 && verbosityLevel >= VERBOSITY_LEVEL_2) || (msgType == Verbosity_3 && verbosityLevel == VERBOSITY_LEVEL_3))) {

		wprintf(L"%s\n", str);
	}
	SetConsoleTextAttribute(console, WHITE); 
#else
	if (verbosityLevel == VERBOSITY_LEVEL_1)
	{
		if ((msgType == Verbosity_1 && verbosityLevel >= 1))
			printf("\033[39;36m");
		if ((msgType == Verbosity_2 && verbosityLevel >= 2))
			printf("\033[39;36m");
		if ((msgType == Verbosity_3 && verbosityLevel >= 3))
			printf("\033[39;36m");

		switch (msgType)
		{
		case GreenInfo:
			printf("\033[00;32m");
			break;
		case Info:
			printf("\e[90m");
			break;
		case Normal:
			printf("\033[39;49m");
			break;
		case Warning:
			printf("\033[00;33m");
			break;
		case Error:
			printf("\033[00;31m");
			break;
		case Title:
			printf("\e[36m\033[01m");
			break;
		}
	}
	if ((msgType != Verbosity_1 && msgType != Verbosity_2 && msgType != Verbosity_3) || ((msgType == Verbosity_1 && verbosityLevel >= 1) || (msgType == Verbosity_2 && verbosityLevel >= 2) || (msgType == Verbosity_3 && verbosityLevel == 3)))
	{
		std::wcout << str << std::endl;
	}
	printf("\033[39;49m");
#endif
}

void logMessage(int msgType, const char* str, ...)
{
#ifdef _WIN32
	switch (msgType)
	{
	default:
	case  Normal:
		SetConsoleTextAttribute(console, WHITE);
		break;
	case  GreenInfo:
		SetConsoleTextAttribute(console, GREEN);
		break;
	case  Info:
		SetConsoleTextAttribute(console, LIGHTGREY);
		break;
	case  Title:
		SetConsoleTextAttribute(console, LIGHTCYAN);
		break;
	case  Error:
		SetConsoleTextAttribute(console, RED);
		break;
	case  Warning:
		SetConsoleTextAttribute(console, YELLOW);
		break;
	case ErrorNoPopup:
		SetConsoleTextAttribute(console, RED);
		break;
	case WarningNoPopup:
		SetConsoleTextAttribute(console, YELLOW);
		break;
	case Verbosity_1:
		SetConsoleTextAttribute(console, CYAN);
		break;
	case Verbosity_2:
		SetConsoleTextAttribute(console, CYAN);
		break;
	case Verbosity_3:
		SetConsoleTextAttribute(console, CYAN);
		break;
	}
#else
	switch (msgType)
	{
	case GreenInfo:
		printf("\033[00;32m");
		break;
	case Info:
		printf("\e[90m");
		break;
	case Normal:
		printf("\033[39;49m");
		break;
	case Warning:
		printf("\033[00;33m");
		break;
	case Error:
		printf("\033[00;31m");
		break;
	case Title:
		printf("\e[36m\033[01m");
		break;
	}
#endif
	va_list args;
	va_start(args, str);
    char buffer[256];
    vsprintf(buffer, str, args);
	printf("%s", buffer);
	va_end(args);

#ifdef _WIN32
	SetConsoleTextAttribute(console, WHITE); 
#else
	printf("\033[39;49m");
#endif
}

/** \endcode **/
