# Ray Tracer
A CPU based Ray Tracer (Path Tracer) written in C++, implemented following [RayTracingInOneWeekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) by Peter Shirley.
![alt text](https://github.com/flummoxedShubh/PathTracer/blob/master/RayTracingInOneWeekend/RenderFinal.ppm](https://github.com/flummoxedShubh/PathTracer/blob/master/Render.png)

✅ What's New in This Version
🧵 Multithreading the Render Loop
'''cpp
    std::vector<std::thread> threads;
		int chunkSize = imageHeight / threadCount;

		std::vector<std::stringstream> streams(threadCount);

		for (int i = 0; i < threadCount; i++)
		{
			int start = i * chunkSize;
			int end = (i == threadCount - 1) ? imageHeight : start + chunkSize;
			threads.emplace_back([this, start, end, &stream = streams[i], &world]() { this->WritePixel(start, end, stream, world); });
		}

		for (auto& t : threads) t.join();
  '''
💾 File I/O
'''cpp
std::ofstream file("Render.ppm");
file << "P3\n" << ImageWidth << " " << imageHeight << "\n255\n";

for (const auto& ss : streams) {
    file << ss.str();  // Merge per-thread output
}
'''
📊 Real-Time Progress Indicator
- Displays the number of remaining scanlines in real time in the console.
- Helps estimate render progress during long jobs.

🎯 Conforms to Company C++ Style Guide
- Consistent spacing, brace placement, and naming conventions.
- Safe atomic progress tracking (std::atomic<int>).
- Clean separation of responsibilities (Initialize() method).
- System call (std::system("start Render.ppm")) opens the image automatically after rendering on Windows.
file.close();
std::system("start Render.ppm");
'''
