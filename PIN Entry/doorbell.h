#ifndef DOORBELL_H
#define DOORBELL_H

#include <QWidget>
#include <atomic>

#define PortButton 8
#define PinButton1 8
#define PinButton2 10
#define PinButton3 12
#define PinButton4 14

namespace Ui
{
class CDoorBell;
}
namespace std
{
class thread;
}

class CDoorBell : public QWidget
{
    Q_OBJECT

public:
    explicit CDoorBell(QWidget *parent = 0);
    ~CDoorBell();

    //return 0 on success
    int Open(bool bPin, std::vector<int> pinCode);
    void Close(void);

signals:
    void ButtonPressSignal(int);


private slots:
    void OnButtonPressed(int);

private:
    void OpenDoor(void);
    void CloseDoor(void);
    void FaceIdentification(void);
    void BlinkFailure(void);
    void ThreadWork(void);
    void ChangeUserLed(bool on);// on is true if you want to turn on

    Ui::CDoorBell *ui;
    int m_state; // 0 if closed, otherwise open
    std::thread *m_pThread;
    bool m_bThreadRunning;
    bool m_bDoorOpen;
    std::atomic<bool> m_bInCritSect;
    QImage m_image[2]; //0 for close, 1 for open
    bool m_bPin;
    std::vector<int> m_pinCode;
    unsigned int m_index; //index of currently expected digit button
    bool m_bPinCorrect; //true if pin is correct until now
};

#endif // DOORBELL_H
