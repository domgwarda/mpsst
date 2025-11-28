#include "directory_scanner_parallel.h"
#include "thread_queue.h"
#include "abstract_file_scanner.h"

#include <filesystem>
#include <thread>
#include <vector>
#include <atomic>
#include <iostream>
#include <chrono>

namespace fs = std::filesystem;

DirectoryScannerParallel::DirectoryScannerParallel(AbstractFileScanner &handler, unsigned workers)
    : handler_(handler) {
    if (workers == 0) {
        unsigned hc = std::thread::hardware_concurrency();
        num_workers_ = (hc == 0) ? 2u : hc;
    } else {
        num_workers_ = workers;
    }
}

void DirectoryScannerParallel::scan(const std::string &root) {
    using clock = std::chrono::steady_clock;
    auto t_start = clock::now();

    ThreadQueue<std::string> queue;
    std::atomic<uint64_t> files_processed{0};

    // Producer thread - DFS
    std::thread producer([&queue, &root]() {
        std::error_code ec;
        fs::path p(root);

        if (fs::is_regular_file(p, ec)) {
            queue.push(p.string());
            queue.close();
            return;
        }

        if (!fs::is_directory(p, ec)) {
            std::cerr << "Path is neither file nor directory: " << root << " (" << ec.message() << ")\n";
            queue.close();
            return;
        }

        try {
            for (auto &entry : fs::recursive_directory_iterator(p)) {
                if (!entry.is_regular_file()) continue;
                queue.push(entry.path().string());
            }
        } catch (const fs::filesystem_error &e) {
            std::cerr << "Filesystem error while scanning " << root << ": " << e.what() << "\n";
        }
        queue.close();
    });

    // Worker threads
    std::vector<std::thread> workers;
    workers.reserve(num_workers_);
    for (unsigned i = 0; i < num_workers_; ++i) {
        workers.emplace_back([&queue, &files_processed, &handler = this->handler_]() {
            while (true) {
                auto opt = queue.pop();
                if (!opt.has_value()) break;
                const std::string path = std::move(opt.value());
                try {
                    handler.scan_file(path);
                } catch (const std::exception &e) {
                    std::cerr << "Worker exception scanning " << path << ": " << e.what() << "\n";
                } catch (...) {
                    std::cerr << "Worker unknown exception scanning " << path << "\n";
                }
                ++files_processed;
            }
        });
    }

    producer.join();
    for (auto &t : workers) if (t.joinable()) t.join();

    auto t_end = clock::now();
    std::chrono::duration<double> elapsed = t_end - t_start;
    std::cerr << "[timing] scanned_files=" << files_processed.load()
              << " elapsed=" << elapsed.count() << "s"
              << " avg_time_per_file="
              << ((files_processed > 0) ? (elapsed.count() / static_cast<double>(files_processed.load())) : 0.0)
              << "s\n";
}