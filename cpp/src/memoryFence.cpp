#include <bits/stdc++.h>

#include <thread>

// why intel only have Store-Load 乱序？

using namespace std;

inline void nocpufence(volatile int& x, volatile int& y, volatile int& r) {
    x = 1;
    // 防止编译器重排 compiler fence
    // 该指令可能会读取任意内存地址,也可能会改写任意内存地址,防止编译器级别的指令重排
    // 副作用: 让编译器把所有缓存在寄存器中的内存变量 flush 到内存中，然后重新从内存中取这些变量
    asm volatile("" ::: "memory");
    r = y;
}

// CPU 级别的memory fence.
// (1) 防止指令之间的重排序
// (2) 保证数据的可见性
//
//
// store A 的时候，由于缓存一致性协议等等原因，其他的 CPU 核心可能会获取到 A 的旧值。
// sfence 将store buffer 中的缓存刷新的 L1 Cache 中，使得其他 CPU 核心可以观测到这些修改，之后的 store 操作不会被调度到之前，即
// sfence 之前的Store 操作一定在 sfence 完成，并且全局可见。
//
// load A 的时候，由于由于 invlid queue的存在也许会读取到 旧值
//
// lfence A 的时候，会让 invlid queue ,强制读取 L1 cache中, lfence 之前的读操作一定在 lfence 完成,之后的读操作不会 重排到 lfence 之前。
//



inline void mfence(volatile int& x, volatile int& y, volatile int& r) {
    x = 1;
    asm volatile("mfence" ::: "memory");
    r = y;
}

inline void lockadd(volatile int& x, volatile int& y, volatile int& r) {
    x = 1;
    asm volatile("lock; addl $0,0(%%rsp)" ::: "memory", "cc");
    r = y;
}

inline void xchg(volatile int& x, volatile int& y, volatile int& r) {
    int tmp = 1;
    asm volatile("xchgl %0, %1" : "+r"(tmp), "+m"(x)::"memory", "cc");  // swap(x, tmp)
    r = y;
}

volatile int g_cnt = 0;

void threadfun(volatile int& loop_cnt, volatile int& x, volatile int& y, volatile int& r) {
    while (true) {
        while (g_cnt == loop_cnt)
            ;

        asm volatile("" ::: "memory");

        mfence(x, y, r);

        asm volatile("" ::: "memory");
        loop_cnt++;
    }
}

int main() {
    alignas(64) volatile int cnt1 = 0;
    alignas(64) volatile int cnt2 = 0;
    alignas(64) volatile int x    = 0;
    alignas(64) volatile int y    = 0;
    alignas(64) volatile int r1   = 0;
    alignas(64) volatile int r2   = 0;
    thread                   thr1(threadfun, ref(cnt1), ref(x), ref(y), ref(r1));
    thread                   thr2(threadfun, ref(cnt2), ref(y), ref(x), ref(r2));

    int detected = 0;
    while (true) {
        x = y = 0;
        asm volatile("" ::: "memory");
        g_cnt++;
        while (cnt1 != g_cnt || cnt2 != g_cnt)
            ;

        asm volatile("" ::: "memory");
        if (r1 == 0 && r2 == 0) {
            detected++;
            cout << "bad, g_cnt: " << g_cnt << " detected: " << detected << endl;
        }
    }
    return 0;
}
