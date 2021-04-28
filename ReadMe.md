

```c
/*---写在前面

	这是本人在大三上实验课程学习《30天自制操作系统》该书过程中的一些记录，
	正值考试周，考完电路分析之后有点空闲时间，便把综合设计做的作业放在这里，
	希望能给需要的学弟学妹一些参考。
	
	我的这个综合设计的开机密码是"WESTWORLD"，如果你要跑的话。
	对于在做综合设计时想要导入图片的同学，可以参考一下picture_process.py文件，可直接使用，代码已附详注。
	
	你所需要修改的主要文件：bootpack.h  Makefile  bootpack.c  pacman.c   
	ps:当时为了赶进度，出于方便，在bootpack.c里面放了一些很大的字符数组用来导入画图，导致bootpack.c代码又臭又长，打开时不要被吓到
	
---2021/1/14
```

### 贴一个运行效果
（其实角色死亡后的内容没好好弄，如果之后有闲心再来完善吧）
![img](https://github.com/nappingman/HariboteOS/blob/master/images/running.gif)

