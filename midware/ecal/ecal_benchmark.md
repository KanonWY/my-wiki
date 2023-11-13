## ecal Benchmark

## 1、protobuf benchmark

![](/run/user/1000/doc/f2ea8cb1/protobufParaser.png)

实际数据序列化与反序列化耗时：

总体在 1ms 以下。

```bash
// IMAGE
// 6M
// 450 us

// PCL
// 1M
// 50 us

// LocationData
//  319 Byte
//  Parser     1.5 us
//  Serialize  1  us
```



## 2、单订阅

## 3、多订阅

### 3.1 多个订阅测试，进程内通信测试 (inProc)

发布数：1；订阅数：3

```bash
al_intra
msg size = 5 MB
Main thread(publisher thread ) = 140457189808064
sub1 callback thread = 140456599193344
sub1 callback thread = 140456599193344
sub1 callback thread = 140456599193344
sub1 timeout = 703539 89 us
sub2 timeout = 592192 89 us
sub3 timeout = 473753 89 us
-------------------------------------------------------------
sub1 timeout = 7904 us
sub2 timeout = 6653 us
sub3 timeout = 5323 us
```

启动零拷贝

```bash
msg size = 5 MB
Main thread(publisher thread ) = 140240492206016
sub1 callback thread = 140239904671488
sub1 callback thread = 140239904671488
sub1 callback thread = 140239904671488
sub1 timeout = 578417 89 us
sub2 timeout = 461603 89 us
sub3 timeout = 333203 89 us
-------------------------------------------------------------
sub1 timeout = 6499 us
sub2 timeout = 5186 us
sub3 timeout = 3743 us
```

可以看到进程内通信，对于同一个消息，订阅者之间是**相互阻塞**的。

### 3.2 多个订阅者测试，进程之间通信（SHM）

发布数：1；订阅数：3

```bash
main thread(sleep thread) = 140512332173632
sub1 callback thread = 140511901890304
sub1 callback thread = 140511901890304
sub1 callback thread = 140511901890304
sub1 timeout = 655971 88 us
sub2 timeout = 541435 88 us
sub3 timeout = 414112 88 us
-------------------------------------------------------------
sub1 timeout = 7454 us
sub2 timeout = 6152 us
sub3 timeout = 4705 us
```

启动零拷贝

```bash
main thread(sleep thread) = 140480327967040
sub1 callback thread = 140479899354880
sub1 callback thread = 140479899354880
sub1 callback thread = 140479899354880
sub1 timeout = 523670 89 us
sub2 timeout = 401088 89 us
sub3 timeout = 261993 89 us
-------------------------------------------------------------
sub1 timeout = 5883 us
sub2 timeout = 4506 us
sub3 timeout = 2943 us
```

### 3.3 多个订阅者测试，进程之间使用 raw 接口(但是使用了 pb 底层序列化)

```bash
sub3 thread = 140669993588480
sub2 thread = 140669993588480
sub1 thread = 140669993588480
sub1 timeout = 706203 87 us
sub2 timeout = 617109 87 us
sub3 timeout = 511543 87 us
-------------------------------------------------------------
sub1 timeout = 8117 us
sub2 timeout = 7093 us
sub3 timeout = 5879 us
```

启用零拷贝

```bash
sub3 thread = 140667871282944
sub2 thread = 140667871282944
sub1 thread = 140667871282944
sub1 timeout = 585798 88 us
sub2 timeout = 487729 88 us
sub3 timeout = 373770 88 us
-------------------------------------------------------------
sub1 timeout = 6656 us
sub2 timeout = 5542 us
sub3 timeout = 4247 us
```

### 3.4 多个订阅测试，使用struct 进行底层传输？



