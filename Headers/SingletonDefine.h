#ifndef __SINGLETON_DEFINE_H__
#define __SINGLETON_DEFINE_H__

#define NO_COPY(CLASS_NAME)						\
private:										\
	explicit CLASS_NAME(const CLASS_NAME&);		\
	CLASS_NAME& operator=(const CLASS_NAME&);

#define DECLARE_SINGLETON(CLASS_NAME)           \
NO_COPY(CLASS_NAME);							\
private:                                        \
    static CLASS_NAME* pInstance;               \
public:                                         \
    static CLASS_NAME* GetInstance();           \
    void DestroyInstance();


#define IMPLEMENT_SINGLETON(CLASS_NAME)         \
CLASS_NAME* CLASS_NAME::pInstance = nullptr;    \
                                                \
CLASS_NAME* CLASS_NAME::GetInstance(){          \
    if(nullptr == pInstance)                    \
        pInstance = new CLASS_NAME;             \
                                                \
    return pInstance;                           \
}                                               \
                                                \
void CLASS_NAME::DestroyInstance(){             \
    if(pInstance){                              \
        delete pInstance;                       \
        pInstance = nullptr;                    \
    }                                           \
}

#endif //! __SINGLETON_DEFINE_H__