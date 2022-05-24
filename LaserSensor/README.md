# **材料**

WT53R-TTL激光传感器

Arduino mega 2560 pro



# **连线**

![img](https://github.com/jiang-xin-859300/Vector-Tensor/blob/main/LaserSensor/reference.jpg)

​	使用mega板子的14 15口，对应的是TX3和RX3口，使用5V给传感器供电。

# **程序**

​	在使用该传感器时，应该先设置波特率115200。根据接线，此处使用Serial3.

``` c
Serial3.begin(115200);
```

**编写逻辑**

​	每1ms读取一次传感器返回值。该款传感器返回为ASCII码，直接定义char变量读取回字符，通过判断把不相关的字符过滤掉。

​	每次读取到数字时，创建数组继续读取，直到读取完这一次的距离返回值。

​	读到0则去除，是因为返回的ASCII码中，每次得到距离前会有一句话：“state 0”，要把这个“0”过滤掉。

​	最后烧录完成使用Arduino IDE读取串口数据。

