简介
======
```WarframeAutoPause``` 别名 ```虚空睡觉助手``` 是一个开源软件，目的是帮助挂机的配置低、不熟练怕白给以及想要快速roll到挂机地形等玩家准备，并且拥有强大的锁定鼠标功能防止开核桃时鼠标回中

[![build](https://github.com/woruteyqi/WarframeAutoPause/actions/workflows/msbuild.yml/badge.svg)](https://github.com/woruteyqi/WarframeAutoPause/actions/workflows/msbuild.yml)
<details> <summary>点击查看使用截图</summary>
  
   ![image](https://github.com/woruteyqi/WarframeAutoPause/assets/20475872/71299979-419e-4f68-9591-222d17cb565e)
   
   ![image](https://github.com/woruteyqi/WarframeAutoPause/assets/20475872/4a728d6e-2fc5-4bfe-94bf-8d09720098a9)
   
</details>

## 主要功能
- 检测氧气过低自动暂停
- 查询稳定的挂机地形
- 自动开核桃
- 锁定鼠标位置
- 漂泊者翻越进墙

## 安全性
- 使用硬件级别的驱动来进行键鼠操作```驱动拥有正常的签名```，使用你自己的键鼠设备来发送事件，没有凭空多出来的键鼠设备
- 使用图形检测、前台截图等操作，来间接获取数据，非内存读取
- 在读取EE文件查询地形时会复制一个副本在程序根目录下来读取这个副本，读取完成会自动删除

## 下载
- 在本项目的[Github Release](https://github.com/woruteyqi/WarframeAutoPause/releases)页面上，下载最新的稳定发布版
- 在本项目的[Github Actions](https://github.com/woruteyqi/WarframeAutoPause/actions)页面上，点击最新的```workflow runs```在下面找到```Artifacts```里面包含最新的自动构建

## 教程 
1. 下载好之后解压到一个文件夹里
2. 右键选择以管理员身份运行`安装驱动_右键以管理员身份运行.bat`，如果正常你会看到下面信息，重启电脑后就可以正常使用
      >
          Interception command line installation tool
          Copyright (C) 2008-2018 Francisco Lopes da Silva
          Interception successfully installed. You must reboot for it to take effect.
      
3. 双击打开**WarframeAutoPause.exe**，会出现提示`按键盘任意键初始化键盘设备`、`按鼠标任意键或移动位置初始化鼠标设备`，按照提示操作就可以了
如果你没有安装驱动就打开，程序将会在10秒后自行退出

## 编译
### 编译环境
* 操作系统：Windows 11 64位
* 编译器：VS2022 MSVC 64
### 第三方库
* [Interception](https://github.com/oblitum/Interception)(已在源码)
* [Op](https://github.com/WallBreaker2/op)(已编译)
