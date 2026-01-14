#include "dir_scanner.h"
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
        // std::cout <<"Thread " << this_thread::get_id() << " file: " << *file << endl;
        handler.scan_file(file.value());
    }
}


void DirScanner::scan(const std::string &root) {
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

    vector<thread> threads;

    for (int i=0; i<threads_; i++) {
        threads.emplace_back(worker, ref(queue), ref(handler_));
    }

    try {
        for (auto &entry : fs::recursive_directory_iterator(p)) {
            if (!entry.is_regular_file()) continue;
            queue.PushItem(entry.path().string());
        }
    } catch (const fs::filesystem_error &e) {
        std::cerr << "Filesystem error while scanning " << root << ": " << e.what() << "\n";
    }

    queue.Close();

    for (auto &t : threads) {
        t.join();
    }

}
