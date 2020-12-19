#ifndef ZENCORE_LINUX_DEBUG_H
#define ZENCORE_LINUX_DEBUG_H

#define HardAssert(expr) do{if(!(expr)){ FailedAssert(__FILE__,__LINE__, #expr, 1) ;}} while(0)
#define SoftAssert(expr) do{if(!(expr)){ FailedAssert(__FILE__,__LINE__, #expr, 0) ;}} while(0)
#define Assert HardAssert

enum debug_log_type {
    DEBUG_LOG_INFO,
    DEBUG_LOG_WARNING,
    DEBUG_LOG_ERROR
};

#define Log(...) DebugLog(0, __FILE__, __LINE__, __VA_ARGS__);
#define LogWarning(...) DebugLog(DEBUG_LOG_WARNING, __FILE__, __LINE__, __VA_ARGS__);
#define LogError(...) DebugLog(DEBUG_LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__);

#define PrintV4(v) Log("v4 {%f, %f, %f, %f}", (v).x, (v).y, (v).z, (v).w);
#define PrintV3(v) Log("v3 {%f, %f, %f, %f}", (v).x, (v).y, (v).z);

#endif //ZENCORE_LINUX_DEBUG_H
