# BearPi

#### 介绍
用于存放小熊派相关实验例程，包括之前已经发表在微信公众号上的文章。

详细Demo文章讲解可关注微信公众号：嵌入式云IOT技术圈

#### 使用说明

- 01.SF1_Demo

  基于小熊派传感器拓展板例程

- 02.GUI

  存放小熊派移植GUI的例程，包括STemWin、lvgl、idea4good等

- 03.LIGHT_SENSOR_I2C

  基于小熊派光强传感器实验

- 04.Multi_Menu

  在Multi_Button的基础上改造为一个简易菜单框架

- 05.RTTnano_Bearpi

  移植rtt基础内核到小熊派上

- 06.Multi_timer

  移植开源软件定时器到小熊派上

- 07.Micro-Lab

  与凤媒电子开发的Micro-Lab进行通信实例

- 08.Smart_Car

  基于小熊派控制的智能小车案例

- 09.SD_FAT_FS_INI

  使用小熊派读取SD卡 INI文件例程，移植INI组件库等

- 10.ZKT_BearPi

  基于中控智慧身份证识别案例

- 11.LCD_ST7789_Chinese_Display

  基于小熊派中文字库显示案例

- 12.BearPi_Menu_Event(进度80%)

  基于小熊派菜单事件框架应用

  (1)UI界面

  主页面、测试页面、版本查看页面、上传页面(未实现)、菜单列表(未实现)

  (2)按键交互

  2个按键4种状态(左键短按、左键长按、右键短按、右键长按)(已实现)

  (3)检测逻辑

  实现烟感检测，大于600pm报警闪烁，小于600ppm && 检测时间>=10s，安全闪烁(已实现)

- 13.基于TOS气体传感器应用

- 14.LCD驱动框架应用

- 15.祝腾讯22周年生日快乐

- 16.传感器曲线显示例程

- 17.达特甲醛传感器驱动例程

- 18.淘宝购买的优库热敏打印模组例程和文档

- 19.小熊派上的QSPI FLASH-W25Q64适配Fatfs文件系统

- 20.小熊派上QSPI与SD卡文件系统同时挂载进行读写测试

- 21.小熊派在SD卡fatfs上将图片资源复制到QSPI的fatfs上，调用api显示QSPI fatfs上的图片

- 22.小熊派从SD卡拷贝UI资源到QSPI_FLASH上，并通过STemWin进行展示

- 23.真随机数产生

- 24.RJ45串口模块例程

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request
