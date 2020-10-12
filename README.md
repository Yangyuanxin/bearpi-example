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



#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request
