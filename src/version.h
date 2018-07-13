#pragma once

#define _BRL_CONCAT(x,y) x ## y
#define BRL_CONCAT(x,y) _BRL_CONCAT(x,y)
#define _BRL_STR(x) #x
#define BRL_STR(x) _BRL_STR(x)

#if defined(_MSC_VER)
	//使用微软编译器
	#define BRL_COMP_NAME MSVC

	//判断架构
	#if defined(_M_ALPHA)
		#define BRL_ARCH ALPHA
	#elif defined(_M_AMD64) || defined(_M_X64)
		#define BRL_ARCH AMD64
	#elif defined(_M_ARM) || defined(_M_ARMT)
		#define BRL_ARCH BRL_CONCAT(ARMv, _M_ARM)
	#elif defined(__aarch64__)
		#define BRL_ARCH ARM64
	#elif defined(_M_IX86)
		#define BRL_ARCH x86
	#elif defined(_M_IA64)
		#define BRL_ARCH IA64
	#elif defined(_M_PPC)
		#define BRL_ARCH PowerPC
	#else
		#define BRL_ARCH unknown
	#endif

	//生成版本
	#if _MSC_VER < 1900
		#define BRL_COMP_VERSION ((_MSC_VER / 100) - 6) << "." << (_MSC_VER / 10 % 10)
	#else
		#define BRL_COMP_VERSION ((_MSC_VER / 100) - 5) << "." << (_MSC_VER / 10 % 10)
	#endif
#elif defined(__GNUC__)
	//使用GCC编译器
	#define BRL_COMP_NAME GCC

	//判断架构
	#if defined(__alpha__)
		#define BRL_ARCH ALPHA
	#elif defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
		#define BRL_ARCH AMD64
	#elif defined(__arm__) || defined(__thumb__)
		#define BRL_ARCH ARM
	#elif defined(__aarch64__)
		#define BRL_ARCH ARM64
	#elif defined(i386) || defined(__i386) || defined(__i386__)
		#define BRL_ARCH x86
	#elif defined(__ia64__) || defined(_IA64) || defined(__IA64__)
		#define BRL_ARCH IA64
	#elif defined(__mips__) || defined(mips)
		#define BRL_ARCH MIPS
	#elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) || defined(__POWERPC__) || defined(__ppc__) || defined(__ppc64__) || defined(__PPC__) || defined(__PPC64__) || defined(_ARCH_PPC) || defined(_ARCH_PPC64)
		#define BRL_ARCH PowerPC
	#else
		#define BRL_ARCH unknown
	#endif

	//生成版本
	#define BRL_COMP_VERSION (__GNUC__) << "." << (__GNUC_MINOR__) << "." << (__GNUC_PATCHLEVEL__)
#elif defined(__CC_ARM)
	#define BRL_COMP_NAME ARMCompiler
	#define BRL_COMP_VERSION (__ARMCC_VERSION / 100000) << "." << ((__ARMCC_VERSION / 10000) % 10)
	#define BRL_ARCH unknown
#elif defined(__clang__)
	#define BRL_COMP_NAME Clang
	#define BRL_COMP_VERSION (__clang_major__) << "." << (__clang_minor__)
	#define BRL_ARCH unknown
#elif defined(__ICL)
	#define BRL_COMP_NAME IntelComplier
	#define BRL_COMP_VERSION (__INTEL_COMPILER / 100) << "." << (__INTEL_COMPILER / 100 % 10) << "." << (__INTEL_COMPILER % 10)
	#define BRL_ARCH unknown
#elif defined(__llvm__)
	#define BRL_COMP_NAME LLVM
	#define BRL_COMP_VERSION "unk"
	#define BRL_ARCH unknown
#elif defined(__MINGW32__)
	#define BRL_COMP_NAME MinGW
	#define BRL_COMP_VERSION (__MINGW32_MAJOR_VERSION) << "." << (__MINGW32_MINOR_VERSION)
	#define BRL_ARCH unknown
#elif defined(__MINGW64__)
	#define BRL_COMP_NAME MinGWw64
	#define BRL_COMP_VERSION (__MINGW64_VERSION_MAJOR) << "." << (__MINGW64_VERSION_MINOR)
	#define BRL_ARCH unknown
#else
	#define BRL_COMP_NAME UNK
	#define BRL_COMP_VERSION ""
	#define BRL_ARCH unknown
#endif

//操作系统
#if defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
	#ifdef __LP64__
		#define BRL_OS Darwin64
	#else
		#define BRL_OS Darwin32
	#endif
#elif defined(ANDROID)
	#define BRL_OS Android
#elif defined(__CYGWIN__)
	#define BRL_OS Cygwin
#elif !defined(SAG_COM) && (!defined(WINAPI_FAMILY) || WINAPI_FAMILY==WINAPI_FAMILY_DESKTOP_APP) && (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
	#define BRL_OS Win64
#elif !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
	#if defined(WINCE) || defined(_WIN32_WCE)
		#define BRL_OS WinCE
	#elif defined(WINAPI_FAMILY)
		#if defined(WINAPI_FAMILY_PHONE_APP) && WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
			#define BRL_OS WinPhone
		#elif WINAPI_FAMILY==WINAPI_FAMILY_APP
			#define BRL_OS WinRT
		#else
			#define BRL_OS Win32
		#endif
	#else
		#define BRL_OS Win32
	#endif
#elif defined(__sun) || defined(sun)
	#define BRL_OS Solaris
#elif defined(hpux) || defined(__hpux)
	#define BRL_OS HPUX
#elif defined(__ultrix) || defined(ultrix)
	#define BRL_OS ULTRIX
#elif defined(sinix)
	#define BRL_OS RELIANT
#elif defined(__native_client__)
	#define BRL_OS NACL
#elif defined(__linux__) || defined(__linux)
	#define BRL_OS Linux
#elif defined(__FreeBSD__) || defined(__DragonFly__) || defined(__FreeBSD_kernel__)
	#define BRL_OS FreeBSD
#elif defined(__NetBSD__)
	#define BRL_OS NetBSD
#elif defined(__OpenBSD__)
	#define BRL_OS OpenBSD
#elif defined(__bsdi__)
	#define BRL_OS BSDI
#elif defined(__sgi)
	#define BRL_OS IRIX
#elif defined(__osf__)
	#define BRL_OS OSF
#elif defined(_AIX)
#define BRL_OS AIX
#elif defined(__Lynx__)
#define BRL_OS Lynx
#elif defined(__GNU__)
#define BRL_OS HURD
#elif defined(__DGUX__)
#define BRL_OS DGUX
#elif defined(__QNXNTO__)
#define BRL_OS QNX
#elif defined(_SEQUENT_)
#define BRL_OS DYNIX
#elif defined(_SCO_DS) /* SCO OpenServer 5 + GCC */
#define BRL_OS SCO
#elif defined(__USLC__) /* all SCO platforms + UDK or OUDK */
#define BRL_OS UNIXWARE
#elif defined(__svr4__) && defined(i386) /* Open UNIX 8 + GCC */
#define BRL_OS UNIXWARE
#elif defined(__INTEGRITY)
#define BRL_OS INTEGRITY
#elif defined(VXWORKS) /* there is no "real" VxWorks define - this has to be set in the mkspec! */
#define BRL_OS VXWORKS
#elif defined(__HAIKU__)
#define BRL_OS HAIKU
#else
#define BRL_OS UNKNOWN
#endif