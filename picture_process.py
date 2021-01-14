"""
xp xp
Created on 2021/1/6
图片转灰度
"""
import matplotlib.pyplot as plt  # plt 用于显示图片
import matplotlib.image as mpimg  # mpimg 用于读取图片
import numpy
from PIL import Image

# 灰度转数值
def pretreatment(ima):
    ima = ima.convert('L')  # 转化为灰度图像
    # 以下两行代码供调试时使用
    # 当你需要把图像中特定区域提取，但不知道其具体灰度值时
    # 取消下列两行的注，运行，鼠标停放处会显示鼠标所指向对应位置的灰度值
    # 颜色从"纯黑色"到"纯白色"，灰度值变化范围是[0,255]

    # plt.imshow(ima)
    # plt.waitforbuttonpress()
    im = numpy.array(ima)  # 转化为二维数组
    for i in range(im.shape[0]):  # 转化为二值矩阵
        for j in range(im.shape[1]):
            # 此处根据需要提取的颜色进行修改
            # 可以有多个分支
            # 示例代码中的意思是：提取图片中黑色的部分，其余部分当噪音处理，全部记"1"
            # 所以最后只要把字符数组中为"0"的按相应的颜色打印就好了

            if im[i, j] >= 50:
                im[i, j] = 1
            else:
                im[i, j] = 0
    return im
ima = Image.open('czz.jpg')  # 读入图像
im = pretreatment(ima)  # 调用图像预处理函数
f = open('array.txt', 'w')  # 文件输出

# 数组过大时，在bootpack.c中直接static char array[][]使用会报错
# 需要使用类似第五天中讲到的hankaku.txt导入的方式
# 此时使用以下代码输出，得到的txt文本先使用ctrl+F删除所有换行符号"\r\n"
# 再把所有的"!"替换为换行"\r\n"
# 最后把所有的0替换成"."，1替换成"*"，至于为什么，可能是编译工具"BIN2OBJ"的规定
# 注意①：使用这种方式时，还需要自己实现打印图案的函数，实现方式参考"Day 5"实验文件中的"bootpack.c"
# 注意②：如果了解了打印的实现方式，就发现这种方式只能呈现最多两种不同的颜色

'''
count = 0  # 字节形式输出
for i in range(im.shape[0]):
    for j in range(im.shape[1]):
        print(im[i, j], file=f)
        count = count + 1
        if count % 8 == 0:
            print("!", file=f)
'''
for i in im:  # 数组形式文件流输出
    print(i, file=f)
f.close()

# infile = open("array.txt", "r")  # 打开文件
# outfile = open("new_array.txt", "w")  # 内容输出

# for line in infile:  # 按行读文件，可避免文件过大，内存消耗
#    outfile.write(line.replace(' ', ''))  # first is old ,second is new
# for line in infile:  # 按行读文件，可避免文件过大，内存消耗
#    outfile.write(line = line.strip("")
# for line in infile:  # 按行读文件，可避免文件过大，内存消耗
#    outfile.write(line.replace(']', "\",\n"))  # first is old ,second is new
# for line in infile:  # 按行读文件，可避免文件过大，内存消耗
#    outfile.write(line.replace('[', "\""))  # first is old ,second is new
# infile.close()  # 文件关闭
# outfile.close()

# 得到文件后，离转化成字符数组还有一步：
# ①删除多余的空格,替换成空
# ②删除多余的空格,替换成空
# ③]替换成",\r\n
# ④[替换成"
# ⑤最后一行的','要删除
# ⑥复制到数组声明处
print(im.shape)  # 打印图像规模，即二维数组声明时的大小
