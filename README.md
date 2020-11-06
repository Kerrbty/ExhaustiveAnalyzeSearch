# ExhaustiveAnalyzeSearch

内存暴力搜索工具，根据json的指定方式进行特征码搜索



**场景1：在某个进程内搜索秘钥（例如QQ号码），那么执行如下规则 **

```json
[
	{
		"name": "QQ_PASSWORD",    // 名称,作标记用 
		"page_type": "rw",        // 搜索内存也属性 
		"offset": 0x3000,         // 开始搜索内存地址 
		"search_string": "\\Tencent\\QQMiniDL\\",  // 搜索字符串 
		"continue":{
			"offset": 18,            // 相对于上一步搜索到的位置偏移18字节 
			"compare_char": "\\",    // 第18个字节为斜杆 
			"continue":{
				"offset": 1,      // 获取上一步斜杆之后的偏移地址 
				"ret_addr": false // 将该地址返回，false表示不继续搜索，如果为true则还要继续从最外层开始搜索 
			}
		},
		"index":0
	}
]

```





**场景2：某个程序需要在某个点打补丁（例如 微信防消息撤回补丁), 那么执行如下规则 **

```json
[
	{
		"name": "WeChat_Back",    // 名称,作标记用 
		"page_type": "rx",        // 只搜索代码段(读执行权限) 
		"offset": 0,              // 开始搜索内存地址 
		"search_byte": "E8.{8} 83C408 6A0C 68.{8} 8D8D.{2}FFFFFF E8", // 搜索二进制串.{}为通配个数 
		"continue":{
			"offset": 11, 
			"compare_string": ",inner_flag:",  // 在搜索到代码的11偏移处湿push 一个字符串，拉出来判断是否搜索正确  
			"continue":{
                "offset": 200,    // 如果字符串正确，则继续在200以后搜索二进制  
                "search_byte": "F6C201 75.{2} 8B.{4} FF7604FF36 E8", 
                "continue":{
                    "offset": 0,
                    "ret_addr": false // 搜索到了补丁位置，返回  
                }
				
			}
		},
		"index":0
	}
]

```
