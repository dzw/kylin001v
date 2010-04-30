#pragma once

namespace Kylin
{

	// run-time type information
	class BtRTTI
	{
	public:
		BtRTTI (const char* pcName, const BtRTTI* pkBaseRTTI, int cid)
			:m_pcName(pcName) ,m_pkBaseRTTI(pkBaseRTTI)	,m_classid(cid) {};

		const char* GetName() const {return m_pcName;}
		const BtRTTI* GetBaseRTTI() const {return m_pkBaseRTTI;}
		const int GetClassID() const {return m_classid;}
	protected:
		const char* m_pcName;
		const BtRTTI* m_pkBaseRTTI;
		int		m_classid;
	};

// insert in root class declaration
#define BtDeclareRootRTTI(classname) \
    public: \
        static const BtRTTI m_RTTI; \
        virtual const BtRTTI* GetRTTI() const {return &m_RTTI;} \
        static bool IsExactKindOf(const BtRTTI* pkRTTI, \
            const classname* pkObject) \
        { \
            if (!pkObject) \
            { \
                return false; \
            } \
            return pkObject->IsExactKindOf(pkRTTI); \
        } \
        bool IsExactKindOf(const BtRTTI* pkRTTI) const \
        { \
            return (GetRTTI() == pkRTTI); \
        } \
        static bool IsKindOf(const BtRTTI* pkRTTI, \
            const classname* pkObject) \
        { \
            if (!pkObject) \
            { \
                return false; \
            } \
            return pkObject->IsKindOf(pkRTTI); \
        } \
        bool IsKindOf(const BtRTTI* pkRTTI) const \
        { \
            const BtRTTI* pkTmp = GetRTTI(); \
            while (pkTmp) \
            { \
                if (pkTmp == pkRTTI) \
                { \
                    return true; \
                } \
                pkTmp = pkTmp->GetBaseRTTI(); \
            } \
            return false; \
        } \
        static void* DynamicCast(const BtRTTI* pkRTTI, \
            const classname* pkObject) \
        { \
            if (!pkObject) \
            { \
                return false; \
            } \
            return pkObject->DynamicCast(pkRTTI); \
        } \
        void* DynamicCast(const BtRTTI* pkRTTI) const \
        { \
            return (IsKindOf(pkRTTI) ? (void*) this : 0 ); \
        }

// insert in class declaration
#define BtDeclareRTTI \
    public: \
	const static BtRTTI m_RTTI; \
        virtual const BtRTTI* GetRTTI() const {return &m_RTTI;}

// insert in root class source file
#define BtImplementRootRTTI(rootclassname, classid) \
    const BtRTTI rootclassname::m_RTTI(#rootclassname, 0, classid)

// insert in class source file
#define BtImplementRTTI(classname, baseclassname, classid) \
	const BtRTTI classname::m_RTTI(#classname, &baseclassname::m_RTTI, classid);

// macros for run-time type testing
#define BtIsExactKindOf(classname, pkObject) \
    classname::IsExactKindOf(&classname::m_RTTI, pkObject)

#define BtIsKindOf(classname, pkObject) \
    classname::IsKindOf(&classname::m_RTTI, pkObject)

// macro for run-time type casting
#define BtStaticCast(classname, pkObject) \
    ((classname*) pkObject)

#define BtDynamicCast(classname, pkObject) \
    ((classname*) classname::DynamicCast(&classname::m_RTTI, pkObject))

}