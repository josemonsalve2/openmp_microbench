#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <vector>

#include <omp.h>

#ifdef USE_NOWAIT
#define NOWAIT nowait
#else
#define NOWAIT
#endif

void kernel(double a, const double *x, double *y, int T, int N) {
#ifdef OUTER_MAP
#pragma omp target teams distribute parallel for simd NOWAIT
#else
#pragma omp target teams distribute parallel for simd map(to: x[0:N]) map(tofrom: y[0:N]) NOWAIT
#endif
  for (int i = 0; i < N; ++i) {
    // Classical FMA
    y[i] = y[i] + x[i] * a;
  }
}

void b1(double a, const double *x, double *y, int T, int N) {
#ifdef OUTER_MAP
#pragma omp target data map(to: x[0:N]) map(tofrom: y[0:N])
  {
#endif
  for (int t = 0; t < T; ++t) {
    kernel(a, x, y, T, N);
  }
#ifdef USE_NOWAIT
// taskwait here is needed as we need to make sure all tasks have done before
// return from this function
#pragma omp taskwait
#endif
#ifdef OUTER_MAP
  }
#endif
}

void b2(double a, const double *x, double *y, int T, int N) {
#ifdef OUTER_MAP
#pragma omp target data map(to: x[0:N]) map(tofrom: y[0:N])
  {
#endif
#pragma omp parallel for
    for (int t = 0; t < T; ++t) {
      kernel(a, x, y, T, N);
    }
#ifdef OUTER_MAP
  }
#endif
}

void b3(double a, const double *x, double *y, int T, int N) {
  std::atomic_bool done(false);
#ifdef OUTER_MAP
#pragma omp target data map(to: x[0:N]) map(tofrom: y[0:N])
  {
#endif
#pragma omp parallel
  {
    if (omp_get_thread_num() == 0) {
      b1(a, x, y, T, N);
      done.store(true);
    } else {
      while (!done.load())
        ;
    }
  }
#ifdef OUTER_MAP
  }
#endif
}

const auto bench = {b1, b2, b3};
constexpr const char *bench_name[] = {"b1", "b2", "b3"};

void run(int id, int M, double a, const double *x, double *y, int T, int N) {
  const auto func = bench.begin() + id;
  for (int k = 0; k < M; ++k) {
    const auto start = std::chrono::high_resolution_clock::now();
    (*func)(a, x, y, T, N);
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    printf("%s - T: %d, N: %d, k: %d, time: %ld ms\n", bench_name[id], T, N, k,
           duration.count());
  }
}

int main(int argc, char *argv[]) {
  // Initialize random device and generator
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(1.0, 10.0);

  // Set up size
  const std::vector<int> sizes = {1 << 10, (1 << 12) - 1, 1 << 14,
                                  (1 << 16) - 1, 1 << 18};
  const auto num_size = sizes.size();
  const auto buffer_size = sizes.back();
  const std::vector<int> times = {1 << 4, 1 << 8, 1 << 12};
  const auto num_time = times.size();

  // Allocate data. In order to speed up, we do not allocate data for each
  // kernel size. Instead, we allocate them of the larget size and just use
  // different first piece of them.
  std::vector<double> x(buffer_size);
  std::vector<double> y(buffer_size);

  // Initialize the buffer with random number
  for (int i = 0; i < buffer_size; ++i) {
    x[i] = dis(gen);
    y[i] = dis(gen);
  }
  // Initialize a scalar
  const double a = dis(gen) / 10.0;
  // Run the function for M times
  const int M = 100;

  // Execute difference benchmarks with different size and execution time
  for (int i = 0; i < num_size; ++i) {
    const int N = sizes[i];
    for (int j = 0; j < num_time; ++j) {
      const int T = times[j];
      for (int k = 0; k < bench.size(); ++k) {
        run(k, M, a, x.data(), y.data(), T, N);
      }
    }
  }

  return 0;
}
