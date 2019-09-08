#include "doorbell.h"
#include "ui_doorbell.h"
#include <thread>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <algorithm>
#include <unistd.h>
#include <future>
#include <stdlib.h>
#include <time.h>
#include <QImageReader>

extern "C"
{
#include "iolib.h"
}

static const char * PATHLED[] = {
	"/sys/devices/platform/leds/leds/beaglebone:green:usr0/trigger",
	"/sys/devices/platform/leds/leds/beaglebone:green:usr1/trigger",
	"/sys/devices/platform/leds/leds/beaglebone:green:usr2/trigger",
	"/sys/devices/platform/leds/leds/beaglebone:green:usr3/trigger" };


CDoorBell::CDoorBell(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CDoorBell)
{
    ui->setupUi(this);
    m_state = 0;
    m_pThread = nullptr;
    m_bThreadRunning  = false;
    connect(this, SIGNAL(ButtonPressSignal(int)), this, SLOT(OnButtonPressed(int)));
}

CDoorBell::~CDoorBell()
{
    delete ui;
    Close();
}

int CDoorBell::Open(
        bool bPin, std::vector<int> pinCode)
{
    Close();
    m_state = 1; //open
    if (::iolib_init() == -1)
    {
        printf("Are you running as root? Exiting...\n");
        Close();
        return -1;
    }
    m_bInCritSect = false;
    m_bPin = bPin;
    ::iolib_setdir(PortButton, PinButton1, DIR_IN);
    if (m_bPin)
    {
        m_pinCode = pinCode;
        for (int elem : m_pinCode) //making sure that pincode is valid
        {
            if (elem > 4 || elem < 1)
            {
                printf("Something went wrong :( Exiting...\n");
                Close();
                return -1;
            }
        }
        ::iolib_setdir(PortButton, PinButton2, DIR_IN);
        ::iolib_setdir(PortButton, PinButton3, DIR_IN);
        ::iolib_setdir(PortButton, PinButton4, DIR_IN);
        m_bPinCorrect = true;
        m_index = 0;
    }

    QImageReader readerOpen(":/door/OpenDoor.png");
    QImageReader readerClose(":/door/CloseDoor.jpg");
    readerOpen.setAutoTransform(true);
    readerClose.setAutoTransform(true);
    m_image[0] = readerClose.read();
    m_image[1] = readerOpen.read();
    if (m_image[0].isNull() || m_image[1].isNull())
    {
        printf("Something went wrong :( Exiting...\n");
        Close();
        return -1;
    }

    CloseDoor(); //init with closed door
    m_bThreadRunning = true;
    m_pThread = new std::thread([this] {this->ThreadWork();} );

    printf("Welcome! Type \"exit\" at any time to close the app\n");
    return 0;
}

void CDoorBell::Close()
{
    if (m_state == 0)
    {
        return;
    }
    ::iolib_free();

    m_bThreadRunning = false;
    if (m_pThread)
    {
        m_pThread->join();
        delete m_pThread;
    }
    m_pinCode.clear();
    //restore LEDs
    std::fstream led;
    led.open (PATHLED[0], std::fstream::out | std::fstream::trunc);
    led << "heartbeat";
    led.close();

    led.open (PATHLED[1], std::fstream::out | std::fstream::trunc);
    led << "mmc0";
    led.close();

    led.open (PATHLED[2], std::fstream::out | std::fstream::trunc);
    led << "cpu0";
    led.close();

    led.open (PATHLED[3], std::fstream::out | std::fstream::trunc);
    led << "mmc1";
    led.close();

    m_state = 0; //close
}


void CDoorBell::ThreadWork()
{

    std::string input;
    auto lambda = [] {std::string input;
                        std::cin >> input;
                        return input; };
    std::future<std::string> future = std::async(std::launch::async, lambda);

    if (m_bPin)
    {
        while (m_bThreadRunning)
        {
            if (::is_high(PortButton, PinButton1))
            {
                //to close door only when buttons 1 and 4 are pressed at the same time
                if (!m_bDoorOpen || ::is_high(PortButton, PinButton4))
                {
                    bool bFalse = false;
                    if (m_bInCritSect.compare_exchange_strong(bFalse, true)) // if m_bInCritSect == false then m_bInCritSect = true
                    {

                        emit ButtonPressSignal(1);
                    }
                }
            }
            if (!m_bDoorOpen) // no need to listen for those button when door is open
            {
                if (::is_high(PortButton, PinButton2))
                {
                    bool bFalse = false;
                    if (m_bInCritSect.compare_exchange_strong(bFalse, true)) // if m_bInCritSect == false then m_bInCritSect = true
                    {
                        emit ButtonPressSignal(2);
                    }
                }
                if (::is_high(PortButton, PinButton3))
                {
                    bool bFalse = false;
                    if (m_bInCritSect.compare_exchange_strong(bFalse, true)) // if m_bInCritSect == false then m_bInCritSect = true
                    {
                        emit ButtonPressSignal(3);
                    }
                }
            }
            if (::is_high(PortButton, PinButton4))
            {
                //to close door only when buttons 1 and 4 are pressed at the same time
                if (!m_bDoorOpen || ::is_high(PortButton, PinButton1))
                {
                    bool bFalse = false;
                    if (m_bInCritSect.compare_exchange_strong(bFalse, true)) // if m_bInCritSect == false then m_bInCritSect = true
                    {

                        emit ButtonPressSignal(4);
                    }
                }
            }
            if (future.wait_for(std::chrono::milliseconds(200)) == std::future_status::ready)
            {
                input = future.get();
                std::transform(input.begin(), input.end(), input.begin(), ::toupper);
                if (input == "EXIT")
                {
                    this->close();
                }
                else
                {
                    future = std::async(std::launch::async, lambda);
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }
    }
    else // if no pin
    {
        while (m_bThreadRunning)
        {
            if (::is_high(PortButton, PinButton1))
            {
                bool bFalse = false;
                if (m_bInCritSect.compare_exchange_strong(bFalse, true)) // if m_bInCritSect == false then m_bInCritSect = true
                {
                    emit ButtonPressSignal(1);
                }
            }
            if (future.wait_for(std::chrono::milliseconds(100)) == std::future_status::ready)
            {
                input = future.get();
                std::transform(input.begin(), input.end(), input.begin(), ::toupper);
                if (input == "EXIT")
                {
                    this->close();
                }
                else
                {
                    future = std::async(std::launch::async, lambda);
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
}


void CDoorBell::ChangeUserLed(bool on)
{
    const char * pTrigger = (on) ? "default-on" : "none";
    std::fstream led;

    for (int i = 0; i < 4; i++)
    {
        led.open (PATHLED[i], std::fstream::out | std::fstream::trunc);
        led << pTrigger;
        led.close();
    }
}

void CDoorBell::OnButtonPressed(int button)
{
    assert(m_bInCritSect);
    assert(button > 0 && button < 5);
    if (m_bDoorOpen)
    {
        CloseDoor();
    }
    else
    {
        if (m_bPin)
        {
            printf("You entered #%d\n", button);
            m_bPinCorrect &= (button == m_pinCode[m_index]);
            if (++m_index == m_pinCode.size())
            {
                if (m_bPinCorrect)
                {
                    OpenDoor();
                }
                else
                {
                    BlinkFailure();
                }
                m_index = 0;
                m_bPinCorrect = true;
            }
        }
        else
        {            
            FaceIdentification();
        }
    }
    m_bInCritSect = false;
}

void CDoorBell::OpenDoor()
{
    printf("Door is Open!\n");
    m_bDoorOpen = true;
    ChangeUserLed(true);
    ui->imageLabel->setPixmap(QPixmap::fromImage(m_image[1]));
}

void CDoorBell::CloseDoor()
{
    printf("Door is Closed!\n");
    m_bDoorOpen = false;
    ChangeUserLed(false);
    ui->imageLabel->setPixmap(QPixmap::fromImage(m_image[0]));
}

void CDoorBell::BlinkFailure()
{
    for (int i = 0; i < 4; i++)
    {
        ChangeUserLed(true);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        ChangeUserLed(false);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    printf("Sorry could not identify you, please try again\n");
}

void CDoorBell::FaceIdentification()
{
    printf("UI can be accessed at the following address:\n192.168.7.2:5000");
    bool wasHidden =this->isHidden();
    this->hide();
    if (std::system("python3 ./web.py") != 0)
    {
        printf("Sorry something went wrong... Did you do the proper setup? (refer to manual)\nExiting...\n");
        close(); //QWidget::close()
    }
    if (!wasHidden)
    {
        this->show();
    }
}
