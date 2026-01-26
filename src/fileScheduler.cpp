#include "fileScheduler.hpp"
#include <chrono>
#include <qmutex.h>

FileScheduler::FileScheduler(const Task &task, QObject *parent)
    : QObject(parent), _task(task), _runningThreads(0) {
  applyTask();
}

void FileScheduler::onSetTask(const Task &task) {
  {
    QMutexLocker lock(&_taskAccessMutex);
    _task = task;
  }
  applyTask();
}

void FileScheduler::processQuery() {
  // TODO: implement
}

void FileScheduler::applyTask() {
  QMutexLocker lock(&_taskAccessMutex);
  _queryTimer.stop();
  _queryTimer.setSingleShot(_task._singleShot);

  if (_task._queryInterval.count() > 0) {
    _queryTimer.setInterval(_task._queryInterval);
  } else {
    _queryTimer.setInterval(std::chrono::milliseconds{0});
    _queryTimer.setSingleShot(true);
    // Fallback to single shot if the duration is negative
    // TODO: maybe change fallback logic
  }

  if (_task._run) {
    _queryTimer.start();
  }
}

void FileScheduler::onTimer() { processQuery(); }

void FileScheduler::onModifierProgress(const FileModifier::Progress &progress) {
  emit showUserInfo(progress._info);
  // TODO: complete
}

void FileScheduler::onModifierFinished(const FileModifier::Progress &progress) {
  _runningThreads--;
  emit showUserInfo(progress._info);
}
