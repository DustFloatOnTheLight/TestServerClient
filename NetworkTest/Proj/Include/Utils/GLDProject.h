#ifndef GLDPROJECT_H
#define GLDPROJECT_H


#if defined(GLDPROJECT_LIBRARY)
    #define GLDPROJECTSHARED_EXPORT Q_DECL_EXPORT
#else
    #if defined(GLDPROJECT_USE_SOURCE_CODE)
        #define GLDPROJECTSHARED_EXPORT
    #else
        #define GLDPROJECTSHARED_EXPORT Q_DECL_IMPORT
    #endif
#endif

#endif // GLDPROJECT_H
