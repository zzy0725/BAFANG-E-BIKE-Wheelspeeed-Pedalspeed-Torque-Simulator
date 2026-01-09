SET PATH=D:\Keil_v5\ARM\ARMCC\Bin;C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common;C:\Program Files (x86)\Common Files\Oracle\Java\javapath;C:\windows\system32;C:\windows;C:\windows\System32\Wbem;C:\windows\System32\WindowsPowerShell\v1.0\;C:\windows\System32\OpenSSH\;C:\Program Files (x86)\Windows Kits\8.1\Windows Performance Toolkit\;C:\Program Files\dotnet\;C:\Program Files (x86)\Microsoft SQL Server\100\Tools\Binn\;C:\Program Files (x86)\Microsoft SQL Server\100\DTS\Binn\;C:\VXIPNP\WINNT\BIN;C:\Program Files (x86)\IVI Foundation\VISA\WinNT\Bin\;C:\Program Files\IVI Foundation\VISA\Win64\Bin\;C:\Program Files\010 Editor;C:\Program Files (x86)\IVI Foundation\VISA\WinNT\Bin;C:\Program Files (x86)\Microsoft Visual Studio\Common\Tools\WinNT;C:\Program Files (x86)\Microsoft Visual Studio\Common\MSDev98\Bin;C:\Program Files (x86)\Microsoft Visual Studio\Common\Tools;C:\Program Files (x86)\Microsoft Visual Studio\VC98\bin;C:\Users\Administrator\AppData\Local\Microsoft\WindowsApps
SET CPU_TYPE=STM32F103RC
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=Target 1
SET CPU_CLOCK=0x007A1200
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\main.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\stm32f10x_it.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\system_stm32f10x.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\led.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\pwm.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\can.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\pwm_cap.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\key.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\watchdog.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\delay.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\sys.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\usart.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\core_cm3.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via "..\startup_stm32f10x_hd._ia"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\stm32f10x_gpio.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\stm32f10x_rcc.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\stm32f10x_usart.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\misc.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\stm32f10x_tim.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\stm32f10x_can.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\stm32f10x_iwdg.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via "..\PWM.lnp"
"D:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" "..\PWM.axf" --i32combined --output "..\PWM.hex"
