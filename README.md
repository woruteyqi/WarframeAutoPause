简介
======
```WarframeAutoPause``` 别名 ```虚空睡觉助手``` 是一个开源软件，目的是帮助挂机的配置低、不熟练怕白给以及想要快速roll到挂机地形等玩家准备，并且拥有强大的锁定鼠标功能防止开核桃时鼠标回中

<details> <summary>使用截图</summary>
  
   ![image](https://github.com/woruteyqi/WarframeAutoPause/assets/20475872/a9e1ff93-42d9-4505-a3e2-e3f127eb9078)
   
   ![image](https://github.com/woruteyqi/WarframeAutoPause/assets/20475872/4a728d6e-2fc5-4bfe-94bf-8d09720098a9)
   
</details>

## 主要功能
- 检测氧气过低自动暂停
- 查询稳定的挂机地形
- 自动开核桃
- 锁定鼠标位置

## 安全性
- 使用硬件级别的驱动来进行键鼠操作```驱动拥有正常的签名```，使用你自己的键鼠设备来发送事件，没有凭空多出来的键鼠设备
- 使用图形检测、前台截图等操作，来间接获取数据，非内存读取

## 下载
- 在本项目的[Github Release](https://github.com/woruteyqi/WarframeAutoPause/releases)页面上，下载最新的稳定发布版
- 在本项目[Actions](https://github.com/woruteyqi/WarframeAutoPause/actions)页面上点击最新的```workflow runs```在下面找到```Artifacts```里面包含最新的自动构建

## 教程
- todo

## 编译
### 编译环境
* 操作系统：Windows 11 64位
* 编译器：VS2022 MSVC 64
### 第三方库
* [Interception](https://github.com/oblitum/Interception)(已在源码)
* [Op](https://github.com/WallBreaker2/op)(动态编译，静态链接)
