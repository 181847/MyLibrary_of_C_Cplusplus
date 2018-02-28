这个目录下是一些常用的工具。

# Cleaner.h
里面包含一个清道夫类，使用RAII的思想来当函数结束调用的时候执行某个功能。

# MyTools.h
一个之前常用到的头文件，但是现在发现没有什么用，里面只有一个宏定义，用来在类的头文件中删除这个类的拷贝复制和拷贝复制构造。

# RandomTool.h
有关随机数组的方法定义。
* RandomTool::RandomSet，一个包装vector的随机数组类，生成随机数组成的数组，或者生成某个随机数组，数组中包含从1到n之间的数字，数字随机排列。
* RandomTool::Func::Shuffle()，将一个vector数组中的元素随机打乱。
* RandomTool::Func::Dispatch(), 将一个vector中的元素随机分发到多个vector数组中
	* 要求参数数组的大小必须预先初始化，第一个参数为被分发的对象，其余数组为接受对象
	* 分发过程涉及直接拷贝，请不要分发复杂类型数据
	* 所有子数组大小的和必须等于第一个数组的大小，否则会报错。

# TypeTool.h
* TypeTool::IDGenerator，一个使用函数静态数据特性为类型添加顺序ID的类模板
	* 这个类型ID虽然与类型直接相关，但是由于只有在函数调用的时候才能生成对应类型ID，所以**不应该**将这个类型ID用于任何编译期的判断。
* TypeTool::IsOneOf<...>::value， 判断一个类型是否属于之后一些列类型的模板函数，
* TypeTool::IsAllOf<TypeContainer<...>, ...>，IsOneOf的升级版，用于判断多个类型是否都属于一个类型集合。

# UnitTestModules.h
使用控制台程序设计的基础单元测试框架，用于方便测试代码，显示测试结果。

# LuaTools.h
定义了为Lua编写库、创建UserData的常用函数。

# 其他一些文件不是太常用，这里就不过多介绍了。
