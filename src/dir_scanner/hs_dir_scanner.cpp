#include "hs_dir_scanner.h"
#include "t_queue.cpp"
#include <filesystem>
#include <iostream>

#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>


namespace fs = std::filesystem;

void worker(TQueue<std::string> &queue, AbstractFileScanner &handler) {
    while (true) {
        auto file = queue.Pop();

        if (!file.has_value()) break;
        if (file->empty()) break;

        std::cout << "Thread " << std::this_thread::get_id()
            << " processing: " << *file << "\n";



        handler.scan_file(file.value());
    }
}

HSDirScanner::HSDirScanner(AbstractFileScanner &handler) : AbstractDirScanner(handler) {}

void HSDirScanner::scan(const std::string &root) {
    std::error_code ec;
    fs::path p(root);

    if (fs::is_regular_file(p, ec)) {
        handler_.scan_file(p.string());
        return;
    }

    if (!fs::is_directory(p, ec)) {
        std::cerr << "Path is neither file nor directory: " << root << " (" << ec.message() << ")\n";
        return;
    }

    TQueue<string> queue;

    thread t1(worker, ref(queue), ref(handler_));
    thread t2(worker, ref(queue), ref(handler_));

    try {
        for (auto &entry : fs::recursive_directory_iterator(p)) {
            if (!entry.is_regular_file()) continue;
            queue.PushItem(entry.path().string());
        }
    } catch (const fs::filesystem_error &e) {
        std::cerr << "Filesystem error while scanning " << root << ": " << e.what() << "\n";
    }

    queue.Close();

    t1.join();
    t2.join();


}
