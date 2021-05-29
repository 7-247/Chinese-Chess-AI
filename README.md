# 食用方式
下载或clone本分支，双击运行.bat文件，自动生成.exe并运行。

如遇编码错误，请修改bat文件中的936为65001（或相反）。

若还不能解决问题，请打开vs code，将position.cpp的编码从UTF8改为GBK（或相反）。

# 已完成
## 1、3层全量搜索，2、4层（对手）中量搜索，5-8层扩展搜索 
- 历史启发
- 迭代深化
- 极小窗口搜索
- 置换哈希（历史记录）
- 针对静止期的扩展搜索
- 残局杀着和困毙判定

# 未完成
## 区间滑块搜索
nodeNum[depth][nownode]
tree[node] {
    mychess
    int[5] childnode
    bool[5] childnode是否被抛弃
    int fathernode
    int evaluate
    int nowbestnode 
}
每次搜完一个节点，获得其evaluate和他的五个儿子的（node和mychess）和bestmove（nowbestnode）
每层全部搜完更新eva直到顶


## 唯一着法
// 唯一着法检验是ElephantEye在搜索上的一大特色，用来判断用以某种深度进行的搜索是否找到了唯一着法。
// 其原理是把找到的最佳着法设成禁止着法，然后以(-WIN_VALUE, 1 - WIN_VALUE)的窗口重新搜索，
// 如果低出边界则说明其他着法都将被杀。

## 杀手着法



vector<int> nodeNum[MaxDepth + 5];
int totalNode;
struct tree {
    PositionStruct chess;
    int childnode[5];
    bool childuse[5];  // 1在用 0抛弃
    int childmove[5];
    int fathernode, eval, nowbestnode;
};
vector<tree> node;

struct allmoves {
    PositionStruct chess;
    int eval, move;
} am[200];