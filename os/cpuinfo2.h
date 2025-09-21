#pragma once

#if _MSC_VER
#include <intrin.h> // __rdtsc
#endif

#include <assert.h>

namespace vml
{

	namespace os
	{

		//-----------------------------------------------------------------------------
		//	  return the current value of the TimeStampCounter (a counter of
		//	  CPU cycles since power-on, which is useful for high-resolution timing
		//	  but potentially differs between multiple CPUs)
		//	 
		//	  notes:
		//	  - a macro avoids call overhead, which is important for TIMER_ACCRUE.
		//	  - x64 RDTSC writes to edx:eax and clears the upper halves of rdx and rax.

		#if _MSC_VER
			static inline uint64_t rdtsc() { return __rdtsc(); }
		#else
			uint64_t rdtsc();
		#endif

		//-----------------------------------------------------------------------------

		#if defined(_MSC_FULL_VER) && _MSC_FULL_VER >= 150030729
			// VC10+ and VC9 SP1: __cpuidex is already available
		#elif GCC_VERSION
		# if defined(__i386__) && defined(__PIC__)
		#  define __cpuidex(regsArray, level, index)\
						  __asm__ __volatile__ ("pushl %%ebx\n"\
												"cpuid\n"\
												"mov %%ebx,%1\n"\
												"popl %%ebx"\
												: "=a" ((regsArray)[0]), "=r" ((regsArray)[1]), "=c" ((regsArray)[2]), "=d" ((regsArray)[3])\
												: "0" (level), "2" (index));
		# else
		#  define __cpuidex(regsArray, level, index)\
							__asm__ __volatile__ ("cpuid"\
												: "=a" ((regsArray)[0]), "=b" ((regsArray)[1]), "=c" ((regsArray)[2]), "=d" ((regsArray)[3])\
												: "0" (level), "2" (index));
		# endif
		#else
		# error "compiler not supported"
		#endif


		//-----------------------------------------------------------------------------

		class SystemInfo
		{
			
			private:

				//--------------------------------------------------------------------

				enum Features
				{
					HASSE3,
					HASPCLMULQDQ,
					HASDTES64,
					HASMONITOR,
					HASDSCPL,
					HASVMX,
					HASSMX,
					HASEST,
					HASTM2,
					HASSSE3,
					HASCNTXID,
					HASSDBG,
					HASFMA,
					HASCX16,
					HASXPTR,
					HASPDCM,
					BITRESERVED16,
					HASPCID,
					HASDCA,
					HASSSE4_1,
					HASSSE4_2,
					HASC2APIC,
					HASMOVBE,
					HASPOPCNT,
					HASTSC2,
					HASAES,
					HASXSAVE,
					HASOXSAVE,
					HASAVX,
					HASF16C,
					HASRDRND,
					HASHYP,

					HASFPU,
					HASVME,
					HASDE,
					HASPSE,
					HASTSC,
					HASMSR,
					HASPAE,
					HASMCE,
					HASCX8,
					HASAPIC,
					BITRESERVED10,
					HASSEP,
					HASMTRR,
					HASPGE,
					HASMCA,
					HASCMOV,
					HASPAT,
					HASPSE36,
					HASPSN,
					HASCLFSH,
					BITRESERVED20,
					HASDS,
					HASACPI,
					HASMMX,
					HASFXSR,
					HASSSE,
					HASSSE2,
					HASSS,
					HASHTT,
					HASTM,
					HASIA64,
					HASPBE,

					HASAHFSAHF,
					HASCMPLEGACY,
					HASSVM,
					HASEXTAPICSPACE,
					HASALTMOVCR8,
					HASLZCNT,
					HASSSE4A,
					HASMISSSE,
					HASPREFETCH,
					HASSKINITDEV,
					HASSYSCALLSYSRET,
					HASEXECDISBIT,
					HASFFXSR,
					HAS1GBSUPPORT,
					HASRDTSCP,
					HAS64,
					HAS3DNOWEXT,
					HAS3DNOW,
					HASNESTEDPAGING,
					HASLBRVISUALIZATION,
					HASFP128,
					HASMOVOPT,
					HASMULTITHREADING,

					UNUSED25,
					UNUSED26,
					UNUSED27,
					UNUSED28,
					UNUSED29,
					UNUSED30,
					UNUSED31,
					UNUSED32

				};

				std::string CpuFeaturesStrings[96] =
				{

					"Prescott New Instructions-SSE3 (PNI)",
					"PCLMULQDQ support",
					"64-bit debug store (edx bit 21)",
					"MONITOR and MWAIT instructions (SSE3)",
					"CPL qualified debug store",
					"Virtual Machine extensions",
					"Safer Mode Extensions (LaGrande)",
					"Enhanced SpeedStep",
					"Thermal Monitor 2",
					"Supplemental SSE3 instructions",
					"L1 Context ID",
					"Silicon Debug interface",
					"Fused multiply-add (FMA3)",
					"CMPXCHG16B instruction",
					"Can disable sending task priority messages",
					"Perfmon & debug capability",
					"Ecx Bit 16	is reserved",
					"Process context identifiers (CR4 bit 17)",
					"Direct cache access for DMA writes",
					"SSE4.1 instructions",
					"SSE4.2 instructions",
					"x2APIC support",
					"MOVBE instruction (big-endian)",
					"POPCNT instruction",
					"APIC supports one-shot operation using a TSC deadline value",
					"AES instruction set",
					"XSAVE, XRESTOR, XSETBV, XGETBV",
					"XSAVE enabled by OS",
					"Advanced Vector Extensions",
					"F16C (half-precision) FP support",
					"RDRAND (on-chip random number generator) support",
					"Running on a hypervisor (always 0 on a real CPU, but also with some hypervisors)",

					"On board x87 fpu",
					"Virtual 8086 mode extensions (such as VIF, VIP, PIV)",
					"Debugging extensions (CR4 bit 3)",
					"Page Size Extension monitor",
					"Time Stamp Counter",
					"Model-specific registers",
					"Physical Address Extension",
					"Machine Check Exception",
					"CMPXCHG8 (compare-and-swap) instruction",
					"Onboard advanced programmable interrupt controller",
					"Edx bit 10	is reserved",
					"SYSENTER and SYSEXIT instructions",
					"Memory Type Range Registers",
					"Page Global Enable bit in CR4",
					"Machine check architecture",
					"Conditional move and FCMOV instructions",
					"Page Attribute Table",
					"36-bit page size extension",
					"Processor Serial Number",
					"CLFLUSH instruction (SSE2)",
					"Edx bit 20	is reserved",
					"Debug store: save trace of executed jumps",
					"Onboard thermal control MSRs for ACPI",
					"MMX instructions",
					"FXSAVE, FXRESTOR instructions, CR4 bit 9",
					"SSE instructions",
					"SSE2 instructions",
					"CPU cache self-snoop",
					"Hyper-threading",
					"Thermal monitor",
					"IA64 processor emulating x86",
					"Pending Break Enable (PBE# pin) wakeup support",

					"LAHF and SAHF instructions",
					"CmpLegacy",
					"SVM",
					"ExtApicSpace",
					"AltMovCr8",
					"LZCNT",
					"SSE4A",
					"MisalignedSSE",
					"Prefetch",
					"SKINITandDEV",
					"SYSCALL and SYSRET",
					"Execute and disable bit",
					"MMX extensions",
					"FFXSR",
					"1GBSupport",
					"RDTSCP",
					"64",
					"3DNowExt",
					"3DNow",
					"Nested paging",
					"LBR visualization",
					"FP128",
					"MOV optimization",
					"Multithreading",

					"Unused25",
					"Unused26",
					"Unused27",
					"Unused28",
					"Unused29",
					"Unused30",
					"Unused31"

				};

				//-----------------------------------------------------------------------------
				//	 CPU vendor.
				//	 (this is exposed because some CPUID functions are vendor-specific.)
				//	 (an enum is easier to compare than the original string values.)

				enum Vendors
				{
					VENDOR_UNKNOWN,
					VENDOR_INTEL,
					VENDOR_AMD
				};

				//-----------------------------------------------------------------------------
				// registers used/returned by cpuid

				#pragma pack(push, 1)   // (allows casting to int*)
				struct CpuidRegs
				{
					uint32_t eax;
					uint32_t ebx;
					uint32_t ecx;
					uint32_t edx;
				};
				#pragma pack(pop)
				
				//-----------------------------------------------------------------------------

				uint32_t CpuIdMaxFunction			= 0;
				uint32_t CpuIdMaxExtendedFunction	= 0;
				uint32_t VendorId					= 0;
				uint32_t CodeNameId					= 0;
				uint32_t Family						= 0;
				uint32_t Model						= 0;
				uint32_t ProcessorType				= 0;
				uint32_t ExtendedModel				= 0;
				uint32_t ExtendedFamily				= 0;
				uint32_t Stepping					= 0;
				uint32_t BrandIndex					= 0;
				uint32_t CLFLUSHCacheLineSize		= 0;
				uint32_t LogicalProcessors			= 0;
				uint32_t APICPhysicalID				= 0;
				uint32_t CacheLineSize				= 0;
				uint32_t L2Associativity			= 0;
				uint32_t CacheSizeK					= 0;
				uint32_t PhysicalAddress			= 0;
				uint32_t VirtualAddress				= 0;
				double EstimatedCpuSpeed			= 0;
				std::string VendorString			= "Unknown";
				std::string CodeNameString			= "Unknown";
				std::string CPUBrandString			= "Unknown";

				int CpuFeaturesTable[96] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
											 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
											 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

				//-----------------------------------------------------------------------------
				//	 invoke CPUID instruction.
				//	 param regs input/output registers.
				//	 regs->eax must be set to the desired function.
				//	 some functions (e.g. 4) require regs->ecx to be set as well.
				//	 rationale: this interface (input/output structure vs. function parameters)
				//	 avoids unnecessary copying/initialization if some inputs aren't needed
				//	 and allows graceful expansion to functions that require further inputs.
				//	 return true on success or false if the sub-function isn't supported.

				void Invoke_cpuid(CpuidRegs* regs)
				{
					assert(sizeof(regs->eax) == sizeof(int));
					assert(sizeof(*regs) == 4 * sizeof(int));
					__cpuidex((int*)regs, regs->eax, regs->ecx);
				}
				
				//-----------------------------------------------------------------------------
				//

				void InitCpuid()
				{
					CpuidRegs regs = { 0 };
					regs.eax = 0;
					Invoke_cpuid(&regs);
					CpuIdMaxFunction = regs.eax;
					regs.eax = 0x80000000;
					Invoke_cpuid(&regs);
					CpuIdMaxExtendedFunction = regs.eax;
				}
				
				//-----------------------------------------------------------------------------
				//

				void InitCaps()
				{
					CpuidRegs regs = { 0 };
					uint32_t ecx, edx;

					regs.eax = 1;
					Invoke_cpuid(&regs);
					ecx = regs.ecx;
					edx = regs.edx;

					CpuFeaturesTable[HASSE3]		= (ecx &        0x1) != 0; // SSE3 instructions
					CpuFeaturesTable[HASPCLMULQDQ]	= (ecx &        0x2) != 0; // PCLMULQDQ support
					CpuFeaturesTable[HASDTES64]		= (ecx &        0x4) != 0; // 64-bit debug store(edx bit 21)
					CpuFeaturesTable[HASMONITOR]	= (ecx &        0x8) != 0; // MONITOR and MWAIT instructions(SSE3)
					CpuFeaturesTable[HASDSCPL]		= (ecx &       0x10) != 0; // CPL qualified debug store
					CpuFeaturesTable[HASVMX]		= (ecx &       0x20) != 0; // Virtual Machine eXtensions
					CpuFeaturesTable[HASSMX]		= (ecx &       0x40) != 0; // Safer Mode Extensions(LaGrande)
					CpuFeaturesTable[HASEST]		= (ecx &       0x80) != 0; // Enhanced SpeedStep
					CpuFeaturesTable[HASTM2]		= (ecx &      0x100) != 0; // Thermal Monitor 2
					CpuFeaturesTable[HASSSE3]		= (ecx &      0x200) != 0; // Supplemental SSE3 instructions
					CpuFeaturesTable[HASCNTXID]		= (ecx &      0x400) != 0; // L1 Context ID
					CpuFeaturesTable[HASSDBG]		= (ecx &      0x800) != 0; // Silicon Debug interface
					CpuFeaturesTable[HASFMA]		= (ecx &     0x1000) != 0; // Fused multiply - add(FMA3)
					CpuFeaturesTable[HASCX16]		= (ecx &     0x2000) != 0; // CMPXCHG16B instruction
					CpuFeaturesTable[HASXPTR]		= (ecx &     0x4000) != 0; // Can disable sending task priority messages
					CpuFeaturesTable[HASPDCM]		= (ecx &     0x8000) != 0; // Perfmon & debug capability
					CpuFeaturesTable[BITRESERVED16] = -1;			           // Bit 16 is reserved
					CpuFeaturesTable[HASPCID]		= (ecx &    0x20000) != 0; // Process context identifiers(CR4 bit 17)
					CpuFeaturesTable[HASDCA]		= (ecx &    0x40000) != 0; // Direct cache access for DMA writes
					CpuFeaturesTable[HASSSE4_1]		= (ecx &    0x80000) != 0; // SSE4.1 instructions
					CpuFeaturesTable[HASSSE4_2]		= (ecx &   0x100000) != 0; // SSE4.2 instructions
					CpuFeaturesTable[HASC2APIC]		= (ecx &   0x200000) != 0; // x2APIC support
					CpuFeaturesTable[HASMOVBE]		= (ecx &   0x400000) != 0; // MOVBE instruction(big - endian)
					CpuFeaturesTable[HASPOPCNT]		= (ecx &   0x800000) != 0; // POPCNT instruction
					CpuFeaturesTable[HASTSC2]		= (ecx &  0x1000000) != 0; // APIC supports one-shot operation using a TSC deadline value
					CpuFeaturesTable[HASAES]		= (ecx &  0x2000000) != 0; // AES instruction set
					CpuFeaturesTable[HASXSAVE]		= (ecx &  0x4000000) != 0; // XSAVE, XRESTOR, XSETBV, XGETBV
					CpuFeaturesTable[HASOXSAVE]		= (ecx &  0x8000000) != 0; // XSAVE enabled by OS
					CpuFeaturesTable[HASAVX]		= (ecx & 0x10000000) != 0; // Advanced Vector Extensions
					CpuFeaturesTable[HASF16C]		= (ecx & 0x20000000) != 0; // F16C(half-precision) FP support
					CpuFeaturesTable[HASRDRND]		= (ecx & 0x40000000) != 0; // RDRAND(on-chip random number generator) support
					CpuFeaturesTable[HASHYP]		= (ecx & 0x80000000) != 0; // Running on a hypervisor(always 0 on a real CPU, but also with some hypervisors)

					CpuFeaturesTable[HASFPU]		= (edx &	    0x1) != 0; // On board x87 fpu
					CpuFeaturesTable[HASVME]		= (edx &	    0x2) != 0; // Virtual 8086 mode extensions (such as VIF, VIP, PIV)	
					CpuFeaturesTable[HASDE]			= (edx &	    0x4) != 0; // Debugging extensions (CR4 bit 3)
					CpuFeaturesTable[HASPSE]		= (edx &	    0x8) != 0; // Page Size Extension	monitor
					CpuFeaturesTable[HASTSC]		= (edx &	   0x10) != 0; // Time Stamp Counter	
					CpuFeaturesTable[HASMSR]		= (edx &	   0x20) != 0; // Model-specific registers
					CpuFeaturesTable[HASPAE]		= (edx &	   0x40) != 0; // Physical Address Extension
					CpuFeaturesTable[HASMCE]		= (edx &	   0x80) != 0; // Machine Check Exception	
					CpuFeaturesTable[HASCX8]		= (edx &	  0x100) != 0; // CMPXCHG8 (compare-and-swap) instruction
					CpuFeaturesTable[HASAPIC]		= (edx &	  0x200) != 0; // Onboard Advanced Programmable Interrupt Controller
					CpuFeaturesTable[BITRESERVED10] = -1;					   // Bit 10 is reserved
					CpuFeaturesTable[HASSEP]		= (edx &      0x800) != 0; // SYSENTER and SYSEXIT instructions
					CpuFeaturesTable[HASMTRR]		= (edx &     0x1000) != 0; // Memory Type Range Registers
					CpuFeaturesTable[HASPGE]		= (edx &     0x2000) != 0; // Page Global Enable bit in CR4
					CpuFeaturesTable[HASMCA]		= (edx &     0x4000) != 0; // Machine check architecture
					CpuFeaturesTable[HASCMOV]		= (edx &     0x8000) != 0; // Conditional move and FCMOV instructions
					CpuFeaturesTable[HASPAT]		= (edx &    0x10000) != 0; // Page Attribute Table
					CpuFeaturesTable[HASPSE36]		= (edx &    0x20000) != 0; // 36-bit page size extension
					CpuFeaturesTable[HASPSN]		= (edx &    0x40000) != 0; // Processor Serial Number
					CpuFeaturesTable[HASCLFSH]		= (edx &    0x80000) != 0; // CLFLUSH instruction (SSE2)
					CpuFeaturesTable[BITRESERVED20] = -1;					   // Bit 20	is reserved
					CpuFeaturesTable[HASDS]			= (edx &   0x200000) != 0; // Debug store: save trace of executed jumps
					CpuFeaturesTable[HASACPI]		= (edx &   0x400000) != 0; // Onboard thermal control MSRs for ACPI
					CpuFeaturesTable[HASMMX]		= (edx &   0x800000) != 0; // MMX instructions
					CpuFeaturesTable[HASFXSR]		= (edx &  0x1000000) != 0; // FXSAVE, FXRESTOR instructions, CR4 bit 9
					CpuFeaturesTable[HASSSE]		= (edx &  0x2000000) != 0; // SSE instructions
					CpuFeaturesTable[HASSSE2]		= (edx &  0x4000000) != 0; // SSE2 instructions
					CpuFeaturesTable[HASSS]			= (edx &  0x8000000) != 0; // CPU cache supports self-snoop
					CpuFeaturesTable[HASHTT]		= (edx & 0x10000000) != 0; // Hyper-threading
					CpuFeaturesTable[HASTM]			= (edx & 0x20000000) != 0; // Thermal monitor
					CpuFeaturesTable[HASIA64]		= (edx & 0x40000000) != 0; // IA64 processor emulating x86
					CpuFeaturesTable[HASPBE]		= (edx & 0x80000000) != 0; // Pending Break Enable (PBE# pin) wakeup support

					regs.eax = 0x80000001;
					Invoke_cpuid(&regs);
					ecx = regs.ecx;
					edx = regs.edx;

					CpuFeaturesTable[HASAHFSAHF]		= (ecx &        0x1) != 0;
					CpuFeaturesTable[HASCMPLEGACY]		= (ecx &        0x2) != 0;
					CpuFeaturesTable[HASSVM]			= (ecx &        0x4) != 0;
					CpuFeaturesTable[HASEXTAPICSPACE]	= (ecx &        0x8) != 0;
					CpuFeaturesTable[HASALTMOVCR8]		= (ecx &       0x10) != 0;
					CpuFeaturesTable[HASLZCNT]			= (ecx &       0x20) != 0;
					CpuFeaturesTable[HASSSE4A]			= (ecx &       0x40) != 0;
					CpuFeaturesTable[HASMISSSE]			= (ecx &       0x80) != 0;
					CpuFeaturesTable[HASPREFETCH]		= (ecx &      0x100) != 0;
					CpuFeaturesTable[HASSKINITDEV]		= (ecx &     0x1000) != 0;

					CpuFeaturesTable[HASSYSCALLSYSRET]	= (edx &      0x800) != 0;
					CpuFeaturesTable[HASEXECDISBIT]		= (edx &    0x10000) != 0;
					CpuFeaturesTable[HASFFXSR]			= (edx &   0x200000) != 0;
					CpuFeaturesTable[HAS1GBSUPPORT]		= (edx &   0x400000) != 0;
					CpuFeaturesTable[HASRDTSCP]			= (edx &  0x8000000) != 0;
					CpuFeaturesTable[HAS64]				= (edx & 0x20000000) != 0;
					CpuFeaturesTable[HAS3DNOWEXT]		= (edx & 0x40000000) != 0;
					CpuFeaturesTable[HAS3DNOW]			= (edx & 0x80000000) != 0;

					// Interpret CPU brand string and cache information.

					if (CpuIdMaxExtendedFunction >= 0x80000004)
					{
						const char* ptr=nullptr;
						size_t count=0;
						char text[64] = { 0 };
						regs.eax = 0x80000002;
						Invoke_cpuid(&regs);
						ptr = (const char*)&regs.eax; 
						if (ptr[0]) text[count++] = ptr[0];
						if (ptr[1]) text[count++] = ptr[1];
						if (ptr[2]) text[count++] = ptr[2];
						if (ptr[3]) text[count++] = ptr[3];
						ptr = (const char*)&regs.ebx; 
						if (ptr[0]) text[count++] = ptr[0];
						if (ptr[1]) text[count++] = ptr[1];
						if (ptr[2]) text[count++] = ptr[2];
						if (ptr[3]) text[count++] = ptr[3];
						ptr = (const char*)&regs.ecx; 
						if (ptr[0]) text[count++] = ptr[0];
						if (ptr[1]) text[count++] = ptr[1];
						if (ptr[2]) text[count++] = ptr[2];
						if (ptr[3]) text[count++] = ptr[3];
						ptr = (const char*)&regs.edx; 
						if (ptr[0]) text[count++] = ptr[0];
						if (ptr[1]) text[count++] = ptr[1];
						if (ptr[2]) text[count++] = ptr[2];
						if (ptr[3]) text[count++] = ptr[3];
						
						regs.eax = 0x80000003;
						Invoke_cpuid(&regs);
						ptr = (const char*)&regs.eax; 
						if (ptr[0]) text[count++] = ptr[0];
						if (ptr[1]) text[count++] = ptr[1];
						if (ptr[2]) text[count++] = ptr[2];
						if (ptr[3]) text[count++] = ptr[3];
						ptr = (const char*)&regs.ebx; 
						if (ptr[0]) text[count++] = ptr[0];
						if (ptr[1]) text[count++] = ptr[1];
						if (ptr[2]) text[count++] = ptr[2];
						if (ptr[3]) text[count++] = ptr[3];
						ptr = (const char*)&regs.ecx;
						if (ptr[0]) text[count++] = ptr[0];
						if (ptr[1]) text[count++] = ptr[1];
						if (ptr[2]) text[count++] = ptr[2];
						if (ptr[3]) text[count++] = ptr[3];
						ptr = (const char*)&regs.edx; 
						if (ptr[0]) text[count++] = ptr[0];
						if (ptr[1]) text[count++] = ptr[1];
						if (ptr[2]) text[count++] = ptr[2];
						if (ptr[3]) text[count++] = ptr[3];

						regs.eax = 0x80000004;
						Invoke_cpuid(&regs);
						ptr = (const char*)&regs.eax; 
						if (ptr[0]) text[count++] = ptr[0];
						if (ptr[1]) text[count++] = ptr[1];
						if (ptr[2]) text[count++] = ptr[2];
						if (ptr[3]) text[count++] = ptr[3];
						ptr = (const char*)&regs.ebx; 
						if (ptr[0]) text[count++] = ptr[0];
						if (ptr[1]) text[count++] = ptr[1];
						if (ptr[2]) text[count++] = ptr[2];
						if (ptr[3]) text[count++] = ptr[3];
						ptr = (const char*)&regs.ecx; 
						if (ptr[0]) text[count++] = ptr[0];
						if (ptr[1]) text[count++] = ptr[1];
						if (ptr[2]) text[count++] = ptr[2];
						if (ptr[3]) text[count++] = ptr[3];
						ptr = (const char*)&regs.edx; 
						if (ptr[0]) text[count++] = ptr[0];
						if (ptr[1]) text[count++] = ptr[1];
						if (ptr[2]) text[count++] = ptr[2];
						if (ptr[3]) text[count++] = ptr[3];

						CPUBrandString = std::string ( text );
					}
					else
					{
						CPUBrandString = "Unknown";
					}

					// get cpu line cache

					regs.eax = 0x80000006;
					Invoke_cpuid(&regs);
					CacheLineSize = (regs.ecx & 0xff);
					L2Associativity = (regs.ecx >> 0xc) & 0xf;
					CacheSizeK = (regs.ecx >> 0x10) & 0xffff;

					// get cpu physical addresses

					regs.eax = 0x80000008;
					Invoke_cpuid(&regs);
					PhysicalAddress = (regs.eax & 0xff);
					VirtualAddress = (regs.eax >> 0x8) & 0xff;

					// get cpu paging

					regs.eax = 0x8000000A;
					Invoke_cpuid(&regs);
					CpuFeaturesTable[HASNESTEDPAGING] = (regs.edx & 0x1) != 0;
					CpuFeaturesTable[HASLBRVISUALIZATION] = (regs.edx & 0x2) != 0;

					// get cpu mov features

					regs.eax = 0x8000000A;
					Invoke_cpuid(&regs);
					CpuFeaturesTable[HASFP128] = (regs.eax & 0x1) != 0;
					CpuFeaturesTable[HASMOVOPT] = (regs.eax & 0x2) != 0;

				}

				//-----------------------------------------------------------------------------
				//
				
				bool InitVendor()
				{
					CpuidRegs regs = { 0 };

					regs.eax = 0;

					Invoke_cpuid(&regs);

					// copy regs to string
					// note: 'strange' ebx,edx,ecx reg order is due to ModR/M encoding order.

					VendorString = std::string((const char *)&regs.ebx, 4);
					VendorString += std::string((const char *)&regs.edx, 4);
					VendorString += std::string((const char *)&regs.ecx, 4);

					if (VendorString == "AuthenticAMD")
					{
						VendorId = VENDOR_AMD;
					}
					else if (VendorString == "GenuineIntel")
					{
						VendorId = VENDOR_INTEL;
					}
					else
					{
						VendorId = VENDOR_UNKNOWN;
						return false;
					}
					return true;
				}

				//-----------------------------------------------------------------------------
				//
				
				bool InitSignature()
				{
					CpuidRegs regs = { 0 };

					regs.eax = 1;
					Invoke_cpuid(&regs);

					Model = (regs.eax >> 4) & 0xf;
					ExtendedModel = (regs.eax >> 16) & 0xf;
					ExtendedFamily = (regs.eax >> 20) & 0xff;
					int familyId = (regs.eax >> 8) & 0xf;
					Family = familyId == 0xf ? ExtendedFamily + familyId : familyId;
					CodeNameId = (regs.eax >> 4);
					Stepping = (regs.eax) & 0xf;

					switch (CodeNameId)
					{
						case 0x506E:	CodeNameString = "Skylake";			break;
						case 0x406C:	CodeNameString = "CherryTrail";		break;
						case 0x306D:	CodeNameString = "Broadwell";		break;
						case 0x306C:	CodeNameString = "Haswell";			break;
						case 0x306A:	CodeNameString = "IvyBridge";		break;
						case 0x206A:
						case 0x206D:	CodeNameString = "SandyBridge";		break;
						case 0x2065:
						case 0x206C:
						case 0x206F:	CodeNameString = "Westmere";		break;
						case 0x106E:
						case 0x106A:
						case 0x206E:	CodeNameString = "Nehalem";			break;
						case 0x1067:
						case 0x106D:	CodeNameString = "Penryn";			break;
						case 0x006F:
						case 0x1066:	CodeNameString = "Merom";			break;
						case 0x0066:	CodeNameString = "Presler";			break;
						case 0x0063:
						case 0x0064:	CodeNameString = "Prescott";		break;
						case 0x006D:	CodeNameString = "Dothan";			break;
						case 0x0366:	CodeNameString = "Cedarview";		break;
						case 0x0266:	CodeNameString = "Lincroft";		break;
						case 0x016C:	CodeNameString = "Pineview";		break;
						default:		CodeNameString = "Unknwon";			break;
					}

					BrandIndex = regs.ebx & 0xff;
					CLFLUSHCacheLineSize = ((regs.ebx >> 8) & 0xff) * 8;
					LogicalProcessors = (regs.ebx >> 16) & 0xff;
					APICPhysicalID = (regs.ebx >> 24) & 0xff;

					CpuFeaturesTable[HASMULTITHREADING] = (regs.edx & (1 << 28)) != 0;

					return true;
				}
				
				//-----------------------------------------------------------------------------
				//

				void InitTimer()
				{
					LARGE_INTEGER s, e, freq;
					uint64_t cs, ce;

					// Determine timer frequency.

					QueryPerformanceFrequency(&freq);
					QueryPerformanceCounter(&s);
					cs = rdtsc();

					// Wait for a while...

					for (volatile long i = 0; i < 1000000; ++i);
					ce = rdtsc();

					QueryPerformanceCounter(&e);

					// Calculate frequency.

					EstimatedCpuSpeed = (unsigned long)((static_cast<double>(ce) - cs) * freq.QuadPart / (e.QuadPart - s.QuadPart));
				}

				
			public:
				
				//-----------------------------------------------------------------------------
				// start measuring services

				void Measure()
				{
					InitCpuid();
					InitVendor();
					InitSignature();
					InitCaps();
					InitTimer();
				}
				
				//-----------------------------------------------------------------------------
				// gets cpu infos

				const std::string &GetCpuVendorString() const
				{
					return VendorString;
				}

				const std::string &GetCpuCodeNameString() const
				{
					return CodeNameString;
				}

				const std::string &GetCpuBrandString() const
				{
					return CPUBrandString;
				}

				const uint32_t GetVendorId() const
				{
					if (CpuIdMaxFunction >= 1)
						return VendorId;
					return 0;
				}

				const uint32_t GetCodeNameId() const
				{
					if (CpuIdMaxFunction >= 1)
						return CodeNameId;
					return 0;
				}

				const uint32_t GetStepping() const
				{
					if (CpuIdMaxFunction >= 1)
						return Stepping;
					return 0;
				}

				const uint32_t GetFamily() const
				{
					if (CpuIdMaxFunction >= 1)
						return Family;
					return 0;
				}

				const uint32_t GetModel() const
				{
					if (CpuIdMaxFunction >= 1)
						return Model;
					return 0;
				}

				const uint32_t GetCPUIdMaxFunction() const
				{
					if (CpuIdMaxFunction >= 1)
						return CpuIdMaxFunction;
					return 0;
				}

				const uint32_t GetCPUIdMaxExtendedFunction() const
				{
					if (CpuIdMaxFunction >= 1)
						return CpuIdMaxExtendedFunction;
					return 0;
				}

				const uint32_t GetProcessorType() const
				{
					if (CpuIdMaxFunction >= 1)
						return ProcessorType;
					return 0;
				}

				const uint32_t GetExtendedModel() const
				{
					if (CpuIdMaxFunction >= 1)
						return ExtendedModel;
					return 0;
				}

				const uint32_t GetExtendedFamily() const
				{
					if (CpuIdMaxFunction >= 1)
						return ExtendedFamily;
					return 0;
				}

				const uint32_t GetBrandIndex() const
				{
					if (CpuIdMaxFunction >= 1)
						return BrandIndex;
					return 0;
				}

				const uint32_t GetCLFLUSHCacheLineSize() const
				{
					if (CpuIdMaxFunction >= 1)
						return CLFLUSHCacheLineSize;
					return 0;
				}

				const uint32_t GetAPICPhysicalID() const
				{
					if (CpuIdMaxFunction >= 1)
						return APICPhysicalID;
					return 0;
				}

				const uint32_t GetCacheLineSize() const
				{
					if (CpuIdMaxFunction >= 1)
						return CacheLineSize;
					return 0;
				}

				const uint32_t GetL2Associativity() const
				{
					if (CpuIdMaxFunction >= 1)
						return L2Associativity;
					return 0;
				}

				const uint32_t GetCacheSize() const
				{
					if (CpuIdMaxFunction >= 1)
						return CacheSizeK;
					return 0;
				}

				const uint32_t GetPhysicalAddress() const
				{
					if (CpuIdMaxFunction >= 1)
						return PhysicalAddress;
					return 0;
				}

				const uint32_t GetVirtualAddress() const
				{
					if (CpuIdMaxFunction >= 1)
						return VirtualAddress;
					return 0;
				}

				const uint32_t GetLogicalProcessorsNumber() const
				{
					if (CpuIdMaxFunction >= 1)
						if (CpuFeaturesTable[HASMULTITHREADING] && (LogicalProcessors > 0))
							return LogicalProcessors;
					return 0;
				}

				const std::string HasFeature(const unsigned int feature) const
				{
					if (CpuIdMaxFunction >= 1)
						if (feature >= HASSE3 && feature <= UNUSED32)
							return CpuFeaturesStrings[feature];
					return "Unknown feature";
				}

				const double GetCpuSpeed() const
				{
					return EstimatedCpuSpeed;
				}
		
				std::string ReportToString()
				{
					std::string output;

					// measure host system

					Measure();

					output =  "Vendor                      : ";	output += GetCpuVendorString();							output += "\n";
					output += "CodeName                    : ";	output += GetCpuCodeNameString();						output += "\n";
					output += "Brand                       : "; output += GetCpuBrandString();							output += "\n";
					output += "VendorId                    : ";	output += std::to_string(GetVendorId());				output += "\n";
					output += "CodeNameId                  : ";	output += std::to_string(GetCodeNameId());				output += "\n";
					output += "Stepping                    : ";	output += std::to_string(GetStepping());                output += "\n";
					output += "Family                      : ";	output += std::to_string(GetFamily());                  output += "\n";
					output += "Model                       : ";	output += std::to_string(GetModel());                   output += "\n";
					output += "CPUMaxFunction              : ";	output += std::to_string(GetCPUIdMaxFunction());        output += "\n";
					output += "Processr Type               : ";	output += std::to_string(GetProcessorType());           output += "\n";
					output += "Extended Model              : ";	output += std::to_string(GetExtendedModel());           output += "\n";
					output += "Extended Family             : ";	output += std::to_string(GetExtendedFamily());          output += "\n";
					output += "Brand Index                 : ";	output += std::to_string(GetBrandIndex());              output += "\n";
					output += "CLFLUSH Cache line size     : ";	output += std::to_string(GetCLFLUSHCacheLineSize());    output += "\n";
					output += "API Physical ID             : ";	output += std::to_string(GetAPICPhysicalID());          output += "\n";
					output += "Cache line size             : ";	output += std::to_string(GetCacheLineSize());           output += "\n";
					output += "L2 Associativity            : ";	output += std::to_string(GetL2Associativity());         output += "\n";
					output += "Cache size                  : ";	output += std::to_string(GetCacheSize());               output += "\n";
					output += "Physical address            : ";	output += std::to_string(GetPhysicalAddress());         output += "\n";
					output += "Virtual address             : ";	output += std::to_string(GetVirtualAddress());          output += "\n";
					output += "Logical processor(s) number : ";	output += std::to_string(GetLogicalProcessorsNumber()); output += "\n";
			
					output += "Feature Bit [ "; output += std::to_string( 0); output += "]           : ";	output += HasFeature( 0); output += "\n"; 
					output += "Feature Bit [ "; output += std::to_string( 1); output += "]           : ";	output += HasFeature( 1); output += "\n"; 
					output += "Feature Bit [ "; output += std::to_string( 2); output += "]           : ";	output += HasFeature( 2); output += "\n"; 
					output += "Feature Bit [ "; output += std::to_string( 3); output += "]           : ";	output += HasFeature( 3); output += "\n"; 
					output += "Feature Bit [ "; output += std::to_string( 4); output += "]           : ";	output += HasFeature( 4); output += "\n"; 
					output += "Feature Bit [ "; output += std::to_string( 5); output += "]           : ";	output += HasFeature( 5); output += "\n";
					output += "Feature Bit [ "; output += std::to_string( 6); output += "]           : ";	output += HasFeature( 6); output += "\n";
					output += "Feature Bit [ "; output += std::to_string( 7); output += "]           : ";	output += HasFeature( 7); output += "\n";
					output += "Feature Bit [ "; output += std::to_string( 8); output += "]           : ";	output += HasFeature( 8); output += "\n";
					output += "Feature Bit [ "; output += std::to_string( 9); output += "]           : ";	output += HasFeature( 9); output += "\n";
					output += "Feature Bit [ "; output += std::to_string(10); output += "]           : ";	output += HasFeature(10); output += "\n";
					output += "Feature Bit [ "; output += std::to_string(11); output += "]           : ";	output += HasFeature(11); output += "\n";
					output += "Feature Bit [ "; output += std::to_string(12); output += "]           : ";	output += HasFeature(12); output += "\n";
					output += "Feature Bit [ "; output += std::to_string(13); output += "]           : ";	output += HasFeature(13); output += "\n";
					output += "Feature Bit [ "; output += std::to_string(14); output += "]           : ";	output += HasFeature(14); output += "\n";
					output += "Feature Bit [ "; output += std::to_string(15); output += "]           : ";	output += HasFeature(15); output += "\n";
					output += "Feature Bit [ "; output += std::to_string(16); output += "]           : ";	output += HasFeature(16); output += "\n";
					output += "Feature Bit [ "; output += std::to_string(17); output += "]           : ";	output += HasFeature(17); output += "\n";
					output += "Feature Bit [ "; output += std::to_string(18); output += "]           : ";	output += HasFeature(18); output += "\n";
					output += "Feature Bit [ "; output += std::to_string(19); output += "]           : ";	output += HasFeature(19); output += "\n";
					output += "Feature Bit [ "; output += std::to_string(20); output += "]           : ";	output += HasFeature(20); output += "\n";
					output += "Feature Bit [ "; output += std::to_string(21); output += "]           : ";	output += HasFeature(21); output += "\n";
					output += "Feature Bit [ "; output += std::to_string(22); output += "]           : ";	output += HasFeature(22); output += "\n";
					output += "Feature Bit [ "; output += std::to_string(23); output += "]           : ";	output += HasFeature(23); output += "\n";
					output += "Feature Bit [ "; output += std::to_string(24); output += "]           : ";	output += HasFeature(24); output += "\n";
					output += "Feature Bit [ "; output += std::to_string(25); output += "]           : ";	output += HasFeature(25); output += "\n";
					output += "Feature Bit [ "; output += std::to_string(26); output += "]           : ";	output += HasFeature(26); output += "\n";
					output += "Feature Bit [ "; output += std::to_string(27); output += "]           : ";	output += HasFeature(27); output += "\n";
					output += "Feature Bit [ "; output += std::to_string(28); output += "]           : ";	output += HasFeature(28); output += "\n";
					output += "Feature Bit [ "; output += std::to_string(29); output += "]           : ";	output += HasFeature(29); output += "\n";
					output += "Feature Bit [ "; output += std::to_string(30); output += "]           : ";	output += HasFeature(30); output += "\n";
					output += "Feature Bit [ "; output += std::to_string(31); output += "]           : ";	output += HasFeature(31); output += "\n";
					
					
					return output;
				}
				
				//-----------------------------------------------------------------------------
				// ctor / dtor

				SystemInfo()
				{
				}

				~SystemInfo()
				{
				}

		};

	}   // namespace os

}   // namespace vml
