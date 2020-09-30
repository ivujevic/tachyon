#pragma once

#include <queue>
#include <mutex>
#include <thread>
#include <future>
#include <numeric>
#include "Timer.hpp"

namespace utils {

template <class T>
struct ProducerConsumersPool {
    template<class Data, class Stream, class Fun, class Acc>
    static Data run(int numberOfThreads, Stream&& stream, Fun&& fun, Acc&& acc) {

        std::queue<T> tasks;
        std::mutex m_;
        bool finished = false;

        std::condition_variable canConsume;
        std::condition_variable canProduce;

        std::thread producer([&]{
            for (const auto& s : stream) {
                std::unique_lock<std::mutex> lock(m_);
                canProduce.wait(lock, [&]{ return tasks.size() < 10000; });

                tasks.push(s);
                lock.unlock();
                canConsume.notify_one();
            }

            std::unique_lock<std::mutex>lock (m_);
            finished = true;
            canConsume.notify_all();
        });

        auto consumerTask = [&] {
            Data data;

            while(true) {
                std::unique_lock<std::mutex> lock(m_);
                canConsume.wait(lock, [&]{return !tasks.empty() || finished;});

                auto tasksSize = (int) tasks.size();

                if (tasksSize == 0) break;

                auto task(tasks.front());
                tasks.pop();
                lock.unlock();

                if (tasksSize < 1000) canProduce.notify_one();

                fun(data, task);
            }

            return data;
        };

        std::vector<std::future<Data>> consumers;
        consumers.reserve(numberOfThreads);
        for (int i = 0; i < numberOfThreads - 1; ++i) {
            consumers.emplace_back(std::async(std::launch::async, consumerTask));
        }

        producer.join();
        consumers.emplace_back(std::async(std::launch::async, consumerTask));

        for (auto& it : consumers) {
            it.wait();
        }

        Data dst;
        for (auto& f : consumers) acc(dst, f.get());

        return dst;

       /* std::vector<Data> ds;
        ds.reserve(consumers.size());
        for (auto& f : consumers) {

            ds.push_back(std::move(f.get()));
        }

        if (ds.size() == 1) return std::move(ds.front());

        while(ds.size() > 2) {

            std::vector<Data> accumulate;
            numberOfThreads = (int) ds.size() / 2;

            std::vector<std::thread> ts;
            accumulate.reserve(numberOfThreads + 1);

            int idx = 0;
            for (int i = 0; i < (int) ds.size() - 1; i += 2) {
                accumulate.push_back(std::move(ds[i]));

                ts.emplace_back([&](int i, int idx) {
                    acc(accumulate[idx], ds[i]);
                }, i + 1, idx);
                idx++;
            }

            for (auto& t : ts) t.join();

            if (numberOfThreads % 2) {
                accumulate.push_back(std::move(ds.back()));
            }

            ds = std::move(accumulate);
        }

        acc(ds[0], ds[1]);

        return std::move(ds.front());*/

    }
};

struct ThreadPool {

    template<typename Data, typename T, typename F>
    static std::vector<Data> run(int numberOfThreads, const std::vector<T>& data, F&& f) {

        std::vector<Data> dst;
        dst.resize(data.size());

        std::queue<int> tasks;
        std::mutex m_;

        auto worker = [&] {
            while(true) {
                std::unique_lock<std::mutex> lock(m_);
                if ((int) tasks.size() == 0) return;

                auto idx(tasks.front());
                tasks.pop();

                lock.unlock();

                dst[idx] = f(data[idx]);
            }
        };

        for (int i = 0; i < (int) data.size(); ++i) {
            tasks.push(i);
        }

        std::vector<std::thread> threads;
        threads.reserve(numberOfThreads);

        for (int i = 0; i < numberOfThreads; ++i) {
            threads.emplace_back(worker);
        }

        for(auto& it : threads) it.join();

        return dst;
    }

};

}

