<h1 align="center">
	<img width="300" src="https://raw.githubusercontent.com/EDI-Systems/G2T1_GeCov/master/logo.png" alt="logo">
</h1>

# EDI Embedded Coverage Testing Tool (ECOV)

Click **[HERE](README.md)** for English version.

&emsp;&emsp;**ECOV** 是一个超级简单的嵌入式代码白盒覆盖测试工具。它是为简单的嵌入式应用程序设计的。
- 容易在嵌入式应用上搭建测试环境
- 只使用串口外设和最少量的代码插入
- 近乎为0的执行开销 - 应用程序不会感知到这一点
- 很少的内存消耗。256字节内存就足够对一个2048行的源程序做覆盖测试了。
- 完全无需移植
- 能生成测试报告

&emsp;&emsp;本软件以MIT协议授权。

## 快速演示
### 安插测试标志
```C
>> 文件名: source.c 
/* 128字节对于128*8=1024行的程序就足够了 */
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

### 输出数据到串口
```C
int Count;
for(Count=0;Count<128*8;Count++)
{
    if(((Test_Array[Count/8])&(1<<(Count%8)))!=0)
    {
        Serial_Output(Count:1);
    }
}

>> 将串口输出日志保存并重命名为 source.txt
```

### 生成测试报告
```sh
./ECOV source.c MARKER source.txt report.txt
```

## 新手上路

&emsp;&emsp;将这些测试标志插入到你的代码之中，而且这些测试标志应当在对应的代码行被执行时置位测试数据数组中的相应位。在所有的测试代码都跑完之后，把那些命中过的代码行以 行号：命中次数 的格式输出到串口。然后将串口输出送给本公举，即可生成测试报告。这个工具仅能一次分析一个.C文件；如果你有好几个.C文件，那么你需要分开做测试。

## EDI 工程信息
&emsp;&emsp;天创 - 微光 - 微风 (G2T1 R2T1)
