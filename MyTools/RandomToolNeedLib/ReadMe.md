此文件夹下的代码的头文件和源文件分开来了，需要手动编译，但是我已经编译好了一份静态库放在 **..\..\lib\MyTools\RandomToolNeedLib**中（即顶层Library文件夹中的lib同名目录里面）。  
静态库的名字叫**LibForMTRandomAndPrimeSearch.lib**

# 类 MTRandom
使用梅森旋转法生成随机数，可以为每一个随机序列创建单独的实例，代码来自《游戏编程权威指南（第4版）》。

* MTRandom()
	* 构造函数，默认seed为1
* unsigned int Random(unsigned int)
	* 生成0到指定参数n的一个随机数，包括n
* float Random()
	* 生成0.0f到1.0f之间的随机浮点数。
* Randomize()
	* 使用当前的系统时间作为seed，相当于把骰子随机摇了一下，

# 类 PrimeSearch
使用素数的性质来生成一个数组的随机序号，用来随机遍历这个数组的元素，代码来自《游戏编程权威指南（第4版）》。
* PrimeSearch(int)
	* 构造函数，设置要遍历的数组大小
* int GetNext(bool restart = false)
	* 获取下一个遍历的需要，如果restart 传入 true，则会立即重新开始，当前返回的随机序号从新重第一个开始。
* bool Done()
	* 判断是否已经完成遍历。
* void InitSizeAndRestart(int)
	* 重新设置遍历的数组大小并且从头开始。
	

