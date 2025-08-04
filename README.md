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

## 📄 License (MIT)

```
MIT License

Copyright (c) 2025 Shubh Sharma

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
