#ifndef ZENCORE_LINUX_DEBUG_H
#define ZENCORE_LINUX_DEBUG_H

// TODO(Abi): define these
#define HardAssert(expr) if(!(expr)) { FailedAssert(__FILE__, __LINE__, #expr, 1); }
#define SoftAssert(expr) if(!(expr)) { FailedAssert(__FILE__, __LINE__, #expr, 0); }
#define Assert HardAssert

enum debug_log_type {
    DEBUG_LOG_INFO,
    DEBUG_LOG_WARNING,
    DEBUG_LOG_ERROR
};

#define Log(...) DebugLog(0, __FILE__, __LINE__, __VA_ARGS__);
#define LogWarning(...) DebugLog(DEBUG_LOG_WARNING, __FILE__, __LINE__, __VA_ARGS__);
#define LogError(...) DebugLog(DEBUG_LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__);


#endif //ZENCORE_LINUX_DEBUG_H
