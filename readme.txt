2023/12/26 9:20:42 systick定时器正常!并且在些基础上自实现定时消息正常(通过GPIO PC.7/PD.1输出方波验证无误)
2023/12/26 10:57:39 确认uart1/uart2收发正常
2023/12/26 17:09:27 添加Tim定时器(Capture1/Compare1)设置8KHz(通过GPIO PD.1输出方波验证无误)
2023/12/27 14:47:48 语音IC控制流程编码完成！待验证
2024/1/4 17:10:42 修改定时器tim2定时参数(约240um); 语音控制调整为idle-L:11ms, 240um:720um
2024/1/5 15:49:32 action队列工作正常(从前天到现在，工作不正常通过keil的调试断点功能发现队列很快就满了，并且队列中的timer成员值不正常，再次检查timer初始传入的值，发现传入的值超出范围！修改后问题解决)
2024/1/5 16:49:18 通过飞线验证，播放语音正常
2024/1/8 9:15:25 重新检查接入SPK后发现语音播放正常(注意供电功率)
2024/1/8 19:12:28 接收json报文调试完成，待联机验证
2024/1/9 13:18:16 通过uart1串口数据播放相应语音，验证正常
2024/1/10 10:54:12 修改串口波特率(9600->115200); 添加初始化流程
2024/1/10 19:40:12 用新框架发送485数据(完成编码待验证)
2024/1/11 10:47:00 用新板子(20231223)上电播放第一条，播不出来！但用demo板飞线与20230417主板上的语音(芯片飞线对接后)能正常播放！原因何在！！！
2024/1/11 13:56:10 语音控制通过先开启电源，(100ms后)再放语音，播放正常
2024/1/11 14:08:46 确认改造后的485数据发送正常(上电后第一条正常; 定时连续发送也正常; 可认为昨天的备份版本是正常的)！
2024/1/11 14:54:23 验证rs485收发数据正常(发数据前至少2个中断时长RE/DE高电平，实际延时在1-2个中断时长)
2024/1/12 8:44:27 补充昨天修正记录：发送后有回应，但发声不正常原因是清除过程中清除了定时器，没以清除语音播放标志，导致语音一直在播放中，不能结束(实际已经播放完成，没有发出完成的信号)
2024/1/12 18:25:36 修改485接收处理(完成后直接将数据取出); 小于100的序号作为检验边界; 485发送接口在FIFO上包装; 
2024/1/12 18:47:02 以uart1接收超时作为485接收的判定依据
2024/1/12 19:03:31 注释掉UART1中的判断代码
2024/1/14 15:56:17 换成中断发送方式(同Demo亦同zjf代码)；仍有数据接收不完整发生的情况; 又烧了个板子！！！
2024/1/14 17:46:00 用Demo板通过两个485调试板与PC串口对接，收发还是有乱码(这个是没有485调试板的DE/RE控制的)！！！能说明485调试板也不可靠么 ？

2024/1/24 17:36:30 语音定时时长改为6秒(原来为3秒)
2024/1/25 9:08:52 去掉开机播放的语音
2024/2/1 8:35:21 去掉音量设置部分
2024/2/1 9:06:40 修改声音控制报文及处理实现：
    1.报文中添加序列号！
    2.命令用序列号区分：如果与上次收到的报文序列号相同，认为是重复命令则只发应答，不执行；否则要应答并执行
2024/2/2 10:19:25 初步验证新报文处理收发正常
2024/3/19 17:54:15 接收485/uart1数据时小优化
2024/3/20 10:08:23 加IWDG功能使死机(跑飞)自恢复；通过配置为：初始化时PC05翻转，主循环中不翻转；在主循环中喂狗时正常运行，与不喂狗时，测得PC05每250ms一组脉冲信号，验证IWDG生效且运行正常！
2024/3/21 13:20:24 修改上电时版本下发过程及内容！增版本查询应答接口！
2024/3/25 17:29:12 与wifi模组通信的部分移过来，编译成功；后续再调！
2024/4/2 11:04:45 重新确认uart2(与wifi模块的通信接口，注意Tx、Rx交叉接线)工作正常
2024/4/3 14:34:06 发现sm_sendData_once()中的sprintf()将数字转换成ASCII字符串故障(其它位置也有用)！未查明原因！sm_receiveData()也故障，用自己实现的digit2ascii()函数替换后，正常！持续观察！
       代码区code+只读内存区RO-data已濒临空间上限！！！！！！
2024/4/10 17:23:03 功能完成，待验证; 通过查表识别命令的方式规避ROM空间不足的问题！没有通过优化actionFIFO功能规避ROM空间不足问题
2024/4/19 13:32:24 定位并解决uart2接收数据处理异常的问题(原因：uart2接收数据速率/处理器处理数据速率/缓冲区取数据方式，三者共同影响导致的问题!)
                   目前的结论是接收uart2数据的速度超过了识别处理关键字的速度，造成数据识别积压(识别关键字过程中又收到多个字节，识别关键字完成后取出的下一个字节不是紧接着的一个，而是取新的)，进而导致识别数据出错!
2024/4/23 9:56:20 网络数据查询及上报功能(可靠性待完善50%正确率)
                  通过 修正 offset = u8FIFOlength(&g_uart2RxQue) - u8FIFOlength2(&g_uart2RxQue); netInfo信息接收处理大幅提升正确率(目前测试结果全部正确！注意测试数据的正确性)
2024/5/13 10:16:13 修改DCDC开关逻辑：播放时才打开 ==> 常开[MVopPower_off()改为常开]
2024/5/30 9:08:31 为兼容与后面wifi模组通信，计划修改端口使用：
           改前原端口规划使用：
             uart1: PA3(Tx)/PA2(Rx)/PB4(485方向)
             uart2: PC3(Tx)/PC4(Rx)/PC5(W_en)
             voice: PD2(DIO)/PD3(PWR)
             IN_K11:PC6
           改后端口规划使用：
             uart1: PA3(Tx)/PD6(Rx)/PB4(485方向)
             uart2: PC5(Tx)/PC6(Rx)/PD1(W_en)
             voice: PD2(DIO)/PD3(BUSY)
             INK11: PC7
             从以上修改对比得到: 只要修改uart1的Rx(修改GPIO_Configuration4uart函数限可); uart2的需要全新修改(修改GPIO_Configuration4uart函数限可); voice引脚没有变化(PD3改为?输入); 
             其它可能影响的调试用接口：GPIO_init4led()；GPIO_PC06_K11INPUT();RCC_Configuration4uart() 
           1. 由于此版本放弃原计划的与wifi模组的通信，此软件版本理论上只要修改uart1的Rx使用即可满足要求！
           2. 出于验证PCB板的可用性，实现uart2只用于验证板子走线！
           待验证！！！
2024/6/3 14:21:02 新板子上验证正常(485发信号，播放语音正常)
             