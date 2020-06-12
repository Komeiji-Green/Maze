# Maze
一个不算很复杂的迷宫游戏。
### 时间：2020-6-12
### 环境：VS2019
### 参考：[UNLOSTER/Maze](https://github.com/UNLOSTER/Maze)
***
游戏系统介绍
## 图形界面
基于 [EasyX库](https://easyx.cn/) ，自行编写了消息循环和组件类来实现UI框架。
## 游戏玩法
### 冒险模式
创建角色，然后  
从 **人间之里** 和 **迷途竹林** 中选择地图，  
从 **EASY NORMAL HARD LUNATIC** 中选择难度，  
地图的生成完全随机。  
可以存档读档、保存游戏进度。
### 练习模式
调整地图尺寸，选择一款地图。  
可以自动寻路、演示寻路（按任意键暂停）。
### 排行系统
通关后的游戏信息（分数、时间等）会被载入排行榜。  
  
#### 一轮游戏的分数构成  
        随时间增加1秒，分数-1，  
        捡到一个金币，分数+100，  
        踏入一块沼泽，分数-20。  
分数下限不低于0。  
### 存档系统
在进入冒险模式前，输入角色名。  
如果角色已创建，则加载对应角色信息，可以读入这个角色之前的存档。  
否则创建新角色。
## 算法
### 迷宫生成算法
参见Al-MazeGenerator.cpp  
生成算法总共有两个版本。  
#### V1.0：随机最小生成树  
        过程：先打乱全体边集序列，再利用Kruskal算法拆墙，最后得到最小生成树。  
        特点：无回路，当尺寸调大后，迷宫会比较“直”，不够蜿蜒。  
#### V2.0：参考 [地牢生成算法](https://indienova.com/indie-game-development/rooms-and-mazes-a-procedural-dungeon-generator/)  
        过程：  
            1.随机生成若干房间。  
            2.DFS挖通其他区域。  
            3.Kruskal沟通全体连通区域，在此过程中随机拆墙，增添回路。   
        特点：  
            1.够蜿蜒。  
            2.小心栈溢出（在release中已通过测试不会发生）。    
### 自动寻路算法
参见Al-AstarSearch.cpp  
经典的Astar算法，不再赘述。
