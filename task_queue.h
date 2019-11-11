//
// task_queue.h
// Created by TheFatNinja 
// 11-10-2019
//

#ifndef REDIS_VIEWER_TASK_QUEUE_H
#define REDIS_VIEWER_TASK_QUEUE_H

#include <uv.h>
#include <functional>
#include <memory>

template <typename T>
struct Task{
    T data;
    std::function<T(void)> task_method = nullptr;
    std::function<void(T&, const std::string&)> callback = nullptr;
    std::string error;
};

struct RunLaterTask {
    std::function<void(void)> callback = nullptr;
};

class TaskQueue {

private:
    uv_loop_s *loop;

public:
    TaskQueue();
    ~TaskQueue();

    template <typename T>
    void RunTask(const std::function<T(void)> &task_method, const std::function<void(T&, const std::string&)> &callback) {
        auto *task = new Task<T>();
        task->task_method = task_method;
        task->callback = callback;
        auto *req = new uv_work_t;
        req->data = task;
        uv_work_cb work_cb = [](uv_work_t *req) {
            auto *task = static_cast<Task<T>*>(req->data);
            try {
                task->data = task->task_method();
            }
            catch (std::exception &ex) {
                task->error = ex.what();
            }
        };
        uv_after_work_cb after_cb = [](uv_work_t *req, int status) {
            auto *task = static_cast<Task<T>*>(req->data);
            task->callback(task->data, task->error);
            delete(req);
            delete(task);
        };
        uv_queue_work(loop, req, work_cb, after_cb);
    }

    void RunLater(const std::function<void(void)> &callback);
    void ProcessCallbacks();

};

typedef std::shared_ptr<TaskQueue> TaskQueuePtr;

TaskQueuePtr GetTaskQueue();

#endif //REDIS_VIEWER_TASK_QUEUE_H
