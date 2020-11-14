internal b32
LinuxTimerInit(void) {
    b32 Success = 0;
    struct timespec StartTS = {0};
    
    if(clock_gettime(CLOCK_MONOTONIC_RAW, &StartTS) == 0) {
        Success = 1;
        LinuxTimer.StartTime = StartTS.tv_sec * 1000000000 + StartTS.tv_nsec;
    }
    
    return Success;
}

internal f64
LinuxTimerGetTime(void) {
    struct timespec Now = {0};
    clock_gettime(CLOCK_MONOTONIC_RAW, &Now);
    
    //NOTE(Zen) Ticks is in nano seconds
    u64 Ticks = Now.tv_sec;
    Ticks *= 1000000000;
    Ticks += Now.tv_nsec;
    
    f64 Time = ((f64)Ticks - (f64)LinuxTimer.StartTime) / (f64)1000000000.0;
    return Time;
}

internal void
LinuxTimerSleep(i32 ms) {
    struct timespec Deadline = {0};
    clock_gettime(CLOCK_MONOTONIC_RAW, &Deadline);
    
    Deadline.tv_nsec += ms * 1000000;
    if(Deadline.tv_nsec >= 1000000000) {
        Deadline.tv_nsec -= 1000000000;
        Deadline.tv_sec  += 1;
    }
    
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &Deadline, NULL);
}

internal void
LinuxTimerBeginFrame(void) {
    LinuxTimer.LastFrame = LinuxTimerGetTime();
}

internal void
LinuxTimerEndFrame(void) {
    f32 EndTime = LinuxTimerGetTime();
    while((EndTime - LinuxTimer.LastFrame) < (1.f/GlobalPlatform.TargetFPS)) {
        i32 msToSleep = ((1.f/GlobalPlatform.TargetFPS) - (EndTime - LinuxTimer.LastFrame)) * 1000.f;
        if(msToSleep > 0) LinuxTimerSleep(msToSleep);
        
        EndTime = LinuxTimerGetTime();
    }
    GlobalPlatform.Delta = EndTime - LinuxTimer.LastFrame;
    GlobalPlatform.TotalTime += GlobalPlatform.Delta;
}