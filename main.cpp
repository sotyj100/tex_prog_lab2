#include <iostream>
#include <windows.h>
using namespace std;

int num = 0;

class Event
{
    HANDLE hEvent;
public:
    Event() {
    hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    cout << endl << "ThreadCreateId: " << GetCurrentThreadId() << endl;
    }

    ~Event() {
    cout << endl << "ThreadDestoydId: " << GetCurrentThreadId() << endl;
        CloseHandle(hEvent);
    }


    void set() {
    SetEvent(hEvent);
    cout << endl << "ThreadDestoydId: " << GetCurrentThreadId() << endl;
    }

    void reset(){
    ResetEvent(hEvent);
    }

    void wait() {
   cout << endl << "WaitThreadId: " << GetCurrentThreadId() << endl;
    WaitForSingleObject(hEvent, INFINITE);
    }

};


class Lock{
public:
    virtual void lock() = 0;
    virtual void unlock() = 0;
    virtual ~Lock() {}
};



class CriticalSection : public Lock {
private:
    CRITICAL_SECTION Crit_Sect;
public:
    CriticalSection() {
    InitializeCriticalSection(&Crit_Sect);
    }

    ~CriticalSection() {
    }

void lock() {
    EnterCriticalSection(&Crit_Sect);
    }

    void unlock() {
    LeaveCriticalSection(&Crit_Sect);
    }
};

class Semaphor : public Lock {
private:
    int count_m;
    int count;
    CriticalSection *tCrit_Sect;
    Event *event;
public:
    Semaphor(int count_m) : count_m(count_m), count(0){
    tCrit_Sect = new CriticalSection();
    event = new Event();
    }

    ~Semaphor(){
    delete event;
    delete tCrit_Sect;
    }

    void lock(){
        tCrit_Sect->lock();
        if(count++ < count_m){
            tCrit_Sect->unlock();
            return;
        }
        tCrit_Sect->unlock();
        event->wait();
    }

    void unlock(){
    tCrit_Sect->lock();
    if(count-- >= count_m){
        event->set();
    }
    tCrit_Sect->unlock();
    }
};

Semaphor *SEMAPHOR;

void inc(void){
cout << endl << "Inc = " << num++ << endl;
}


DWORD WINAPI Foo(PVOID){
    while(int i = 5){
        SEMAPHOR->lock();
        inc();
        Sleep(2000);
        SEMAPHOR->unlock();
        i--;
    }
    return 0;
}

int main()
{

    cout << "Vvod num: ";
    cin >> num;
HANDLE ThreadArray[15];
SEMAPHOR = new Semaphor(5);
for(register int i=0; i<15; i++){
    ThreadArray[i] = CreateThread(NULL, 0, Foo, NULL, 0, NULL);
  }


DWORD result_stop_thread = WaitForMultipleObjects(15, ThreadArray, true, INFINITE);

  switch (result_stop_thread){
  case WAIT_TIMEOUT:{
  cout << "Stop timeout!" << endl;
  break;
  }


  case WAIT_OBJECT_0:{
  cout << "THREAD " << GetCurrentThreadId() << "READ." << endl;
  break;
  }
  default:{
  cout << "Osibka: " << GetLastError() << endl;
  break;
  }
  }


  for (register int i=0; i<15; i++){
    CloseHandle(ThreadArray[i]);
  }

  delete SEMAPHOR;
  SEMAPHOR = NULL;

  return 0;
}
