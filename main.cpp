#include <iostream>
#include <windows.h>
using namespace std;

class Event
{
    HANDLE hEvent;
public:
    Event() {
    hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    cout << endl << "ThreadCreate ID: " << GetCurrentThreadId() << endl;
    }

    ~Event() {
    cout << endl << "ThreadDestoyd ID: " << GetCurrentThreadId() << endl;
        CloseHandle(hEvent);
    }



    void wait() {
    cout << endl << "WaitThread ID: " << GetCurrentThreadId() << endl;
    WaitForSingleObject(hEvent, INFINITE);
    }



class CriticalSection : public Lock {
private:
    CRITICAL_SECTION CS;
public:
    CriticalSection() {
    InitializeCriticalSection(&CS);
    }

    ~CriticalSection() {
        }

int main()
{

}
