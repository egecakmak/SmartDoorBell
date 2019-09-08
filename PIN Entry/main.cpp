#include "doorbell.h"
#include <QApplication>
#include <X11/Xlib.h>
#include <vector>
#include "uniquelock.h"
#include <stdio.h>


int main(int argc, char *argv[])
{
    CUniqueLock lock; //to check if only one instance is running
    int val = lock.Open("DoorBellUniqueLock");
    if (val == -1)
    {
        printf("An instance is already running, exiting...\n");
        return 0;
    }
    else if (val == -2)
    {
        printf("Are you running as root? Exiting...\n");
        return 0;
    }
    else if (val != 0)
    {
        printf("An error occurred, exiting...\n");
        return 0;
    }
    bool bPin = false; // true if using pin code
    std::vector<int> pinCode;
    std::vector<char *> newArgv;
    for (int i = 0; i < argc; i++)
    {
        if(strcmp(argv[i], "-pin") == 0)
        {
            bPin = true;
            if (i + 1 < argc)
            {
                std::string code(argv[i+1]);
                for ( std::string::iterator it = code.begin(); it != code.end(); ++it)
                {
                    int number = *it - '0';
                    if (number > 0 && number < 5) //from 1 to 4
                    {
                        pinCode.push_back(number);
                    }
                    else
                    {
                        printf("Wrong pin argument...\nFormat: \"-pin <pincode>\" where <pincode> consists of digits from 1 to 4");
                        bPin = false;
                        break;
                    }
                }
            }
            else
            {
                printf("Wrong pin argument...\nFormat: \"-pin <pincode>\" where <pincode> consists of digits from 1 to 4");
                bPin = false;
            }

        }
        newArgv.push_back(argv[i]);
    }
    char platform[] = "-platform";// to be added to argv if no screen found
    char offscreen[] = "offscreen";
    bool screenAvail = true;
    Display *d = XOpenDisplay(NULL);
    if (!d || (XScreenCount(d) <= 0)) //no screen, will use command line
    {
        argc += 2;
        newArgv.push_back(platform);
        newArgv.push_back(offscreen);
        screenAvail = false;
    }

    QApplication a(argc, newArgv.data());
    CDoorBell w;
    if (w.Open(bPin, pinCode) != 0)
    {
        return 0;
    }
    if (screenAvail)
    {
        w.show();
    }
    int ret = a.exec();

    //clean up
    lock.Close();
    w.Close();

    return ret;
}
