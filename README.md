# 食用方式
下载或clone本分支，双击运行.bat文件，自动生成.exe并运行。

如遇编码错误，请修改bat文件中的936为65001（或相反）。

若还不能解决问题，请打开vs code，将position.cpp的编码从UTF8改为GBK（或相反）。

# 已完成
## 1-4层全量搜索，5-6层中量搜索，7-10层静止期扩展搜索 
- 历史启发
- 迭代深化
- 极小窗口搜索
- 置换哈希（历史记录）
- 针对静止期的扩展搜索

# 未完成
缝合开局库（gzy）

# 已知问题
当迭代超过18层时，程序异常退出。
