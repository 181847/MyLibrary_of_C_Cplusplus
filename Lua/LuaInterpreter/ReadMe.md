这是一个lua_State的包装类，用于在C++代码中方便的从lua中获取数据，这个类的主要目的是获取数据，而不是设置数据，基本上都是对栈顶的元素进行操作。

 * 任何错误的发生都会直接抛出SimpleException， 若无特别指明，下文中提到的异常均值SimpleException

构造函数：
函数类型    |   解释
-----------|------------
LuaInterpreter(void)  | 构建一个新的lua_State，并且设置为主线程（主线程意味着这个state能够在LuaInterpreter释放的时候closeState，*lua中支持多线程state的创建，但是穿件出来的子线程不能被单独释放*） 
LuaInterpreter(lua_State \*, bool isMainTread)  |  手动的包装一个lua_State，我们可以自己设置这个lua_State是不是主线程。

成员函数(**有的函数具有模板参数，但是这里没有具体的给出**)：
函数名  | 返回值|  解释
-------|--------|-----
Run()  | PLuaInterpreter（解释器本身，以下无特别指明均是指解释器本身） |  开启命令行下的交互模式，以标准输出和输入流进行lua命令的数据
GetStackSize() |  int |  获取栈的大小
ToNumberAndPop()  | NUMBER_TYPE（模板参数） |  返回栈顶的数字并弹栈，使用模板参数来阻止类型转换的报警
ToNumberAndPop(NUMBER_TYPE * outNumber)   | PLuaInterpreter |  获取数字并弹栈，将数字放进参数中指针指向的内存，返回luaInterpreter本身
ToIntegerAndPop()  |  INTEGER_TYPE  |  返回栈顶整数
ToIntegerAndPop(INTEGER_TYPE * outInteger)  | PLuaInterpreter  |  将整数放进
ToStringAndClear(char * buffer)  | PLuaInterpreter  |  将栈顶的字符串复制到指定的内存中，然后弹栈，注意有一个模板参数指明缓冲的大小，超过栈的大小会抛出异常
GetGlobal(const pchar)  | PLuaInterpreter |  从全局变量中将指定的变量压入栈顶
SetglobalAndPop(const pchar)  | PLuaInterpreter  |  将栈顶元素设置为全局变量并弹栈
GetFieldOnTop(const pchar)  | PLuaInterpreter  |  在栈顶为table的情况下，获取字符串为key的一个域并压入栈顶
GetIndexOnTop(const lua_Integer)  | PLuaInterpreter  | 在栈顶为table的情况下，获取数组形式的元素，压入栈顶 
Pop(void)  | PLuaInterpreter  |  弹出栈顶元素
DoFile(const pchar)  | PLuaInterpreter  |  执行lua脚本文件
IsNil(void)  | bool  |  判断栈顶元素是否为nil
If(function Condition, function Then, function Else)  | PLuaInterpreter  |  用于在操作LuaInterpreter的过程中保持操作的连续性而设置的分支函数，第一个是一个函数返回值为true或者false，如果返回true，**Then** 代表的函数就会被执行，否则执行 **Else**
ToUserdataAndClear(const pchar, function converter)  | USERDATA_TYPE\*  |  将栈顶元素转换为userdata，使用参数中的字符串作为metatable的名字检查类型，converter用于将void\*转换为指定的类型，这个函数有一个函数模板代表返回值的指针的原类型，converter默认将void\*直接转换为模板参数类型的指针类型，用户可以自定义converter（接受一个void\*，返回一个需要的类型指针）
ToUserDataAndClear(const pchar , USERDATA_TYPE \*\*, function converter)  | PLuaInterpreter  |  和上面一个函数的作用类似，只不过从参数中获取指针的地址然后设置指针内容。
Foreach(function work)  | PLuaInterpreter  |  确保栈顶是一个table， 遍历table中的元素，每遍历一次就调用一次work函数对解释器进行操作，遍历完之后保证栈的状态和遍历之前一样，每一次work调用之后程序也会自动清理栈顶来进行下一次的遍历，但是必须保证work中不能手动的pop出太多的元素，比如把遍历的table pop掉什么的那就没得玩了，所以work可以放心的push、get数据但是不要随意的pop（要有限制的pop）。每一次遍历都会将键值的相关信息传递到work函数中，详细信息请查看原函数声明。
Call(int , UINT, UINT)  | PLuaInterpreter  | 调用函数，指定函数在栈中的位置，参数的数量，返回值的数量 
Push...{Integer/Number/String}...(...)  |  PLuaInterpreter | 向栈顶压入数据 
LengthOfTop(...)  | ...  |  获取栈顶table的长度，只有数组部分，字符串为键值的部分不会考虑在内
ConstainStackSizeMax(functoin doWithInterpreter)  | PLuaInterpreter  | 执行一段对LuaInterpreter的操作，函数完成之后确保栈的最大数量不超过执行之前，但是不能保证执行完后，栈比执行前小。 
