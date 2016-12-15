#include <iostream>

using namespace std;

class Event
{
    HANDLE hEvent;
public:
    Event() {
    hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    cout << endl << "ThreadCreate ID: " << GetCurrentThreadId() << endl;
    }


int main()
{
    cout << "Hello world!" << endl;
    return 0;
}
