#include <iostream>
#include <unistd.h>

using namespace std;


void showThanksDialogBox() 
{
	system("zenity --info --text='Thank you! , Bye Bye...   : )' --title='Plant Notice' --width=200 --height=120 --timeout=3");
}

void showNeedWaterDialogBox()
{
	system("zenity --info --text='Hey, I need water !!!    : (' --title='Plant Notice' --width=200 --height=120");
}

void showDeviceOfflineDialogBox()
{
        system("zenity --info --text='Hey, I am Offline !' --title='Plant Notice' --width=200 --height=120");
}
