# IBDP External Assessment - Computer Science

The structure of this README is based on the book [Extended Essay - Course Companion](Extended%20Essay%20-%20Course%20Companion%20by%20Kosta%20Lekanides%20(z-lib.org).pdf).

## Research Question

> **V0.0.1** (Deprecated)<br>
> To what extent does Linux x86_64 instructions have equivalents in Fedora Linux aarch64, and what’s its implication on instruction translation technology?

> V0.1.0 (Dev/Latest)<br>
> What’s the feasibility of a TIKTAG gadget compromising memory security against MTE in an Apple M1 Armv8.5 Linux machine?

## Feasibility Check

| Checklist | Tick |
| --- | --- |
| 1. Are there sufficient primary sources available (if appropriate)? | V |
| 2. Are there sufficient secondary sources available (if appropriate)? | V |
| 3. Can you access the sources in your location? | V |
| 4. Do you have all the materials necessary to carry out your investigation at hand? | V |
| 6. Can you access the materials and equipment in your location (that is, have you gained necessary permissions)? | V |
| 7. Can you begin your research immediately (for example, you do not need to defer your research until the summer when you will visit X or Y place)? | V |
| 8. Your research question (or title) can be assessed against the Extended Essay criteria? |  |
| 9. The chosen research methods or concepts underpinning your research question are relevant and appropriate to the subject? | V |
| 10. Does your research meet all of the IB's ethical guidelines on research and fieldwork? | V |

## Research Environment

1. Self-made TikTag 'gadget' (See `src/gadget.c`)
2. Fedora Linux Asahi Remix 39 aarch64 @ Apple MacBook Air (M1, 2020)
3. Kernel: 6.8.10-400.asahi.fc39.aarch64+16k
4. CPU: Icestorm-M1 (8) @ 2.06 GHz
5. `qemu-user`, specifically `qemu-aarch64` (8.1.3-5.fc39)

## Locating, Organizing, and Evaluating

Essays in computer science can be based on secondary source material entirely.
A large number of such sources should be used where possible with the proviso that the most up-to-date material is found in every instance.
Primary source material, in the form of programme runs or statistical charts, can also be used provided they are reliably constructed and relevant to the question.

### Types of Source

All of these resources are immediately available. Just take a few web search!

- Previous works, research, and exploits on speculative execution
- MTE documentation

### Locating Relevant Sources

- Technical Documentation:
  - [Official ARM Architecture Guides](https://www.arm.com/architecture/learn-the-architecture)
  - [Official Intel 64 and IA-32 Architecture Software Developer Manuals](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html)
- Source Code:
  - [Linux Kernel Source Code Tree](https://github.com/torvalds/linux)
  - [Github Mirror of QEMU Source Code](https://github.com/qemu/qemu)

### Organizing Source Material

| Source | Info \[or Notes\] | Topic Headings |
| ------ | ----------------- | -------------- |
| ${}TYPEOFSOUCE}: ${LASTNAME}, ${FIRSTNAMEINITIAL} [${YEAR}]. ${TITLE} [${MONTH} ${DAYS}, ${YEAR}]. ${PUBLISHERASONEWORD}: ${PUBLISHERFULL} | ${PARAGRAPH} [pp.${PAGENUMBERS}]<br>${COMMENT} | -${TOPIC} [${DETAILS}] |

### Organizing Chapter or Paragraphs: Branches

RQ: To what extent does Linux x86_64 instructions have equivalents in Fedora Linux aarch64, and what’s its implication on instruction translation technology?

1. Chapter 1: Introduction to Linux, x86_64, and aarch64 Architecture
   - Linux and Open Source
   - Research environment
   - Why?
2. Chapter 2: History and Past x86_64 -> aarch64 Transitions
   - Intel/AMD architecture
   - ARM versions: ARMv7 until ARMv9
   - Apple's Proprietary Rosetta 2 Translation Software
3. Chapter 3: Challenges in Translation Technologies
   - Lack of Developer Supports
   - Performance Issues
4. Chapter 4: Cons and Pros of Various Translation Software
   - [QEMU](https://github.com/qemu/qemu)
   - [VMWare](https://github.com/vmware)
   - [box64](https://github.com/ptitSeb/box64)
   - [xbyak_translator_aarch64](https://github.com/fujitsu/xbyak_translator_aarch64)

### Value and Limitations

<!-- Be aware of the limitations and shortcomings of methodological approach-->

| Source | Value | Limitations |
| ------ | ----- | ----------- |
|  | Objective,Insightful,RevealWeakness,Accurate,Credible,InPeriod,FirstHand,Uncensored,WideContext,IndicateEffects,ExpertPerspective,Correlations,Standarized,ConsiderVariables,Validated,Qualitative,IRL,Timeless | Bias,Swayful,OneSided,Exaggerating,NotInPeriod,NotInPlace,Unvalidated,RestrainedPolitically,NonSpecialist,Sarcasm,Dissenting,PartialUnderstanding,Limited,NoVariables,NotTestedIRL,OutOfDate |

### Evaluating Methodology

- Benchmark Analysis
  The CPU, memory, and file I/O performance is benchmarked for both the emulated and bare-metal x86_64, using [`sysbench`](https://github.com/akopytov/sysbench) by running a binary from the same source code compiled for both system.
  (Initial Research) From looking at the [issues in the Github](https://github.com/akopytov/sysbench/issues), there are some problems related to the CPU benchmarking and MySQL/MariaDB
  In order to achieve reliable results, the software of tested machines: operating system and software versions are the same.
- Binary Translation Framework Analysis
  Focusing on performance overhead, emulate x86_64 on an aarch64 system and comparing it's performance and correctness to native x86_64.
	Translation frameworks performance are different. Some might be faster on a specific case but slower on another.
	The sources are from own experiments, but (TODO: use better word) 'unprofessional' researcher might makes mistake that impact the experiments data.
	The base performance Mac M1 might be higher than (TODO: rename) x86_86 system. Some translations might not work or the instruction does not have equivalents.
- Comparative Analysis
  Determine equivalent for each instruction in a representative set of instructions from x86_64 and aarch64 that is commonly used in Linux, while also analyze the differences in instruction semantics, operand types, and encoding between the two ISAs and Identify instructions that have no direct equivalents and require more complex translation or emulation.
	The documentation of x86_64 and aarch64 is extensive and exhaustive, but the different versions of x86_64/aarch64 ISA will impact the result. Might be time consuming, and is only static analysis which disregard performance implications.
	The source is first-hand from documentations in the official x86_64 and aarch64 website.
- Literature Review and Analyses [1](https://bodhibloom.com/aarch64-vs-x86/) [2](https://paracr4ckbeginnings.wordpress.com/2014/10/03/comparing-x86_64-and-aarch64-assembly-which-do-you-prefer/) [3](https://itsfoss.com/arm-aarch64-x86_64/j) [4](https://www.redhat.com/en/topics/linux/ARM-vs-x86)
- Case studies: 

## Testing the Gadget (Linux)

Install the meta package which includes software tools for building the example C program, e.g.:

```
sudo apt install build-essential -y
```

Please check if your processors support MTE.
Read `/boot/config-` file and see if the `CONFIG_ARM64_AS_HAS_MTE` and `CONFIG_ARM64_MTE=y` option is set to `y`.
Install `qemu-user` to run the example on processors which do not support MTE:

```
sudo apt install qemu-user -y
```

Compile the gadget using `gcc`:

```
gcc src/gadget.c -o gadget -march=armv8.5-a+memtag
```

Run the gadget using `qemu-aarch64`, the userspace program which includes support for MTE:

```
qemu-aarch64 ./gadget
```

> [!NOTE]
> For more information about how to identify hardware features refer to [ARM64 ELF hwcaps](https://docs.kernel.org/arch/arm64/elf_hwcaps.html).
