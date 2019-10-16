//
// task_queue.h
// Created by TheFatNinja 
// 11-10-2019
//

#ifndef REDIS_VIEWER_TASK_QUEUE_H
#define REDIS_VIEWER_TASK_QUEUE_H

#include <uv.h>
#include <functional>

template <typename T>
struct Task{
    T data;
    std::function<T(void)> task_method = nullptr;
    std::function<void(T)> callback = nullptr;
};

class TaskQueue {

private:
    uv_loop_s *loop;

public:
    TaskQueue();
    ~TaskQueue();

    template <typename T>
    void RunTask(const std::function<T(void)> &task_method, const std::function<void(T)> &callback) {
        auto *task = new Task<T>();
        task->task_method = task_method;
        task->callback = callback;
        auto *req = new uv_work_t;
        req->data = task;
        uv_work_cb work_cb = [](uv_work_t *req) {
            auto *task = static_cast<Task<T>*>(req->data);
            task->data = task->task_method();
        };
        uv_after_work_cb after_cb = [](uv_work_t *req, int status) {
            auto *task = static_cast<Task<T>*>(req->data);
            task->callback(task->data);
            delete(req);
            delete(task);
        };
        uv_queue_work(loop, req, work_cb, after_cb);
    }

    void ProcessCallbacks();

};

#endif //REDIS_VIEWER_TASK_QUEUE_H