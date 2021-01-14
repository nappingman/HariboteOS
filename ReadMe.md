

```c
/*---写在前面

	这是笔者在大三上实验课程学习《30天自制操作系统》该书过程中的一些记录，
	正值考试周，考完电路分析之后有点空闲时间，便把综合设计做的作业放在这里，
	希望能给需要的学弟学妹一些参考。
	
	综合设计的开机密码是"WESTWORLD"
	对于在做综合设计时想要导入图片的同学，可以参考一下picture_process.py文件，可直接使用，里面也有注释提示。
	
	主要修改的文件：bootpack.h  Makefile  bootpack.c  pacman.c   
	ps:出于方便，在bootpack.c里面放了很多字符数组用来画图，导致该文加有点长，看起来有点丑陋......
	
---2021/1/14
```

### 运行效果

![img](https://github.com/nappingman/HariboteOS/blob/master/images/running.gif)

