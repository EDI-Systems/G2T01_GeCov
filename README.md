<h1 align="center">
	<img width="300" src="https://raw.githubusercontent.com/EDI-Systems/G2T1_GeCov/master/logo.png" alt="logo">
</h1>

# EDI Embedded Coverage Testing Tool (ECOV)

点击 **[这里](README_CN.md)** 查看中文版。

&ensp;&ensp;&ensp;&ensp;**ECOV** is a super-simple embedded code coverage tool meant for microcontrollers and simple projects.
- Easy to set up for embedded applications
- Minimal instrumentation, only serial port needed
- Minimal overhead for application being instrumented - no perceivable overhead
- Little memory overhead, 256B memory sufficient for 2048 lines of code
- Completely free from porting
- Coverage report generation

&ensp;&ensp;&ensp;&ensp;This software is licensed under the MIT license.

## Quick Demo
### Marker Installation
```C
>> Filename: source.c 
/* This is sufficient for 128*8=1024 lines of code */
unsigned char Test_Array[128]={0};
#define MARKER() \
do \
{ \
    Test_Array[__LINE__/8]|=1<<(__LINE__%8);\
} \
while(0)

...
if(Condition==TRUE)
    MARKER();
else
    MARKER();
...
```

### Data Output to Serial 
```C
int Count;
for(Count=0;Count<128*8;Count++)
{
    if(((Test_Array[Count/8])&(1<<(Count%8)))!=0)
    {
        Serial_Output(Count:1);
    }
}

>> Save serial log and rename as source.txt
```

### Report Generation 
```sh
./ECOV source.c MARKER source.txt report.txt
```

## Getting Started

&ensp;&ensp;&ensp;&ensp;Add markers into your code to be instrumented, and the marker should mark a bit in the coverage data array after the corresponding line gets executed. Then, after all tests are finished, print all the markers that are hit to serial, in the format line_number:hit_times. Then feed this serial log into the ECOV tool, and you will get a coverage report. This tool only works on a single .C file; if you ave multiple files, you need to do coverage test separately. 

## EDI Project Information
&ensp;&ensp;&ensp;&ensp;Genesis - Twilight - Breeze (G2T1 R2T1)
