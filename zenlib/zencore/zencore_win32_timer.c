typedef struct win32_timer_info win32_timer_info;
struct win32_timer_info {
    LARGE_INTEGER ClockFrequency;
    LARGE_INTEGER StartTime;
    LARGE_INTEGER LastFrame;
    BOOL          SetSleepGranular;
};

global win32_timer_info Win32Timer;

internal f64
Win32TimerGetTime(void) {
    LARGE_INTEGER Now;
    QueryPerformanceCounter(&Now);
    return ((f64)(Now.QuadPart - Win32Timer.StartTime.QuadPart) / ((f64)Win32Timer.ClockFrequency.QuadPart));
}

internal void
Win32TimerInit(void) {
    UINT DesiredSleepGranularity = 1;
    Win32Timer.SetSleepGranular = timeBeginPeriod(DesiredSleepGranularity) == TIMERR_NOERROR;
    QueryPerformanceFrequency(&Win32Timer.ClockFrequency);
    
    QueryPerformanceCounter(&Win32Timer.StartTime);
    Win32Timer.LastFrame = Win32Timer.StartTime;
}

internal void
Win32TimerEndFrame(void) {
    LARGE_INTEGER EndTime;
    QueryPerformanceCounter(&EndTime);
    
    f64 TimeTaken = ((f64)(EndTime.QuadPart-Win32Timer.LastFrame.QuadPart) /
                     (f64)(Win32Timer.ClockFrequency.QuadPart));
    while(TimeTaken < 1.f/GlobalPlatform.TargetFPS) {
        if(Win32Timer.SetSleepGranular) {
            DWORD TimeToWait = (DWORD) (((1.f/GlobalPlatform.TargetFPS)-TimeTaken) * 1000);
            if(TimeToWait > 0) {
                Sleep(TimeToWait);
            }
        }
        
        QueryPerformanceCounter(&EndTime);
        TimeTaken = ((f64)(EndTime.QuadPart-Win32Timer.LastFrame.QuadPart) /
                     (f64)(Win32Timer.ClockFrequency.QuadPart));
    }
    Win32Timer.LastFrame = EndTime;
    GlobalPlatform.Delta = TimeTaken;
    GlobalPlatform.TotalTime += GlobalPlatform.Delta;
}