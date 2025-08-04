# 🎨 Ray Tracer

A CPU-based Ray Tracer (Path Tracer) written in C++, implemented by following [RayTracingInOneWeekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) by Peter Shirley.

![Render](https://github.com/flummoxedShubh/PathTracer/blob/master/Render.png)

---

## ✅ What's New in This Version

### 🧵 Multithreading the Render Loop

```cpp
std::vector<std::thread> threads;
int chunkSize = imageHeight / threadCount;
std::vector<std::stringstream> streams(threadCount);

for (int i = 0; i < threadCount; i++) {
    int start = i * chunkSize;
    int end = (i == threadCount - 1) ? imageHeight : start + chunkSize;
    threads.emplace_back([this, start, end, &stream = streams[i], &world]() {
        this->WritePixel(start, end, stream, world);
    });
}

for (auto& t : threads) t.join();
```

> Splits rendering across hardware threads using row-wise distribution. Each thread writes to a separate `std::stringstream` to avoid race conditions.

---

### 💾 File-Based Output

```cpp
std::ofstream file("Render.ppm");
file << "P3\n" << ImageWidth << " " << imageHeight << "\n255\n";

for (const auto& ss : streams) {
    file << ss.str();  // Merge per-thread output
}

file.close();
std::system("start Render.ppm");
```

> Renders are now written to a `.ppm` file instead of piped to `std::cout`, and the image opens automatically after render (on Windows).

---

### 📊 Real-Time Progress Indicator

Displays the number of remaining scanlines in real-time:

```cpp
if (j % 10 == 0) {
    std::cout << "\rScanlines Remaining: " << remainingScanlines.load() << "   " << std::flush;
}
```

> Helps monitor long renders and confirms that multithreading is progressing.

---

### 🎯 Conforms to Company C++ Style Guide

- ✅ Consistent spacing, brace placement, and naming
- ✅ Use of `std::atomic<int>` for thread-safe logging
- ✅ Clean separation of setup logic into `Initialize()`
- ✅ Safe use of `std::ofstream`, `stringstream`, and system calls

---

## 🛠️ Building the Project

- Open the `.sln` file in **Visual Studio**
- Set build mode to `Release` or `Debug`
- Build and run

The output image (`Render.ppm`) will be saved and opened automatically.

---

## 📌 License

This project is educational and built on public-domain resources. If you plan to distribute or reuse this, consider adding an appropriate open-source license.
