#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <future>
#include <vector>

// See benchmarking directory for testing this code

template <class SrcIt, class DstIt, class Func>
auto transform(SrcIt first, SrcIt last, DstIt dst, Func func) {
  // 循环不变式:
  // [first, last)是源数据待处理区间
  // [dst, )是目标数据待写入区间
  // func是变形函数
  while (first != last) {
    *dst++ = func(*first++);
  }
}

template <typename SrcIt, typename DstIt, typename Func>
auto par_transform_naive(SrcIt first, SrcIt last, DstIt dst, Func f) {
  // 待处理的元素个数
  auto n = static_cast<size_t>(std::distance(first, last));
  // 硬件CPU核心数
  auto n_cores = size_t{std::thread::hardware_concurrency()};
  // 并行处理的任务数
  auto n_tasks = std::max(n_cores, size_t{1});
  auto chunk_sz = (n + n_tasks - 1) / n_tasks;
  // 异步执行的结果集合
  auto futures = std::vector<std::future<void>>{};
  // Process each chunk on a separate task
  for (auto i = 0ul; i < n_tasks; ++i) {
    // 本块任务的起始元素下标
    auto start = chunk_sz * i;
    if (start < n) {
      // 本块任务的终止元素下标(不包含)
      auto stop = std::min(chunk_sz * (i + 1), n);
      // 异步执行本块任务
      auto fut = std::async(std::launch::async, [first, dst, start, stop, f]() {
        std::transform(first + start, first + stop, dst + start, f);
      });
      // 将本块任务的异步执行结果保存到futures中
      futures.emplace_back(std::move(fut));
    }
  }
  // Wait for each task to finish
  for (auto&& fut : futures) {
    fut.wait();
  }
}

//
// A divide and conquer version of par_transform()
//

template <typename SrcIt, typename DstIt, typename Func>
auto par_transform(SrcIt first, SrcIt last, DstIt dst, Func func,
                   size_t chunk_sz) {
  const auto n = static_cast<size_t>(std::distance(first, last));
  if (n <= chunk_sz) {
    std::transform(first, last, dst, func);
    return;
  }
  const auto src_middle = std::next(first, n / 2);

  // Branch of first part to another task
  auto future = std::async(std::launch::async, [=, &func] {
    par_transform(first, src_middle, dst, func, chunk_sz);
  });

  // Recursively handle the second part
  const auto dst_middle = std::next(dst, n / 2);
  par_transform(src_middle, last, dst_middle, func, chunk_sz);
  future.wait();
}
