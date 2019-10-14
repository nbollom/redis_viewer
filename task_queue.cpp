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

}

void TaskQueue::ProcessCallbacks() {
    uv_run(loop, UV_RUN_NOWAIT);
}
