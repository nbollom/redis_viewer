//
// task_queue.cpp
// Created by TheFatNinja 
// 11-10-2019
//

#include "task_queue.h"

TaskQueue::TaskQueue() {
    loop = uv_default_loop();
}

TaskQueue::~TaskQueue() {
    uv_loop_delete(loop);
}

void TaskQueue::RunLater(const std::function<void(void)> &callback) {
    auto *task = new RunLaterTask();
    task->callback = callback;
    auto *req = new uv_work_t;
    req->data = task;
    uv_work_cb work_cb = [](uv_work_t *req) {
    };
    uv_after_work_cb after_cb = [](uv_work_t *req, int status) {
        auto *task = static_cast<RunLaterTask*>(req->data);
        task->callback();
        delete(req);
        delete(task);
    };
    uv_queue_work(loop, req, work_cb, after_cb);
}

void TaskQueue::ProcessCallbacks() {
    uv_run(loop, UV_RUN_NOWAIT);
}

static TaskQueuePtr task_queue = nullptr;

TaskQueuePtr GetTaskQueue() {
    if (task_queue == nullptr) {
        task_queue = std::make_shared<TaskQueue>();
    }
    return task_queue;
}
