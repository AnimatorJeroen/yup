/*
  ==============================================================================

   This file is part of the YUP library.
   Copyright (c) 2024 - kunitoki@gmail.com

   YUP is an open source library subject to open-source licensing.

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   to use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   YUP IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2022 - Raw Material Software Limited

   JUCE is an open source library subject to commercial or open-source
   licensing.

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

/*
    Note that a lot of methods that you'd expect to find in this file actually
    live in juce_posix_SharedCode.h!
*/

#define JNI_CLASS_MEMBERS(METHOD, STATICMETHOD, FIELD, STATICFIELD, CALLBACK) \
    FIELD (activityInfo, "activityInfo", "Landroid/content/pm/ActivityInfo;")

DECLARE_JNI_CLASS (AndroidResolveInfo, "android/content/pm/ResolveInfo")
#undef JNI_CLASS_MEMBERS

//==============================================================================
JavaVM* androidJNIJavaVM = nullptr;
jobject androidApkContext = nullptr;

//==============================================================================
JNIEnv* getEnv() noexcept
{
    if (androidJNIJavaVM != nullptr)
    {
        JNIEnv* env;
        androidJNIJavaVM->AttachCurrentThread (&env, nullptr);

        return env;
    }

    // You did not call Thread::initialiseJUCE which must be called at least once in your apk
    // before using any JUCE APIs. The Projucer will automatically generate java code
    // which will invoke Thread::initialiseJUCE for you.
    jassertfalse;
    return nullptr;
}

static void JNICALL juce_JavainitialiseJUCE (JNIEnv* env, jobject /*jclass*/, jobject context)
{
    JNIClassBase::initialiseAllClasses (env, context);
    Thread::initialiseJUCE (env, context);
}

extern "C" jint JNIEXPORT juce_JNI_OnLoad (JavaVM* vm, void*)
{
    // Huh? JNI_OnLoad was called two times!
    jassert (androidJNIJavaVM == nullptr);

    androidJNIJavaVM = vm;

    auto* env = getEnv();

    // register the initialisation function
    auto juceJavaClass = env->FindClass ("com/rmsl/juce/Java");

    if (juceJavaClass != nullptr)
    {
        JNINativeMethod method { "initialiseJUCE", "(Landroid/content/Context;)V", reinterpret_cast<void*> (juce_JavainitialiseJUCE) };

        auto status = env->RegisterNatives (juceJavaClass, &method, 1);
        jassert (status == 0);
    }
    else
    {
        // com.rmsl.juce.Java class not found. Apparently this project is a library
        // or was not generated by the Projucer. That's ok, the user will have to
        // call Thread::initialiseJUCE manually
        env->ExceptionClear();
    }

    return JNI_VERSION_1_2;
}

//==============================================================================
class JuceActivityWatcher final : public ActivityLifecycleCallbacks
{
public:
    JuceActivityWatcher()
    {
        LocalRef<jobject> appContext (getAppContext());

        if (appContext != nullptr)
        {
            auto* env = getEnv();

            myself = GlobalRef (CreateJavaInterface (this, "android/app/Application$ActivityLifecycleCallbacks"));
            env->CallVoidMethod (appContext.get(), AndroidApplication.registerActivityLifecycleCallbacks, myself.get());
        }

        checkActivityIsMain (androidApkContext);
    }

    ~JuceActivityWatcher() override
    {
        LocalRef<jobject> appContext (getAppContext());

        if (appContext != nullptr && myself != nullptr)
        {
            auto* env = getEnv();

            env->CallVoidMethod (appContext.get(), AndroidApplication.unregisterActivityLifecycleCallbacks, myself.get());
            clear();
            myself.clear();
        }
    }

    void onActivityStarted (jobject activity) override
    {
        auto* env = getEnv();

        checkActivityIsMain (activity);

        ScopedLock lock (currentActivityLock);

        if (currentActivity != nullptr)
        {
            // see Clarification June 2001 in JNI reference for why this is
            // necessary
            LocalRef<jobject> localStorage (env->NewLocalRef (currentActivity));

            if (env->IsSameObject (localStorage.get(), activity) != 0)
                return;

            env->DeleteWeakGlobalRef (currentActivity);
            currentActivity = nullptr;
        }

        if (activity != nullptr)
            currentActivity = env->NewWeakGlobalRef (activity);
    }

    void onActivityStopped (jobject activity) override
    {
        auto* env = getEnv();

        ScopedLock lock (currentActivityLock);

        if (currentActivity != nullptr)
        {
            // important that the comparison happens in this order
            // to avoid race condition where the weak reference becomes null
            // just after the first check
            if (env->IsSameObject (currentActivity, activity) != 0
                || env->IsSameObject (currentActivity, nullptr) != 0)
            {
                env->DeleteWeakGlobalRef (currentActivity);
                currentActivity = nullptr;
            }
        }
    }

    LocalRef<jobject> getCurrent()
    {
        ScopedLock lock (currentActivityLock);
        return LocalRef<jobject> (getEnv()->NewLocalRef (currentActivity));
    }

    LocalRef<jobject> getMain()
    {
        ScopedLock lock (currentActivityLock);
        return LocalRef<jobject> (getEnv()->NewLocalRef (mainActivity));
    }

    static JuceActivityWatcher& getInstance()
    {
        static JuceActivityWatcher activityWatcher;
        return activityWatcher;
    }

private:
    void checkActivityIsMain (jobject context)
    {
        auto* env = getEnv();

        ScopedLock lock (currentActivityLock);

        if (mainActivity != nullptr)
        {
            if (env->IsSameObject (mainActivity, nullptr) != 0)
            {
                env->DeleteWeakGlobalRef (mainActivity);
                mainActivity = nullptr;
            }
        }

        if (mainActivity == nullptr)
        {
            LocalRef<jobject> appContext (getAppContext());
            auto mainActivityPath = getMainActivityClassPath();

            if (mainActivityPath.isNotEmpty())
            {
                auto clasz = env->GetObjectClass (context);
                auto activityPath = juceString (LocalRef<jstring> ((jstring) env->CallObjectMethod (clasz, JavaClass.getName)));

                // This may be problematic for apps which use several activities with the same type. We just
                // assume that the very first activity of this type is the main one
                if (activityPath == mainActivityPath)
                    mainActivity = env->NewWeakGlobalRef (context);
            }
        }
    }

    static String getMainActivityClassPath()
    {
        static String mainActivityClassPath;

        if (mainActivityClassPath.isEmpty())
        {
            LocalRef<jobject> appContext (getAppContext());

            if (appContext != nullptr)
            {
                auto* env = getEnv();

                LocalRef<jobject> pkgManager (env->CallObjectMethod (appContext.get(), AndroidContext.getPackageManager));
                LocalRef<jstring> pkgName ((jstring) env->CallObjectMethod (appContext.get(), AndroidContext.getPackageName));

                LocalRef<jobject> intent (env->NewObject (AndroidIntent, AndroidIntent.constructWithString, javaString ("android.intent.action.MAIN").get()));

                intent = LocalRef<jobject> (env->CallObjectMethod (intent.get(),
                                                                   AndroidIntent.setPackage,
                                                                   pkgName.get()));

                LocalRef<jobject> resolveInfo (env->CallObjectMethod (pkgManager.get(), AndroidPackageManager.resolveActivity, intent.get(), 0));

                if (resolveInfo != nullptr)
                {
                    LocalRef<jobject> activityInfo (env->GetObjectField (resolveInfo.get(), AndroidResolveInfo.activityInfo));
                    LocalRef<jstring> jName ((jstring) env->GetObjectField (activityInfo.get(), AndroidPackageItemInfo.name));
                    LocalRef<jstring> jPackage ((jstring) env->GetObjectField (activityInfo.get(), AndroidPackageItemInfo.packageName));

                    mainActivityClassPath = juceString (jName);
                }
            }
        }

        return mainActivityClassPath;
    }

    GlobalRef myself;
    CriticalSection currentActivityLock;
    jweak currentActivity = nullptr;
    jweak mainActivity = nullptr;
};

//==============================================================================
#if JUCE_MODULE_AVAILABLE_juce_events && JUCE_ANDROID
void juce_juceEventsAndroidStartApp();
#endif

void Thread::initialiseJUCE (void* jniEnv, void* context)
{
    static CriticalSection cs;
    ScopedLock lock (cs);

    // jniEnv and context should not be null!
    jassert (jniEnv != nullptr && context != nullptr);

    auto* env = static_cast<JNIEnv*> (jniEnv);

    if (androidJNIJavaVM == nullptr)
    {
        JavaVM* javaVM = nullptr;

        auto status = env->GetJavaVM (&javaVM);
        jassert (status == 0 && javaVM != nullptr);

        androidJNIJavaVM = javaVM;
    }

    static bool firstCall = true;

    if (firstCall)
    {
        firstCall = false;

        // if we ever support unloading then this should probably be a weak reference
        androidApkContext = env->NewGlobalRef (static_cast<jobject> (context));
        JuceActivityWatcher::getInstance();

#if JUCE_MODULE_AVAILABLE_juce_events && JUCE_ANDROID
        juce_juceEventsAndroidStartApp();
#endif
    }
}

//==============================================================================
LocalRef<jobject> getAppContext() noexcept
{
    auto* env = getEnv();
    auto context = androidApkContext;

    // You did not call Thread::initialiseJUCE which must be called at least once in your apk
    // before using any JUCE APIs. The Projucer will automatically generate java code
    // which will invoke Thread::initialiseJUCE for you.
    jassert (env != nullptr && context != nullptr);

    if (context == nullptr)
        return LocalRef<jobject>();

    if (env->IsInstanceOf (context, AndroidApplication) != 0)
        return LocalRef<jobject> (env->NewLocalRef (context));

    LocalRef<jobject> applicationContext (env->CallObjectMethod (context, AndroidContext.getApplicationContext));

    if (applicationContext == nullptr)
        return LocalRef<jobject> (env->NewLocalRef (context));

    return applicationContext;
}

LocalRef<jobject> getCurrentActivity() noexcept
{
    return JuceActivityWatcher::getInstance().getCurrent();
}

LocalRef<jobject> getMainActivity() noexcept
{
    return JuceActivityWatcher::getInstance().getMain();
}

//==============================================================================
using RealtimeThreadFactory = pthread_t (*) (void* (*entry) (void*), void* userPtr);
// This is defined in the juce_audio_devices module, with different definitions depending on
// whether OpenSL/Oboe are enabled.
RealtimeThreadFactory getAndroidRealtimeThreadFactory();

#if ! JUCE_MODULE_AVAILABLE_juce_audio_devices
RealtimeThreadFactory getAndroidRealtimeThreadFactory()
{
    return nullptr;
}
#endif

extern JavaVM* androidJNIJavaVM;

static auto setPriorityOfThisThread (Thread::Priority p)
{
    return setpriority (PRIO_PROCESS,
                        (id_t) gettid(),
                        ThreadPriorities::getNativePriority (p))
        == 0;
}

bool Thread::createNativeThread (Priority)
{
    const auto threadEntryProc = [] (void* userData) -> void*
    {
        auto* myself = static_cast<Thread*> (userData);

        setPriorityOfThisThread (myself->priority);

        juce_threadEntryPoint (myself);

        if (androidJNIJavaVM != nullptr)
        {
            void* env = nullptr;
            androidJNIJavaVM->GetEnv (&env, JNI_VERSION_1_2);

            // only detach if we have actually been attached
            if (env != nullptr)
                androidJNIJavaVM->DetachCurrentThread();
        }

        return nullptr;
    };

    if (isRealtime())
    {
        if (const auto factory = getAndroidRealtimeThreadFactory())
        {
            threadHandle = (void*) factory (threadEntryProc, this);
            threadId = (ThreadID) threadHandle.load();
            return threadId != nullptr;
        }
        else
        {
            jassertfalse;
        }
    }

    PosixThreadAttribute attr { threadStackSize };
    threadId = threadHandle = makeThreadHandle (attr, this, threadEntryProc);

    return threadId != nullptr;
}

void Thread::killThread()
{
    if (threadHandle != nullptr)
        jassertfalse; // pthread_cancel not available!
}

Thread::Priority Thread::getPriority() const
{
    jassert (Thread::getCurrentThreadId() == getThreadId());

    const auto native = getpriority (PRIO_PROCESS, (id_t) gettid());
    return ThreadPriorities::getJucePriority (native);
}

bool Thread::setPriority (Priority priorityIn)
{
    jassert (Thread::getCurrentThreadId() == getThreadId());

    if (isRealtime())
        return false;

    const auto priorityToUse = priority = priorityIn;
    return setPriorityOfThisThread (priorityToUse) == 0;
}

//==============================================================================
JUCE_API void JUCE_CALLTYPE Process::setPriority (ProcessPriority) {}

JUCE_API bool JUCE_CALLTYPE juce_isRunningUnderDebugger() noexcept
{
    StringArray lines;
    File ("/proc/self/status").readLines (lines);

    for (int i = lines.size(); --i >= 0;) // (NB - it's important that this runs in reverse order)
        if (lines[i].upToFirstOccurrenceOf (":", false, false).trim().equalsIgnoreCase ("TracerPid"))
            return (lines[i].fromFirstOccurrenceOf (":", false, false).trim().getIntValue() > 0);

    return false;
}

JUCE_API void JUCE_CALLTYPE Process::raisePrivilege() {}

JUCE_API void JUCE_CALLTYPE Process::lowerPrivilege() {}

} // namespace juce
