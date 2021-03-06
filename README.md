# THScore
一个用于记录打分成绩的小工具
## 说明
### 安装
从[release](https://github.com/denebwang/touhouScore/releases)下载最新版，解压至任意文件夹即可。
### 使用方法
### \*首次使用时，需要在路线编辑器保存你的路线！
#### 1. 记录工具
打开THScore.exe，程序开始等待游戏启动。

目前支持的游戏：`th10` `th11` `TH12`。请使用日文版，中文版不保证正常运行，并有炸游戏的风险。若无法读取到游戏，请尝试管理员启动。

进入游戏后，选择难度与机体直接开始游戏即可，程序会自动切换至对应路线。

#### 2. 路线编辑器
点击工具栏上的`路线编辑器`按钮进入路线编辑器。

通过页面上的3个下拉框，可以选择游戏、难度与机体。选择后，会自动切换至对应的路线，如果没有则会创建一个空的。

在有任意一项未选中时，将显示满足已选项的所有路线，双击路线直接进入编辑界面。
##### 修改分段
其中，通过第二列修改一个面的数据的分段方式，目前支持：
* 整面
* 道中+boss
* 道中+boss+结算分

选择时需要双击，选择后按回车或在任意其他位置单击即刻完成修改。
##### 修改路线数据
路线数据右侧有一个增量栏，填入该段的增量即刻自动更新对应的数值。编辑数值同样会自动更新增量。

更改某一节点数据时，会保持其余节点的增量不变，分数增加会自动应用到后续的所有节点。

完成编辑后，按下`保存`按钮保存路线。路线文件保存在根目录`/csv`文件夹下，UFO文件保存在`/csv/ufo`路径下。
### 如果未保存直接切换至别的机体，则所有数据将丢失！
保存后，本体暂时不能检测到更改，需要关闭本体窗口重新运行。
#### 3. UFO相关
#### UFO记录工具 
若游戏为`星莲船`，将自动打开一个UFO窗口，显示每个飞碟的信息。
#### UFO路线编辑器 
在路线编辑器中选择星莲船将自动打开UFO编辑窗口，在路线编辑器中选择对应的机体后即可开始编辑。右键可以增加或删除一行，输入对应的数据即可。保存时会检测关卡数输入是否正确。

### 注意事项
本程序涉及读取程序内存，虽然没有任何写入操作，但不保证游戏不会出现包括rep爆炸在内的意外状况，请谨慎使用

由于本人并非cs相关专业，代码水平有限。如果发现bug、程序停止运行等，或有功能建议，请发布[issue](https://github.com/denebwang/touhouScore/issues), 并附上相应log文件(位于/`logs`目录下)。同时欢迎大佬帮忙修改不合理的代码

## 本软件基于QT6.0.1msvc-64bit开发，遵循GPL V3协议。目前仅支持64位windows系统。
